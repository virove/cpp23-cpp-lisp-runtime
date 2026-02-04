#ifndef FORM_H
#define FORM_H

#include "cell_adaptor.h"
#include "lisp_runtime.h"

namespace lisp_runtime::forms {

        class Form {
        public:
            virtual ~Form() = default;

            virtual lisp_runtime::CellAdaptor Eval(LispRuntime* lisp_runtime,CellAdaptor expression, CellAdaptor context )const = 0;

            [[nodiscard]]
            virtual std::string Name() const = 0;
        };

} // lisp_runtime forms

#endif //FORM_H
