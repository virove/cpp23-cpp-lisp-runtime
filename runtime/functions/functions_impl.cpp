#include "functions_impl.h"

namespace lisp_runtime {
    namespace functions {
        Functions::FindFunctionResult FunctionsImpl::FindFunction(const CellAdaptor &function_name) {
            assert(function_name.GetType() == foundation::mem::Cell::Type::AtomType) ;

            auto function_definition = atom_factory_->GetProperty(function_name.GetAtom() , atom_fn_ );

            if(function_definition.has_value()){
                return {lisp_runtime::CellAdaptor{function_definition.value()}};
            } else{
                const auto& builtin_function_optional = atom_factory_->GetFunctionProperty(function_name.GetAtom() , atom_builtin_function_property_ );

                if(builtin_function_optional.has_value()){
                    return {builtin_function_optional.value()};
                } else{
                    return {std::nullopt};
                }
            }

        }

        void FunctionsImpl::RegisterFunction(foundation::Function* function) {
            to_be_registered_.emplace_back(function);
        }

        void FunctionsImpl::Init() {
            atom_fn_ = cell_factory_->GetOrCreate("fn")->atom_;
            atom_builtin_function_property_ = cell_factory_->GetOrCreate("BUILT_FN")->atom_;

            while (!to_be_registered_.empty()){
                auto function = std::move(to_be_registered_.front());
                to_be_registered_.pop_front();

                foundation::ATOM function_name_atom = atom_factory_->GetOrCreate(function->GetName()).value()->atom_;
                atom_factory_->SetFunctionProperty(function_name_atom, atom_builtin_function_property_,
                                                   function.release());
            }
        }
    } // functions
} // lisp_runtime