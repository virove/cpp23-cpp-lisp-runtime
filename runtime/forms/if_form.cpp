#include "if_form.h"
#include "debug_output.h"

namespace lisp_runtime::forms {
        void If_Form::Init() {
        }

        void If_Form::Shutdown() {
        }

        CellAdaptor If_Form::Eval(LispRuntime *lisp_runtime, CellAdaptor expression, CellAdaptor context) const {
            auto if_condition = expression.Cdr().Car();

            auto result = lisp_runtime->Eval(if_condition, context);
            if(!result.IsEmpty()){
                auto then_clause = expression.Cdr().Cdr().Car();

                return lisp_runtime->Eval(then_clause, context);
            } else{
                auto else_clause = expression.Cdr().Cdr().Cdr().Car();

                return lisp_runtime->Eval( else_clause, context);
            }
        }

        std::string If_Form::Name() const {
            return {"if"};
        }
} // lisp_runtime::forms