#include "forms_impl.h"
#include "atom_factory.h"

namespace lisp_runtime::forms {

    std::optional<const Form *> FormsImpl::FindForm(lisp_runtime::CellAdaptor form_name) const{
        if(form_name.GetType() != foundation::mem::Cell::Type::AtomType){
            auto  d = form_name;
        }
        assert(form_name.GetType() == foundation::mem::Cell::Type::AtomType);

        auto iterator = forms_.find(form_name.GetAtom());

        return iterator == forms_.end() ? std::nullopt : std::optional(iterator->second.get() ) ;
    }

    void FormsImpl::Register(Form *form) {
        to_be_registered_.emplace_back(form);
    }

    void FormsImpl::Init() {
                while (!to_be_registered_.empty()){
                    auto form = std::move(to_be_registered_.front());
                    to_be_registered_.pop_front();

                    auto form_name = form->Name();
                    auto form_name_atom = cell_factory_->GetOrCreate(form_name)->atom_;

                    forms_.emplace(form_name_atom, std::move(form));
                }

                for (auto& [key, form] : forms_) {
                    form->Init();
                }
    }

    void FormsImpl::Shutdown() {
        for (auto& [key, form] : forms_) {
            form->Shutdown();
        }
    }

} // lisp_runtime::forms
