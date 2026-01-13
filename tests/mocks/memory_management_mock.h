#ifndef MEMORY_MANAGEMENT_MOCK_H
#define MEMORY_MANAGEMENT_MOCK_H

#include "memory_management.h"

class MemoryManagementMock :  public foundation::MemoryManagement{
public:
    ~MemoryManagementMock() override  = default;

    MOCK_METHOD(void, Init,() , (override));
    MOCK_METHOD(foundation::mem::Cell *, Allocate, (), ( override ));
};


#endif
