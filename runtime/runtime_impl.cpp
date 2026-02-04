#include "runtime_impl.h"
#include "forms/quote_form.h"

namespace lisp_runtime {
    void RuntimeImpl::Init() {

        functions_->Init();
        lisp_runtime_->Init();

        forms_->Register(new lisp_runtime::forms::QuoteForm());
        functions_->RegisterBuiltinFunction("atom", lisp_runtime::functions::BuiltinAtomPredicate );
    }

    void RuntimeImpl::Shutdown() {
        lisp_runtime_->Shutdown();
        functions_->Shutdown();
    }

} // lisp_runtime