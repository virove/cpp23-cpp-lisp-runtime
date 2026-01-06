#ifndef CPP23_CPP_LISP_RUNTIME_LISPRUNTIME_H
#define CPP23_CPP_LISP_RUNTIME_LISPRUNTIME_H

#include "s_expr.h"

namespace lisp_runtime{

    class LispRuntime{
    public:
        virtual ~LispRuntime() = default;
        virtual void Init()  = 0;
        virtual void Shutdown() = 0;
        virtual SExpr Eval(SExpr expression, SExpr context) = 0;
        virtual SExpr Apply(SExpr function, SExpr arguments, SExpr context) = 0;
    };

} //namespace lisp_runtime

#endif //CPP23_CPP_LISP_RUNTIME_LISPRUNTIME_H
