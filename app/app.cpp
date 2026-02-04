#include "app.h"

namespace app{

void App::Run() {
    runtime_->GetLispRuntime()->Eval({   foundation::Defines::NIL()},{ foundation::Defines::NIL()} );
}

void App::Init() {
    foundation_->Init();
    runtime_->Init();
}

void App::Shutdown() {
    runtime_->Shutdown();
    foundation_->Shutdown();
}

} // namespace app