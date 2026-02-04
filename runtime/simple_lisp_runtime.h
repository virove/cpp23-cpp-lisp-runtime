#ifndef SIMPLE_LISP_RUNTIME_H
#define SIMPLE_LISP_RUNTIME_H

#include <memory>
#include <variant>

#include "lisp_runtime.h"
#include "cell_factory.h"
#include "atom_factory.h"
#include "functions/builtin_function.h"
#include "forms/forms.h"
#include "functions/functions.h"
#include "variables/variables.h"

namespace lisp_runtime{

    class SimpleLispRuntime : public LispRuntime {
    public:
        explicit SimpleLispRuntime(std::unique_ptr<lisp_runtime::CellFactory> cell_factory,
                                   std::shared_ptr< foundation::AtomFactory> atom_factory,
                                    std::shared_ptr<lisp_runtime::forms::Forms> forms,
                                   std::shared_ptr<lisp_runtime::functions::Functions> functions,
                                   std::shared_ptr<lisp_runtime::vars::Variables> variables)
            : cell_factory_{std::move(cell_factory)},
            atom_factory_{std::move(atom_factory)},
              forms_{std::move(forms)},
              functions_{std::move(functions)},
              variables_{variables}{
        }

        ~SimpleLispRuntime() override = default;

        void Init() override;

        void Shutdown() override;

        SExpr Eval(SExpr expression, SExpr context) override;

        SExpr Apply(SExpr function, SExpr arguments, SExpr context) override;

    private:
        lisp_runtime::CellAdaptor  EvalList(lisp_runtime::CellAdaptor parameters, lisp_runtime::SExpr context);


        lisp_runtime::SExpr EvalFunction(functions::Functions::FindFunctionResult find_function_result, lisp_runtime::CellAdaptor function_name, CellAdaptor parameters, SExpr adaptor);

        lisp_runtime::SExpr ApplyBuiltinFunction(foundation::Function* builtin_function, lisp_runtime::CellAdaptor parameters);

        std::unique_ptr<lisp_runtime::CellFactory> cell_factory_;
        std::shared_ptr< foundation::AtomFactory> atom_factory_;
        std::shared_ptr<lisp_runtime::forms::Forms> forms_;
        std::shared_ptr<lisp_runtime::functions::Functions> functions_;
        std::shared_ptr<lisp_runtime::vars::Variables> variables_;
    };

} // namespace foundation

#endif //SIMPLE_LISP_RUNTIME_H
