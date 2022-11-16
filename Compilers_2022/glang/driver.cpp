#include <iostream>
#include <fstream>
#include <sstream>

#include "parser/driver.h"


std::string readFile(const std::string &fileName) {
    std::ifstream f(fileName);
    if (!f) {
        throw std::runtime_error("cant open file: " + fileName);
    }
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}


int main(int argc, char* argv[])
{
    yy::Driver driver(argv[1]);
    auto source = readFile(argv[1]);

    // declare builtins like Integer class
    driver.setSourceString(source);
    driver.declare_builtins();

    size_t n_err = driver.parse();
    if (n_err != 0) {
        std::cerr << "Bad compilation. Errors: " << n_err << std::endl;
        return 1;
    }

    // create main function with call "Program.Program"
    driver.create_main("Program", "Program");
    driver.dump_llvm("dump.ll");

    return 0;
}