#include <boost/di.hpp>
#include <iostream>
#include "app/app.h"
#include "runtime/lisp_runtime.h"

#include "simple_lisp_runtime.h"
#include "simple_cell_factory.h"
#include "atom_factory_impl.h"
#include "mem/simple_memory_management.h"
#include "runtime_impl.h"
#include "foundation_impl.h"
#include "forms/forms_impl.h"
#include "functions/functions_impl.h"
#include "variables/variables_impl.h"


static const int kPreallocatedMemory_CellNumber = 1024*300;
namespace di = boost::di;

int main(){
    std::unique_ptr<app::App> app;

        auto injector = di::make_injector(
                di::bind<std::size_t>().named(foundation::mem::mgr::number_of_preallocated_nodes).to(static_cast<std::size_t>(kPreallocatedMemory_CellNumber)),
                di::bind<lisp_runtime::LispRuntime>().in(di::singleton).to<lisp_runtime::SimpleLispRuntime>() ,
                di::bind<lisp_runtime::CellFactory>().to<lisp_runtime::SimpleCellFactory>(),
                di::bind<foundation::AtomFactory>().in(di::singleton).to<foundation::AtomFactoryImpl>(),
                di::bind<foundation::mem::mgr::AllocatorFromArena>,
                di::bind<foundation::Foundation>().in(di::singleton).to<foundation::FoundationImpl>(),
                di::bind<lisp_runtime::Runtime>().in(di::singleton).to<lisp_runtime::RuntimeImpl>(),
                di::bind<foundation::mem::mgr::MemoryManagement>().in(di::singleton).to<foundation::mem::mgr::SimpleMemoryManagement>(),
                di::bind<lisp_runtime::forms::Forms>().to<lisp_runtime::forms::FormsImpl>().in(di::singleton),
                di::bind<lisp_runtime::functions::Functions>().to<lisp_runtime::functions::FunctionsImpl>().in(di::singleton),
                di::bind<lisp_runtime::vars::Variables>().to<lisp_runtime::vars::VariablesImpl>().in(di::singleton)
        );

    app = injector.create<std::unique_ptr<app::App>>();

    app->Init();
    app->Run();
    app->Shutdown();

    return 0;
}