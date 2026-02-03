#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/di.hpp>
#include "boost/di/extension/scopes/shared.hpp"

#include "app.h"
#include "mocks/foundation_mock.h"
#include "mocks/RuntimeMock.h"

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

namespace di = boost::di;

class LispRuntimeMock: public lisp_runtime::LispRuntime{
public:
    MOCK_METHOD(void, Init,(),(override));
    MOCK_METHOD(void, Shutdown,(),(override));
    MOCK_METHOD(lisp_runtime::SExpr, Eval, (lisp_runtime::SExpr expression, lisp_runtime::SExpr context), (override));
    MOCK_METHOD(lisp_runtime::SExpr, Apply, (lisp_runtime::SExpr function, lisp_runtime::SExpr arguments, lisp_runtime::SExpr context), (override));
};

TEST(AppTest, InitTest) {
     auto  lisp_runtime_mock = std::make_shared<LispRuntimeMock>();
    auto foundation_mock = new FoundationMock();
    auto runtime_mock =  new RuntimeMock();

    auto NIL = std::make_unique<foundation::mem::CellAtom>(2);
    auto T = std::make_unique<foundation::mem::CellAtom>(3);

    auto defines = std::make_shared<foundation::Defines>();
    defines->Init(NIL.get(), T.get());


    auto injector = di::make_injector(
            di::bind<foundation::Foundation>().to(
                    [foundation_mock](){ return std::shared_ptr<foundation::Foundation>(foundation_mock);}
            ),
                di::bind<lisp_runtime::Runtime>().to(
                        [runtime_mock](){ return std::shared_ptr<lisp_runtime::Runtime>(runtime_mock);}
                )
    );



    auto app = injector.create<std::shared_ptr<app::App>>();

        {
            InSequence s;

            EXPECT_CALL(*foundation_mock, Init());
            EXPECT_CALL(*runtime_mock, Init());
        }

    app->Init();


    EXPECT_CALL(*runtime_mock, GetLispRuntime())
            .WillOnce(Return(lisp_runtime_mock));

    EXPECT_CALL(*lisp_runtime_mock, Eval(_,_))
            .WillOnce(Return(NIL.get()));

    app->Run();

    {
        InSequence s;

        EXPECT_CALL(*runtime_mock, Shutdown());
        EXPECT_CALL(*foundation_mock, Shutdown());
    }

    app->Shutdown();
}