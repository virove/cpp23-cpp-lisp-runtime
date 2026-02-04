#include <string>
#include "quote_form.h"

namespace lisp_runtime::forms {

    CellAdaptor QuoteForm::Eval(LispRuntime *lisp_runtime, CellAdaptor expression, CellAdaptor context) const {
        return expression.Cdr().Car();
    }

    std::string QuoteForm::Name() const {
        return "quote";
    }

} // lisp_runtime::forms
