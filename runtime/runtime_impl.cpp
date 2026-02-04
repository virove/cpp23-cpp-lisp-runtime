#include "runtime_impl.h"
#include "forms/quote_form.h"
#include "forms/defun_form.h"
#include "forms/if_form.h"

namespace lisp_runtime {

     RuntimeImpl::RuntimeImpl(std::shared_ptr<lisp_runtime::LispRuntime> lisp_runtime,
                                std::shared_ptr<lisp_runtime::forms::Forms> forms,
                                std::shared_ptr<lisp_runtime::functions::Functions> functions,
                                std::unique_ptr<lisp_runtime::CellFactory>  cell_factory,
                                std::shared_ptr<foundation::Foundation> foundation)
                                : lisp_runtime_{ std::move(lisp_runtime) },
                                forms_{std::move(forms)},
                                functions_{std::move(functions)},
                                cell_factory_{std::move(cell_factory)},
                                foundation_{std::move(foundation)}
     {

     }


    void RuntimeImpl::Init() {
        lisp_runtime_->Init();


        {
            forms_->Register(new lisp_runtime::forms::QuoteForm(foundation_,cell_factory_.get()));
            forms_->Register(new lisp_runtime::forms::DefunForm(foundation_,cell_factory_.get()));
            forms_->Register(new lisp_runtime::forms::If_Form(foundation_,cell_factory_.get()));

            functions_->RegisterFunction(new lisp_runtime::functions::BuiltinPlus(this));
            functions_->RegisterFunction(new lisp_runtime::functions::BuiltinMinus(this));
            functions_->RegisterFunction(new lisp_runtime::functions::BuiltinMultiplication(this));
            functions_->RegisterFunction(new lisp_runtime::functions::BuiltinDivision(this));
            functions_->RegisterFunction(new lisp_runtime::functions::BuiltinComparisonEqual());
        }
        functions_->Init();
        forms_->Init();
    }

    void RuntimeImpl::Shutdown() {
        forms_->Shutdown();
        functions_->Shutdown();
        lisp_runtime_->Shutdown();
    }



} // lisp_runtime