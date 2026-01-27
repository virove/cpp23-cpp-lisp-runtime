#ifndef GARBAGE_COLLECTOR_ACCEPTOR_MOCK_H
#define GARBAGE_COLLECTOR_ACCEPTOR_MOCK_H

#include <gmock/gmock.h>

#include "mem/garbage_collector_acceptor.h"

class  GarbageCollectorAcceptorMock : public foundation::mem::mgr::GarbageCollectorAcceptor{
    public:
    MOCK_METHOD(void, AcceptGarbageCollectorVisitor,(foundation::mem::mgr::GarbageCollectorVisitor *visitor),(override));
};

#endif //GARBAGE_COLLECTOR_ACCEPTOR_MOCK_H
