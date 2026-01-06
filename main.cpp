#include <boost/di.hpp>
#include "app/app.h"
#include "runtime/lisp_runtime.h"

namespace di = boost::di;

class LispRuntimeImpl : public lisp_runtime::LispRuntime{
public:
    ~LispRuntimeImpl() override = default;

    SExpr Eval(SExpr expression, SExpr context) override {
        return {};
    }

    SExpr Apply(SExpr function, SExpr arguments, SExpr context) override {
        return {};
    }

    void Init() override {
    }

    void Shutdown() override {
    }
};

int main(){
    auto injector = di::make_injector(
            di::bind<lisp_runtime::LispRuntime>().to<LispRuntimeImpl>() );

    auto app = injector.create<std::unique_ptr<app::App>>();
    app->Init();
    app->Run();
    app->Shutdown();

    return 0;
}