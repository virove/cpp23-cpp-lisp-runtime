#include <boost/di.hpp>
#include <iostream>
#include "app/app.h"
#include "runtime/lisp_runtime.h"

#include "simple_lisp_runtime.h"
#include "simple_cell_factory.h"
#include "atom_factory_impl.h"
#include "mem/simple_memory_management.h"


static const int kPreallocatedMemory_CellNumber = 1024*300;
namespace di = boost::di;

class AllocOperationStub : public foundation::mem::mgr::AllocOperation{
public:
    ~AllocOperationStub() override= default;

private:
    void Init() override {}

    std::optional<foundation::mem::Cell*>  Allocate() override {
        return nullptr;
    }
};

class MemoryManagementStub : public foundation::mem::mgr::SimpleMemoryManagement{
public:
    ~MemoryManagementStub() override = default;

private:
    void Init() override {
    }

    std::optional<foundation::mem::Cell*>  Allocate() override {
        return nullptr;
    }
};

int main(){
    std::unique_ptr<app::App> app;

        auto injector = di::make_injector(
                di::bind<std::size_t>().named(foundation::mem::mgr::number_of_preallocated_nodes).to(static_cast<std::size_t>(kPreallocatedMemory_CellNumber)),
                di::bind<lisp_runtime::LispRuntime>().in(di::singleton).to<lisp_runtime::SimpleLispRuntime>() ,
                di::bind<lisp_runtime::CellFactory>().to<lisp_runtime::SimpleCellFactory>(),
                di::bind<foundation::AtomFactory>().in(di::singleton).to<foundation::AtomFactoryImpl>(),
                di::bind<foundation::mem::mgr::AllocatorFromArena>,
                di::bind<foundation::mem::mgr::MemoryManagement>().in(di::singleton).to<foundation::mem::mgr::SimpleMemoryManagement>()
        );



    app = injector.create<std::unique_ptr<app::App>>();

    app->Init();
    app->Run();
    app->Shutdown();

    return 0;
}