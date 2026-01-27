
#include <iostream>
#include "app.h"

namespace app{

void App::Run() {
    lisp_runtime_->Eval({},{});
}

void App::Init() {
    foundation_->Init();
    lisp_runtime_->Init();
}

void App::Shutdown() {
    lisp_runtime_->Shutdown();
    foundation_->Shutdown();
}

} // namespace app