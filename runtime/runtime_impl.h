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
        RuntimeImpl(std::shared_ptr<lisp_runtime::LispRuntime> lisp_runtime,
                             std::shared_ptr<lisp_runtime::forms::Forms> forms,
                             std::shared_ptr<lisp_runtime::functions::Functions> functions,
                             std::unique_ptr<lisp_runtime::CellFactory>  cell_factory,
                             std::shared_ptr<foundation::Foundation> foundation);


        void Init() override;

        void Shutdown() override;

         [[nodiscard]]
         std::shared_ptr<lisp_runtime::LispRuntime> GetLispRuntime() const override {
            return lisp_runtime_;
        }

        [[nodiscard]]
        lisp_runtime::CellFactory* CellFactory() const override {
            return cell_factory_.get();
        }
    public:
        std::shared_ptr<lisp_runtime::LispRuntime> lisp_runtime_;
        std::shared_ptr<lisp_runtime::forms::Forms> forms_;
        std::shared_ptr<lisp_runtime::functions::Functions> functions_;
        std::unique_ptr<lisp_runtime::CellFactory>  cell_factory_;
        std::shared_ptr<foundation::Foundation> foundation_;
    };

} // lisp_runtime  

#endif //RUNTIME_IMPL_H
