#include "simple_lisp_runtime.h"
#include "defines.h"

void lisp_runtime::SimpleLispRuntime::Init() {

}

void lisp_runtime::SimpleLispRuntime::Shutdown() {

}

lisp_runtime::SExpr lisp_runtime::SimpleLispRuntime::Eval(SExpr expression, SExpr context) {
    if(expression.GetType() == foundation::mem::Cell::Type::NumberType){
        return  expression;
    }

    return SExpr(foundation::Defines::NIL());
}

lisp_runtime::SExpr lisp_runtime::SimpleLispRuntime::Apply(SExpr function, SExpr arguments, SExpr context) {
    return SExpr(foundation::Defines::NIL());
}

lisp_runtime::SimpleLispRuntime::~SimpleLispRuntime() {
}


