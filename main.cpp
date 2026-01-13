#include <boost/di.hpp>
#include <iostream>
#include "app/app.h"
#include "runtime/lisp_runtime.h"

#include "simple_lisp_runtime.h"
#include "simple_cell_factory.h"
#include "atom_factory_impl.h"


namespace di = boost::di;

class AllocOperationStub : public foundation::mem::mgr::AllocOperation{
public:
    ~AllocOperationStub() override= default;

private:
    void Init() override {}

    foundation::mem::Cell * Allocate() override {
        return nullptr;
    }
};

class MemoryManagementStub : public foundation::MemoryManagement{
public:
    ~MemoryManagementStub() override = default;

private:
    void Init() override {
    }

    foundation::mem::Cell *Allocate() override {
        return nullptr;
    }
};

int main(){
    std::unique_ptr<app::App> app;

        auto injector = di::make_injector(
                di::bind<lisp_runtime::LispRuntime>().to<lisp_runtime::SimpleLispRuntime>() ,
                di::bind<lisp_runtime::CellFactory>().to<lisp_runtime::SimpleCellFactory>(),
                di::bind<foundation::mem::mgr::AllocOperation>().to<AllocOperationStub>(),
                di::bind<foundation::AtomFactory>().in(di::singleton).to<foundation::AtomFactoryImpl>(),
                di::bind<foundation::MemoryManagement>().to<MemoryManagementStub>()
        );

    app = injector.create<std::unique_ptr<app::App>>();

    app->Init();
    app->Run();
    app->Shutdown();

    return 0;
}