#include <iostream>
#include <memory>
#include "simple_lisp_runtime.h"

void lisp_runtime::SimpleLispRuntime::Init() {

}

void lisp_runtime::SimpleLispRuntime::Shutdown() {

}

lisp_runtime::SExpr lisp_runtime::SimpleLispRuntime::Eval(SExpr expression, SExpr context) {
    return SExpr();
}

lisp_runtime::SExpr lisp_runtime::SimpleLispRuntime::Apply(SExpr function, SExpr arguments, SExpr context) {
    return SExpr();
}

lisp_runtime::SimpleLispRuntime::~SimpleLispRuntime() {
}


