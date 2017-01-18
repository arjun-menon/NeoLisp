/*
 * Common universal header file
 *
 * For simplicity's sake, just include this header everywhere.
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
 * Project header files
 */

#include "utils.hpp"
#include "datatypes.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "eval.h"
#include "builtin.h"
#include "column.hpp"
#include "data_reader.hpp"
#include "table.hpp"
