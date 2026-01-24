#ifndef ALLOC_OPERATION_MOCK_H
#define ALLOC_OPERATION_MOCK_H

#include <gmock/gmock.h>

#include "mem/simple_memory_management.h"
#include "mem/allocator_from_arena.h"
#include "mem/allocator_from_garbage_collected.h"

class AllocOperationMock:   public foundation::mem::mgr::AllocOperation{
public:
    AllocOperationMock()= default;
    ~AllocOperationMock() override = default;

    MOCK_METHOD(void, Init,(),(override));
    MOCK_METHOD( std::optional<foundation::mem::Cell*>, Allocate,() ,(override));
};

class AllocatorFromArenaMock:   public foundation::mem::mgr::AllocatorFromArena{
public:
    AllocatorFromArenaMock() :  AllocatorFromArena(1){  };
    ~AllocatorFromArenaMock() override = default;

    MOCK_METHOD(void, Init,(),(override));
    MOCK_METHOD( std::optional<foundation::mem::Cell*>, Allocate,() ,(override));
    MOCK_METHOD(void, MarkAllAsFree,(), (override));
    MOCK_METHOD(void, CollectAllFreeNodes,(foundation::mem::mgr::FreeNode* gc_allocator), (const, override));
};

class AllocatorFromGarbageCollectedMock : public foundation::mem::mgr::AllocatorFromGarbageCollected{
public:
    MOCK_METHOD(void, Init,(),(override));
    MOCK_METHOD( std::optional<foundation::mem::Cell*>, Allocate,() ,(override));
    MOCK_METHOD(void, ReturnCellToPool, (foundation::mem::Cell * node),   (override));
};

#endif //ALLOC_OPERATION_MOCK_H
