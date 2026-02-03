#include <variant>
#include <stdexcept>
#include <functional>

#include "simple_lisp_runtime.h"
#include "defines.h"
#include "utilities/utilities.h"
#include "functions/builtin_function.h"
#include "debug_output.h"

void lisp_runtime::SimpleLispRuntime::Init() {
}

void lisp_runtime::SimpleLispRuntime::Shutdown() {
}

lisp_runtime::SExpr lisp_runtime::SimpleLispRuntime::Eval(SExpr expression, SExpr context) {
    DEBUG_OUTPUT(expression, atom_factory_.get());
//    {
//        const std::string &debug_output = utilities::to_str(*atom_factory_, expression);
//        std::cout << "expression << " << "= " << debug_output << std::endl; }

    if(expression.GetType() == foundation::mem::Cell::Type::NumberType){
        return  expression;
    } else if(expression.GetType() == foundation::mem::Cell::Type::AtomType){
        if(expression.GetThisCell() == foundation::Defines::NIL()){
            return foundation::Defines::NIL();
        }
        else if(expression.GetThisCell() == foundation::Defines::T()){
            return foundation::Defines::T();
        }
        else{
            assert(!"Variables are not implemented");
        }
    }
    else if(expression.GetType() == foundation::mem::Cell::Type::ListType){

        auto form_name = expression.Car();

        if(auto form = forms_->FindForm(form_name) ; form.has_value()){

            return form.value()->Eval(this, expression, context);
        }
        else{
            auto function_name = expression.Car();

            auto find_function_result = functions_->FindFunction(function_name);

            auto function_parameters = expression.Cdr();

            return EvalFunction( find_function_result,function_name, function_parameters, context);
        }
    } else
    {
        assert(false);
    }
}



lisp_runtime::SExpr lisp_runtime::SimpleLispRuntime::Apply(SExpr function, SExpr arguments, SExpr context) {
    return SExpr(foundation::Defines::NIL());
}

lisp_runtime::SExpr
lisp_runtime::SimpleLispRuntime::EvalFunction(functions::Functions::FindFunctionResult find_function_result, CellAdaptor function_name,  CellAdaptor parameters,
                                              SExpr context) {
    if (const std::nullopt_t* none = std::get_if<std::nullopt_t>(&find_function_result)) {
        throw std::runtime_error("Failed to find function " + atom_factory_->GetAtomName(function_name.GetAtom()).value_or(" nil") );
    }
    const CellAdaptor &evaluated_arguments =  EvalList(  parameters, context);

    DEBUG_OUTPUT(evaluated_arguments, atom_factory_);

    if (auto function_definition = std::get_if<lisp_runtime::CellAdaptor>(&find_function_result)) {
        return Apply(*function_definition, evaluated_arguments, context);
    }
    else {
        auto builtin_function = std::get_if<lisp_runtime::functions::BuiltinFunction>(&find_function_result);

        return ApplyBuiltinFunction(*builtin_function, evaluated_arguments);
    }
}

lisp_runtime::CellAdaptor lisp_runtime::SimpleLispRuntime::EvalList(  lisp_runtime::CellAdaptor not_evaluated, lisp_runtime::CellAdaptor context) {
    DEBUG_OUTPUT(not_evaluated,atom_factory_);

    if(not_evaluated.IsEmpty()){
        return not_evaluated;
    } else{
        const auto &expression = not_evaluated.Car();
        auto new_head = Eval(expression, context);

        auto evaluated_tail = EvalList( not_evaluated.Cdr(), context);

        auto* new_node = cell_factory_->CreateListCell();
        new_node->head_ = new_head.GetHead();
        new_node->tail_ = evaluated_tail.GetHead();

        return {new_node };
    }
}

lisp_runtime::SExpr
lisp_runtime::SimpleLispRuntime::ApplyBuiltinFunction(lisp_runtime::functions::BuiltinFunction builtin_function, lisp_runtime::CellAdaptor parameters) {
    return builtin_function(parameters.GetThisCell());
}


