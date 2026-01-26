#ifndef MEMORY_MANAGEMENT_MOCK_H
#define MEMORY_MANAGEMENT_MOCK_H

#include "mem/simple_memory_management.h"
#include "alloc_operation_mock.h"

class MemoryManagementMock :  public foundation::mem::mgr::MemoryManagement{
public:
    MemoryManagementMock() {
    };
    ~MemoryManagementMock() override  = default;

    MOCK_METHOD(void, Init,() , (override));
    MOCK_METHOD(void, Shutdown,() , (override));
    MOCK_METHOD(std::optional<foundation::mem::Cell*>, Allocate, (), ( override ));
    MOCK_METHOD(void, ReturnCellToPool, (foundation::mem::Cell * node), (override));
    MOCK_METHOD(void, MarkCellsAsUsed, (const foundation::mem::Cell *cell),( override ));
};


#endif
