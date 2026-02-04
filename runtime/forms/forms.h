#ifndef FORMS_H
#define FORMS_H

#include <optional>

#include "cell_adaptor.h"
#include "form.h"

namespace lisp_runtime::forms {

    class Forms {
    public:
        virtual ~Forms() = default;

        virtual std::optional<const Form*> FindForm(lisp_runtime::CellAdaptor form_name) const = 0;

        virtual void Register(const Form* form)=0;
    };

} // lisp_runtime::forms

#endif //FORMS_H
