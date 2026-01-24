#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/di.hpp>

#include "app.h"
#include "atom_factory_impl.h"
#include "mem/simple_memory_management.h"
#include "mocks/memory_management_mock.h"

using ::testing::_;
namespace di = boost::di;

class LispRuntimeMock: public lisp_runtime::LispRuntime{
public:
    MOCK_METHOD(void, Init,(),(override));
    MOCK_METHOD(void, Shutdown,(),(override));
    MOCK_METHOD(lisp_runtime::SExpr, Eval, (lisp_runtime::SExpr expression, lisp_runtime::SExpr context), (override));
    MOCK_METHOD(lisp_runtime::SExpr, Apply, (lisp_runtime::SExpr function, lisp_runtime::SExpr arguments, lisp_runtime::SExpr context), (override));
};

TEST(AppTest, InitTest) {

    auto lisp_runtime_mock = new  LispRuntimeMock();
    auto injector = di::make_injector(
            di::bind<lisp_runtime::LispRuntime>().to<>([lisp_runtime_mock]() {
                return std::unique_ptr<lisp_runtime::LispRuntime> {lisp_runtime_mock};
            }),
            di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>(),
            di::bind<foundation::mem::mgr::MemoryManagement>().to<MemoryManagementMock>()
    );

    auto app = injector.create<std::unique_ptr<app::App>>();
    EXPECT_CALL(*lisp_runtime_mock, Init());
    app->Init();

    EXPECT_CALL(*lisp_runtime_mock, Eval(_,_));
    app->Run();

    EXPECT_CALL(*lisp_runtime_mock, Shutdown());
    app->Shutdown();
}