#ifndef DEBUG_OUTPUT_H
#define DEBUG_OUTPUT_H

#include <iostream>

#include "utilities/utilities.h"

#if !defined(NDEBUG) || defined(_DEBUG)
#define DEBUG_OUTPUT(var, atom_factory) { const std::string &debug_output = utilities::to_str(*atom_factory, var); std::cout << #var   << "= " << debug_output << std::endl; }
#else
#define DEBUG_OUTPUT(var, atom_factory) do {} while(0)
#endif

#endif //DEBUG_OUTPUT_H
