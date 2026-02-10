#include "atom_factory.h"
#include "defun_form.h"
#include "runtime.h"
#include "debug_output.h"

namespace lisp_runtime {
    namespace forms {
        CellAdaptor DefunForm::Eval(LispRuntime *lisp_runtime, CellAdaptor expression, CellAdaptor context) const {
            auto function_definition = expression.Cdr();
            auto function_name = function_definition.Car();
            auto function_parameter_list = function_definition.Cdr();
            DEBUG_OUTPUT(function_definition, GetFoundation()->GetAtomFactory());
            DEBUG_OUTPUT(function_name, GetFoundation()->GetAtomFactory());
            DEBUG_OUTPUT(function_parameter_list, GetFoundation()->GetAtomFactory());

            auto* lambda_definition = GetCellFactory()->CreateListCell();
            lambda_definition->head_ = lambda_atom_cell_;
            lambda_definition->tail_ = function_parameter_list.GetHead();


            DEBUG_OUTPUT(lambda_definition, GetFoundation()->GetAtomFactory());

            GetFoundation()->GetAtomFactory()->SetProperty(function_name.GetHead()->atom_,fn_atom_cell_->atom_,lambda_definition);

            return function_name;
        }

        std::string DefunForm::Name() const {
            return "defun";
        }

        void DefunForm::Init() {
            lambda_atom_cell_ = GetCellFactory()->GetOrCreate("lambda");
            fn_atom_cell_ = GetCellFactory()->GetOrCreate("fn");
        }


    } // forms
} // lisp_runtime