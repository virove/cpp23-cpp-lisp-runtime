#ifndef SIMPLE_EVAL_TEST_CONTEXT_H
#define SIMPLE_EVAL_TEST_CONTEXT_H


#include "base_test_context.h"
#include "runtime.h"

template <auto InjectorFactoryFunction>
class SimpleEvalTestContext : public test_support::BaseTestContext<InjectorFactoryFunction>{
public:
    explicit SimpleEvalTestContext() {
        runtime_ = this->injector.template create<std::shared_ptr<lisp_runtime::Runtime>>();
        runtime_->Init();
    }

    ~SimpleEvalTestContext() {
        runtime_->Shutdown();
    }

    std::shared_ptr<lisp_runtime::LispRuntime> GetSimpleLispRuntime(){
        return runtime_->GetLispRuntime();
    }

    std::shared_ptr<lisp_runtime::Runtime> runtime_;
};


#endif //SIMPLE_EVAL_TEST_CONTEXT_H
