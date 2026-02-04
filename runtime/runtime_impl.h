#ifndef RUNTIME_IMPL_H
#define RUNTIME_IMPL_H

#include <memory>

#include "runtime.h"
#include "lisp_runtime.h"
#include "forms/forms.h"
#include "functions/functions.h"

namespace lisp_runtime {

    class RuntimeImpl : public Runtime{
    public:
        explicit RuntimeImpl(std::shared_ptr<lisp_runtime::LispRuntime> lisp_runtime,
                             std::shared_ptr<lisp_runtime::forms::Forms> forms,
                             std::shared_ptr<lisp_runtime::functions::Functions> functions)
                             : lisp_runtime_{ std::move(lisp_runtime) },
                               forms_{std::move(forms)},
                               functions_{std::move(functions)}
        {}

        void Init() override;

        void Shutdown() override;

         std::shared_ptr<lisp_runtime::LispRuntime> GetLispRuntime() const override {
            return lisp_runtime_;
        }

    public:
        std::shared_ptr<lisp_runtime::LispRuntime> lisp_runtime_;
        std::shared_ptr<lisp_runtime::forms::Forms> forms_;
        std::shared_ptr<lisp_runtime::functions::Functions> functions_;
    };

} // lisp_runtime  

#endif //RUNTIME_IMPL_H
