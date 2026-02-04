#ifndef DEFUN_FORM_H
#define DEFUN_FORM_H

#include <string>

#include "form.h"
#include "cell_factory.h"

namespace lisp_runtime {
    namespace forms {

        class DefunForm : public Form{
        public:
            explicit DefunForm(std::shared_ptr<foundation::Foundation> foundation ,CellFactory *cell_factory) : Form(foundation, cell_factory) {}

            void Init() override;

            void Shutdown() override {}

            CellAdaptor Eval(LispRuntime *lisp_runtime, CellAdaptor expression, CellAdaptor context) const override;

            [[nodiscard]]
            std::string Name() const override;

            foundation::mem::Cell *lambda_atom_cell_;
            foundation::mem::Cell *fn_atom_cell_;
        };

    } // forms
} // lisp_runtime  

#endif //DEFUN_FORM_H
