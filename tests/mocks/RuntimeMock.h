#ifndef RUNTIMEMOCK_H
#define RUNTIMEMOCK_H

#include <gmock/gmock.h>

#include "runtime.h"

class RuntimeMock : public lisp_runtime::Runtime{
public:
    MOCK_METHOD( void, Init,() ,(override));

    MOCK_METHOD( void, Shutdown,() ,(override));

    MOCK_METHOD( std::shared_ptr<lisp_runtime::LispRuntime>, GetLispRuntime,() ,(const override));

    MOCK_METHOD(lisp_runtime::CellFactory *,CellFactory,(), (const override));
};

#endif //RUNTIMEMOCK_H
