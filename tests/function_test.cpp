#include <gmock/gmock-matchers.h>

#include "boost/di.hpp"
#include "boost/di/extension/scopes/shared.hpp"

#include "mem/allocator_from_arena.h"
#include "lisp_runtime.h"
#include "simple_cell_factory.h"
#include "simple_lisp_runtime.h"
#include "atom_factory_impl.h"
#include "foundation_impl.h"
#include "base_test_context.h"
#include "runtime_impl.h"
#include "forms/forms_impl.h"
#include "functions/functions_impl.h"
#include "simple_eval_test_context.h"
#include "utilities/utilities.h"
#include "variables/variables_impl.h"

namespace di = boost::di;

using ::testing::_;

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
                di::bind<foundation::mem::mgr::MemoryManagement>().to<foundation::mem::mgr::SimpleMemoryManagement>().in(di::extension::shared),
                di::bind<lisp_runtime::Runtime>().in(di::extension::shared).to<lisp_runtime::RuntimeImpl>(),
                di::bind < foundation::Defines >.in(di::extension::shared),
                di::bind<lisp_runtime::forms::Forms>().to<lisp_runtime::forms::FormsImpl>().in(di::extension::shared),
                di::bind<lisp_runtime::functions::Functions>().to<lisp_runtime::functions::FunctionsImpl>().in(di::extension::shared),
                di::bind<lisp_runtime::vars::Variables>().to<lisp_runtime::vars::VariablesImpl>().in(di::extension::shared)
        );
    }
}

using EvalTestContext = SimpleEvalTestContext<make_test_injector>;

TEST(FunctionTest, eval_lambda) {

    EvalTestContext simple_eval_test_context;

    auto lisp_runtime = simple_eval_test_context.GetSimpleLispRuntime();

    auto parser = simple_eval_test_context.CreateParser();
    {
        auto *lambda = "((lambda (x y) (+ x y)) 2 3)";

        lisp_runtime::Stream<test_support::Std_StringStreamType> stream1(lambda);

        auto expression =parser->Parse(&stream1);

        auto result = simple_eval_test_context.GetSimpleLispRuntime()->Eval(expression,{foundation::Defines::NIL()});

        EXPECT_TRUE(result.GetType() == foundation::mem::Cell::Type::NumberType);
        EXPECT_EQ(result.GetThisCell()->number_ ,5);
    }
}

TEST(FormTest, function_test) {
    EvalTestContext simple_eval_test_context;

    auto lisp_runtime = simple_eval_test_context.GetSimpleLispRuntime();

    auto parser = simple_eval_test_context.CreateParser();
    {
        auto *function_definition = "(defun factorial (n)\n"
                                    "  (if (= n 0)\n"
                                    "      1\n"
                                    "      (* n (factorial (- n 1))) ) )";

        lisp_runtime::Stream<test_support::Std_StringStreamType> stream1(function_definition);

        auto expression =parser->Parse(&stream1);

        auto result = simple_eval_test_context.GetSimpleLispRuntime()->Eval(expression,{foundation::Defines::NIL()});

        EXPECT_EQ(result.GetThisCell(), simple_eval_test_context.CellFactory()->GetOrCreate("factorial"));


    }

    {
        auto *function_definition = "(factorial 5)";

        lisp_runtime::Stream<test_support::Std_StringStreamType> stream1(function_definition);

        auto expression =parser->Parse(&stream1);

        auto result = simple_eval_test_context.GetSimpleLispRuntime()->Eval(expression,{foundation::Defines::NIL()});

        EXPECT_EQ(result.GetType() , foundation::mem::Cell::Type::NumberType  );
        EXPECT_EQ(result.GetThisCell()->number_ , 120 );


    }

}



