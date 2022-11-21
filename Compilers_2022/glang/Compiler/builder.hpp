#pragma once

#include "llvm/IR/Module.h"
#include <memory>

#include "VarTable.h"

namespace ezg {

// TODO replace llvm-gen logic from gramar.y
class GLangBuilder final {
    struct idInfo {
        llvm::Value *value = {};
        llvm::StructType *type = {};

        // set if we need create load before set as function's argument
        bool to_load = false;
    };

    llvm::LLVMContext m_context;
    llvm::Module m_module;
    llvm::IRBuilder<> m_builder;

    std::string m_curClassName = "UNDEFINED";
    
    std::unordered_map< std::string, std::pair<size_t, std::string> > m_structFields;

    llvm::Value *m_this = nullptr;

    ezg::ScopeTable< std::string, idInfo > m_nameTable;

    // Problem: when a falseBB is created before opening an scope, 
    // but it must be put in the IRBuilder.SetInsertPoint after the if-scope, 
    // we need to save this BB
    std::stack< llvm::BasicBlock * > m_ifFalseBBs;
    // Same problem as with if
    std::stack< llvm::BasicBlock * > m_whileCondBBs;
    std::stack< llvm::BasicBlock * > m_whileFalseBBs;

public:
    GLangBuilder(const std::string &name) 
    : m_context()
    , m_module(name, m_context)
    , m_builder(m_context)
    {}

public:
    void genIntegerBuiltin();
    void genGraphicsBuiltin();

    void startClassDecl(const std::string& name);
    void endClassDecl();

    void declareClassField(const std::string& name, const std::string& type_name);

    /// @brief 
    /// @param name - method name
    /// @param params - vector pairs = <llvm type, string type name>
    void startMethodDecl(const std::string& name);

    void declareParametr(const std::string& name, const std::string& type_name);

    /// @brief 
    /// @param insert_ret_void - set true if need to emplace retVoid statement at the end
    void endMethodDecl(bool insert_ret_void = false);




}; // class GLangBuilder

} // namespace ezg