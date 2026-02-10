#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <variant>

#include "cell_adaptor.h"
#include <functions/builtin_function.h>


namespace lisp_runtime::functions {

    class Functions {
    public:
        using FindFunctionResult = std::variant<lisp_runtime::CellAdaptor,foundation::Function*, std::nullopt_t>;

        virtual ~Functions() = default;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual FindFunctionResult FindFunction(const lisp_runtime::CellAdaptor& function_name) = 0;

        virtual void RegisterFunction(foundation::Function*) =0;
    };

}// lisp_runtime  ::functions

#endif //FUNCTIONS_H
