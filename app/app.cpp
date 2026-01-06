
#include <iostream>
#include "app.h"

namespace app{

void App::Run() {
    lisp_runtime_->Eval({},{});
}

void App::Init() {
    lisp_runtime_->Init();
}

void App::Shutdown() {
    lisp_runtime_->Shutdown();
}

} // namespace app