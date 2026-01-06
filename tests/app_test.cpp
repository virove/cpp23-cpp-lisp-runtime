
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/di.hpp>

#include "app.h"

using ::testing::_;
namespace di = boost::di;

class LispRuntimeMock: public lisp_runtime::LispRuntime{
public:
    MOCK_METHOD(void, Init,(),(override));
    MOCK_METHOD(void, Shutdown,(),(override));
    MOCK_METHOD(SExpr, Eval,(SExpr expression, SExpr context),(override));
    MOCK_METHOD(SExpr, Apply,(SExpr function, SExpr arguments, SExpr context),(override));
};

TEST(AppTest, InitTest) {
    LispRuntimeMock lisp_runtime_mock;

    auto injector = di::make_injector(
            di::bind<lisp_runtime::LispRuntime>().to(&lisp_runtime_mock) // bind інтерфейсу до конкретного екземпляра
    );

    auto app = injector.create<std::unique_ptr<app::App>>();
    EXPECT_CALL(lisp_runtime_mock, Init());
    app->Init();

    EXPECT_CALL(lisp_runtime_mock, Eval(_,_));
    app->Run();

    EXPECT_CALL(lisp_runtime_mock, Shutdown());
    app->Shutdown();
}