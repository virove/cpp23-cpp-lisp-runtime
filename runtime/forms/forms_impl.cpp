#include "forms_impl.h"
#include "atom_factory.h"

namespace lisp_runtime::forms {

        std::optional<const Form *> FormsImpl::FindForm(lisp_runtime::CellAdaptor form_name) const{
            assert(form_name.GetType() == foundation::mem::Cell::Type::AtomType);

            auto iterator = registered.find(form_name.GetAtom());

            return iterator == registered.end() ? std::nullopt : std::optional(iterator->second.get() ) ;
        }

        void FormsImpl::Register(const Form *form) {
            auto form_name_atom = cell_factory_->GetOrCreate(form->Name())->atom_;

            registered.emplace(form_name_atom, form);
        }

} // lisp_runtime::forms
