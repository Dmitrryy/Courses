#pragma once

#include <sstream>
#include <iomanip>
#include <stack>

#include "../Compiler/builtins.h"
#include "../Compiler/utils.h"

#include "compiler.cpp.h"
#include "scanner.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/Support/TargetSelect.h"

namespace yy
{

    class Driver
    {
    private:
        Scanner *m_lexer = nullptr;
        mutable size_t m_numErrors = 0;

        std::istringstream m_strInput;
        std::vector<std::string> m_sourceByLine;

    public:
        llvm::LLVMContext context;
        llvm::Module * module = nullptr;

    public:
        Driver(const std::string &name) 
            : m_lexer(new Scanner(name))
            , module(new llvm::Module(name, context))
        {
        }

        // not supported
        Driver(const Driver &) = delete;
        const Driver &operator=(const Driver &) = delete;
        Driver(Driver &&) = delete;
        const Driver &operator=(Driver &&) = delete;

        ~Driver()
        {
            delete m_lexer;
            delete module;
        }

        parser::token_type yylex(parser::location_type *l, parser::semantic_type *yylval)
        {
            parser::token_type tokenType = static_cast<parser::token_type>(m_lexer->yylex());
            switch (tokenType)
            {
            case yy::parser::token_type::INTEGER_LITERAL:
            {
                yylval->as<int64_t>() = std::stoll(m_lexer->YYText());
                break;
            }

            case yy::parser::token_type::IDENTIFIER:
            {
                std::string word(m_lexer->YYText());
                parser::semantic_type tmp;
                tmp.as<std::string>() = word;
                yylval->swap<std::string>(tmp);
                break;
            }

            default:
            {
                break;
            }
            }

            *l = m_lexer->getLocation();
            return tokenType;
        }


        size_t parse()
        {
            parser parser(this);
            bool res = parser.parse();
            if (res)
            {
                std::cerr << "compile terminate!" << std::endl;
            }
            return m_numErrors;
        }

        void declare_builtins() {
            ezg::declareIntegerBuiltin(module);
        }

        void create_main(const std::string &entry_class, const std::string &entry_method) {
            llvm::IRBuilder<> builder(context);
            const auto linkage = llvm::GlobalValue::LinkageTypes::ExternalLinkage;

            auto&& main_Ty = llvm::FunctionType::get(builder.getVoidTy(), false);
            auto&& main_F = llvm::Function::Create(main_Ty, linkage, "main", module);

            llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", main_F);
            builder.SetInsertPoint(entryBB);

            auto&& class_name = ezg::mangl_class_name(entry_class);
            auto&& program_stract_Ty = llvm::StructType::getTypeByName(context, class_name);
            auto&& a1 = builder.CreateAlloca(program_stract_Ty);

            auto&& programm_F = module->getFunction(ezg::mangl_method_name(class_name, entry_method, {}));
            if(!programm_F) {
                throw std::runtime_error("Cant find method: " + entry_class + "." + entry_method);
            }
            auto&& a2 = builder.CreateCall(programm_F, {a1});

            builder.CreateRetVoid();
        }

        void dump_llvm(const std::string &fname) {
            std::cout << "Dump to: " << fname << std::endl;
            std::error_code EC;
            llvm::raw_fd_ostream file(fname, EC);
            std::cout << "Dump status: " << EC.message() << std::endl;
            module->print(file, nullptr);
            file.close();
        }

        void setSourceString(const std::string &source)
        {
            m_strInput.str(source);
            while (m_strInput)
            {
                std::string line;
                std::getline(m_strInput, line);
                m_sourceByLine.emplace_back(std::move(line));
            }

            m_strInput.str(source);
            m_lexer->switch_streams(m_strInput, std::cout);
        }

        void error(parser::context const &ctx, const std::string &msg) const
        {
            m_numErrors++;
            std::vector<parser::symbol_kind_type> expected_symbols(10);
            int size_es = ctx.expected_tokens(expected_symbols.data(), 10);

            auto loc = ctx.location();
            std::cerr << loc << ":Error: " << msg << std::endl;
            if (size_es > 0)
            {
                std::cerr << "Expected: ";
                for (int i = 0; i < size_es; i++)
                {
                    if (i != 0)
                    {
                        std::cerr << " or ";
                    }
                    std::cerr << "\'" << parser::symbol_name(expected_symbols[i]) << "\'";
                }
                std::cerr << " before \'" << parser::symbol_name(ctx.token()) << "\'" << std::endl;
            }

            std::cerr << "context: " << std::endl;
            for (size_t begl = loc.begin.line; begl <= loc.end.line; begl++)
            {
                std::cerr << begl << ' ' << m_sourceByLine[begl - 1] << std::endl;
            }

            const size_t ws = std::to_string(loc.end.line).size() + 1;
            std::cerr << std::setw(ws) << ' ';

            for (size_t i = 0; i < loc.end.column; i++)
            {
                if (i == ctx.lookahead().location.begin.column - 1)
                {
                    std::cerr << '^';
                }
                else
                {
                    std::cerr << '~';
                }
            }
            std::cerr << std::endl
                      << std::endl;
        }
    };

} // namespace yy