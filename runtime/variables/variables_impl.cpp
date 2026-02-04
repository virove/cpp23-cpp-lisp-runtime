#include <stdexcept>
#include "variables_impl.h"
#include "utilities/utilities.h"

lisp_runtime::CellAdaptor
CreateBinding(lisp_runtime::CellAdaptor formal_parameters, lisp_runtime::CellAdaptor arguments,
              lisp_runtime::CellAdaptor links);

void lisp_runtime::vars::VariablesImpl::Init() {

}

void lisp_runtime::vars::VariablesImpl::Shutdown() {

}

lisp_runtime::CellAdaptor
lisp_runtime::vars::VariablesImpl::CreateVariableBinding(lisp_runtime::CellAdaptor formal_parameters, lisp_runtime::CellAdaptor arguments, lisp_runtime::CellAdaptor previous_binding )
{
    if(formal_parameters.IsEmpty()){
        return previous_binding;
    } else{
        const auto* new_pair = cell_factory_->CreateListCell(formal_parameters.GetHead()->head_, arguments.GetHead()->head_);

        auto binding = CreateVariableBinding( {formal_parameters.GetHead()->tail_}, {arguments.GetHead()->tail_}, previous_binding);

        auto* new_links_head = cell_factory_->CreateListCell(new_pair, binding.GetHead());

        return {new_links_head};
    }
}


void lisp_runtime::vars::VariablesImpl::SetVariableBinding(lisp_runtime::CellAdaptor adaptor) {

}

std::optional<lisp_runtime::CellAdaptor> lisp_runtime::vars::VariablesImpl::GetVariableValue(lisp_runtime::CellAdaptor expression,
                                                                              lisp_runtime::CellAdaptor context) const {
    auto pair_value = utilities::assoc(expression, context);
    if(pair_value.IsEmpty()){
        return std::nullopt;
    } else {
        return std::optional<lisp_runtime::CellAdaptor>{pair_value.GetHead()->tail_ };
    }
}






