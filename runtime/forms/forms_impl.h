#ifndef FORMS_IMPL_H
#define FORMS_IMPL_H

#include <unordered_map>
#include <memory>
#include <list>

#include "atom_factory.h"
#include "forms.h"
#include "cell_factory.h"

namespace lisp_runtime {
    namespace forms {

        class FormsImpl : public Forms{
        public:
            explicit FormsImpl(std::shared_ptr<lisp_runtime::CellFactory> cell_factory) : cell_factory_{std::move(cell_factory)} {}

            void Init() override;

            void Shutdown() override ;

            std::optional<const Form *> FindForm(lisp_runtime::CellAdaptor form_name) const override;

            void Register( Form *form) override;
        private:
            std::list<std::unique_ptr<Form>> to_be_registered_;
            std::unordered_map<foundation::ATOM , std::unique_ptr<Form>> forms_;
            std::shared_ptr<lisp_runtime::CellFactory>    cell_factory_;
        };

    } // forms
} // lisp_runtime  

#endif //FORMS_IMPL_H
