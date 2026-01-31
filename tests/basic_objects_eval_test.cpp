#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "base_test_context.h"
#include "boost/di/extension/scopes/shared.hpp"
#include "boost/di.hpp"
#include "mem/allocator_from_arena.h"
#include "lisp_runtime.h"
#include "simple_cell_factory.h"
#include "atom_factory_impl.h"
#include "foundation_impl.h"
#include "simple_lisp_runtime.h"

#include <memory>
#include <boost/di.hpp>

namespace di = boost::di;

constexpr std::size_t kPreallocatedMemory_CellNumber = 1000;

namespace {
    auto make_test_injector() {
        return di::make_injector<di::extension::shared_config>(
                di::bind<std::size_t>().named(foundation::mem::mgr::number_of_preallocated_nodes).to(
                        static_cast<std::size_t>(kPreallocatedMemory_CellNumber)),
                di::bind<lisp_runtime::LispRuntime>().to<lisp_runtime::SimpleLispRuntime>().in(di::extension::shared),
                di::bind<lisp_runtime::CellFactory>().to<lisp_runtime::SimpleCellFactory>(),
                di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>().in(di::extension::shared),
                di::bind < foundation::mem::mgr::AllocatorFromArena > ,
                di::bind<foundation::Foundation>().to<foundation::FoundationImpl>().in(di::extension::shared),
                di::bind<foundation::mem::mgr::MemoryManagement>().to<foundation::mem::mgr::SimpleMemoryManagement>(),
                di::bind < foundation::Defines >.in(di::extension::shared)

        );
    }
}

class SimpleEvalTestContext : public test_support::BaseTestContext<make_test_injector>{
public:
    explicit SimpleEvalTestContext() {
        lisp_runtime_ = injector.template create<std::shared_ptr<lisp_runtime::LispRuntime>>();
    }

    std::shared_ptr<lisp_runtime::LispRuntime> GetSimpleLispRuntime(){
        return lisp_runtime_;
    }

    std::shared_ptr<lisp_runtime::LispRuntime> lisp_runtime_;
};


TEST(BasicObjectEvalTest, EvalNumber) {
    SimpleEvalTestContext simple_eval_test_context;


    auto lisp_runtime = simple_eval_test_context.GetSimpleLispRuntime();
    auto cell_factory = simple_eval_test_context.CellFactory();

    auto argument = cell_factory->CreateNumber(123);
    auto result = lisp_runtime->Eval({argument},  {foundation::Defines::NIL() } );


    const auto& expected = cell_factory->CreateNumber(123);
    EXPECT_TRUE(result.GetHead()->GetType() == foundation::mem::Cell::Type::NumberType);
    EXPECT_EQ(result.GetHead()->number_ , expected->number_);

}