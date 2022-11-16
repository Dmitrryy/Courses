#pragma once

#include <cstdint>

#include "llvm/IR/Module.h"

namespace ezg
{

llvm::StructType *declareIntegerBuiltin(llvm::Module *module);

llvm::Value *createIntegerByLiteral(llvm::Function *func, int64_t val);

}