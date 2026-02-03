#include "builtin_function.h"

namespace lisp_runtime::functions {

foundation::mem::Cell* BuiltinAtomPredicate( const foundation::mem::Cell* cell_expression){
    lisp_runtime::CellAdaptor expression{cell_expression};

    assert(expression.GetHead()->GetType()==foundation::mem::Cell::Type::ListType);

    auto car_of_expression = expression.Car();

    return car_of_expression.IsAtom() ? foundation::Defines::T() : foundation::Defines::NIL();
}

} // namespace lisp_runtime::functions