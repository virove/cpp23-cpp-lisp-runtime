#ifndef BUILTIN_FUNCTION_H
#define BUILTIN_FUNCTION_H

#include "cell_adaptor.h"
#include "runtime.h"
#include "defines.h"

namespace lisp_runtime::functions {

    using BuiltinFunction = foundation::mem::Cell* (*)( const foundation::mem::Cell*);

    foundation::mem::Cell* BuiltinAtomPredicate( const foundation::mem::Cell* cell_expression);
} // list_runtime

#endif //BUILTIN_FUNCTION_H
