/*
 * Common header file for all *.cpp files in this project.
 *
 * Include this header everywhere. (This is just for simplicity's sake.)
 */

#pragma once

/*
 * C++ Standard Library headers
 */

#include <functional>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <numeric>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <list>
#include <map>

using namespace std;

/*
 * LLVM header files
 */

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

/*
 * Project header files
 */

#include "utils.hpp"
#include "datatypes.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "eval.h"
#include "matrix.hpp"
