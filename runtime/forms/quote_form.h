#ifndef QUOTE_FORM_H
#define QUOTE_FORM_H

#include "form.h"

namespace lisp_runtime::forms {

    class QuoteForm : public Form{
    public:
        QuoteForm(std::shared_ptr<foundation::Foundation> foundation, CellFactory *cell_factory) : Form(
                foundation, cell_factory) {}

        void Init() override {}

        void Shutdown() override {}

        CellAdaptor Eval(LispRuntime *lisp_runtime, CellAdaptor expression, CellAdaptor context) const override;

        [[nodiscard]]
        std::string Name() const override;
    };

} // lisp_runtime::forms

#endif //QUOTE_FORM_H
