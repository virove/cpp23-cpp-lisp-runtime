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

        void FunctionsImpl::RegisterBuiltinFunction(const std::string &name, BuiltinFunction function) {
            atom_factory_->SetFunctionProperty(atom_factory_->GetOrCreate(name).value()->atom_, atom_builtin_function_property_,
                                               lisp_runtime::functions::BuiltinAtomPredicate);
        }

        void FunctionsImpl::Init() {
            atom_fn_ = cell_factory_->GetOrCreate("fn")->atom_;
            atom_builtin_function_property_ = cell_factory_->GetOrCreate("BUILT_FN")->atom_;
        }
    } // functions
} // lisp_runtime