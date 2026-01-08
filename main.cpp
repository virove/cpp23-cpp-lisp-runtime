#include <boost/di.hpp>
#include "app/app.h"
#include "runtime/lisp_runtime.h"

#include "simple_lisp_runtime.h"
#include "simple_cell_factory.h"

namespace di = boost::di;

class AllocOperationStub : public lisp_runtime::mem::mgr::AllocOperation{
public:
    ~AllocOperationStub() override= default;

private:
    void Init() override {}

    lisp_runtime::mem::Cell * Allocate() override {
        return nullptr;
    }
};

int main(){
    std::unique_ptr<app::App> app;

        auto injector = di::make_injector(
                di::bind<lisp_runtime::LispRuntime>().to<lisp_runtime::SimpleLispRuntime>() ,
                di::bind<lisp_runtime::CellFactory>().to<lisp_runtime::SimpleCellFactory>(),
                di::bind<lisp_runtime::mem::mgr::AllocOperation>().to<AllocOperationStub>()
        );

    app = injector.create<std::unique_ptr<app::App>>();

    app->Init();
    app->Run();
    app->Shutdown();

    return 0;
}