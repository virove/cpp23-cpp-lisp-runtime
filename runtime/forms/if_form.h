#ifndef IF_FORM_H
#define IF_FORM_H

#include "form.h"

namespace lisp_runtime {
    namespace forms {

        class If_Form : public Form{
        public:
            If_Form(const std::shared_ptr<foundation::Foundation> &foundation, CellFactory *cell_factory) : Form(
                    foundation, cell_factory) {}

            void Init() override;

            void Shutdown() override;

            CellAdaptor Eval(LispRuntime *lisp_runtime, CellAdaptor expression, CellAdaptor context) const override;

            std::string Name() const override;
        };

    } // forms
} // lisp_runtime  

#endif //IF_FORM_H
