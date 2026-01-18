#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/di.hpp>

#include "mem/simple_memory_management.h"
#include "atom_factory_impl.h"
#include "mocks/memory_management_mock.h"
#include "defines.h"
#include "mem/allocator_from_arena.h"
#include "mem/allocator_from_garbage_collected.h"

#include "mocks/alloc_operation_mock.h"
#include "mocks/garbage_collector_acceptor_mock.h"

#include "mem/simple_memory_management.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Throw;
using ::testing::InSequence;

namespace di = boost::di;


TEST(MemoryManagementTest, allocator_from_arena) {
    foundation::mem::mgr::AllocatorFromArena allocatorFromArena(3);
    allocatorFromArena.Init();

    const std::optional<foundation::mem::Cell *> &allocate1 = allocatorFromArena.Allocate();
    EXPECT_TRUE(allocate1.has_value());
    const std::optional<foundation::mem::Cell *> &allocate2 = allocatorFromArena.Allocate();
    EXPECT_TRUE(allocate2.has_value());
    const std::optional<foundation::mem::Cell *> &allocate3 = allocatorFromArena.Allocate();
    EXPECT_TRUE(allocate3.has_value());
    EXPECT_TRUE(!(allocate1.value() == allocate2.value() && allocate2.value() == allocate3.value()));

    EXPECT_THROW(allocatorFromArena.Allocate(), foundation::mem::mgr::CouldNotAllocateMemoryInArenaException);
}


TEST(MemoryManagementTest, allocator_from_garbage_collected1) {
    foundation::mem::mgr::AllocatorFromArena allocatorFromArena(3);
    foundation::mem::mgr::AllocatorFromGarbageCollected allocatorFromGarbageCollected;

    allocatorFromArena.Init();
    allocatorFromGarbageCollected.Init();

    auto node = allocatorFromArena.Allocate();
    auto *allocated_node1 = node.value();
    allocatorFromGarbageCollected.ReturnCellToPool(allocated_node1);
    auto optional = allocatorFromGarbageCollected.Allocate();
    auto *allocated_node2 = optional.value();
    EXPECT_TRUE(optional.has_value());
    EXPECT_TRUE(allocated_node1 == allocated_node2);

    EXPECT_THROW(allocatorFromGarbageCollected.Allocate(), foundation::mem::mgr::CouldNotAllocateMemoryException);
}


TEST(MemoryManagementTest, allocator_from_garbage_collected3) {
    foundation::mem::mgr::AllocatorFromArena allocatorFromArena(3);
    foundation::mem::mgr::AllocatorFromGarbageCollected allocatorFromGarbageCollected;

    allocatorFromArena.Init();
    allocatorFromGarbageCollected.Init();

    auto node = allocatorFromArena.Allocate();
    auto allocated_node1 = node.value();
    allocatorFromGarbageCollected.ReturnCellToPool(allocated_node1);

    node = allocatorFromArena.Allocate();
    auto  *allocated_node2 = node.value();
    allocatorFromGarbageCollected.ReturnCellToPool(allocated_node2);

    node = allocatorFromArena.Allocate();
    auto  *allocated_node3 = node.value();
    allocatorFromGarbageCollected.ReturnCellToPool(allocated_node3);

    const auto  optional = allocatorFromGarbageCollected.Allocate();
    auto  *allocated_node = optional.value();
    EXPECT_TRUE(optional.has_value());
    EXPECT_TRUE(allocated_node == allocated_node3);


    const auto  optional2 = allocatorFromGarbageCollected.Allocate();
    allocated_node = optional2.value();
    EXPECT_TRUE(optional.has_value());
    EXPECT_TRUE(allocated_node == allocated_node2);
}

/***
 *  Precondition:
 *       Current state: allocation from arena
 *  1. make first 3 allocations,
 *  2. 4th allocation fails
 *  Expected:
 *      3.Switch to allocation from garbage collected pool
 *      4. 4th Allocation from  garbage collected pool is succsessful
 */
TEST(MemoryManagementTest, allocation_from_farbage_collected_allocator) {
    std::unique_ptr<AllocatorFromArenaMock> allocator_from_arena_from_arena_ptr = std::make_unique<AllocatorFromArenaMock>();
    std::unique_ptr<AllocatorFromGarbageCollectedMock> allocatorFromGarbageCollectedMock_ptr = std::make_unique<AllocatorFromGarbageCollectedMock>();


    AllocatorFromArenaMock* allocator_from_arena_from_arena =  allocator_from_arena_from_arena_ptr.get();
    AllocatorFromGarbageCollectedMock* allocatorFromGarbageCollectedMock =  allocatorFromGarbageCollectedMock_ptr.get();

    EXPECT_CALL(*allocator_from_arena_from_arena, Init()).Times(1);
    EXPECT_CALL(*allocatorFromGarbageCollectedMock, Init()).Times(1);

    auto test_data_node1 = std::make_unique<foundation::mem::Cell>();
    auto test_data_node2 = std::make_unique<foundation::mem::Cell>();
    auto test_data_node3 = std::make_unique<foundation::mem::Cell>();
    auto test_data_node4 = std::make_unique<foundation::mem::Cell>();


    EXPECT_CALL(*allocator_from_arena_from_arena, Allocate())
            .WillOnce(Return(test_data_node1.get() ))
            .WillOnce(Return(test_data_node2.get() ))
            .WillOnce(Return(test_data_node3.get() ))
            .WillOnce(Throw(foundation::mem::mgr::CouldNotAllocateMemoryInArenaException()));

    EXPECT_CALL(*allocatorFromGarbageCollectedMock, Allocate())
            .Times(1)
            .WillOnce(Return(test_data_node4.get() ));


    foundation::mem::mgr::SimpleMemoryManagement memoryManagement(std::move(allocator_from_arena_from_arena_ptr), std::move(allocatorFromGarbageCollectedMock_ptr));
    memoryManagement.Init();
    auto node1 = memoryManagement.Allocate();
    auto node2 = memoryManagement.Allocate();
    auto node3 = memoryManagement.Allocate();
    auto node4 = memoryManagement.Allocate();
    EXPECT_TRUE(node4.has_value());
    EXPECT_EQ(node4.value(), test_data_node4.get());

}


/***
 *  Precondition:
 *       Current state: allocation from arena
 *
 *  1. make first 3 allocations,
 *  2. 4th allocation fails
 *  3.Switched to allocate from garbage collected pool
 *  4. Allocation from  garbage collected pool failed
 *  Expected:
 *      5. Run Garbage collection
 *      5.1         Mark all cells as free;
 *      5.2         Traverse cells used by interpretor and by calculations and mark the as busy;
 *      5.3         Collect cells marked as free and add the them to pool of garbage collected cells
 *      6. Allocation from  garbage collected pool is succsessful
 */
TEST(MemoryManagementTest, garbage_collection) {
    std::unique_ptr<AllocatorFromArenaMock> allocator_from_arena_from_arena_ptr = std::make_unique<AllocatorFromArenaMock>();
    std::unique_ptr<AllocatorFromGarbageCollectedMock> allocatorFromGarbageCollectedMock_ptr = std::make_unique<AllocatorFromGarbageCollectedMock>();

    AllocatorFromArenaMock* allocator_from_arena_from_arena = allocator_from_arena_from_arena_ptr.get();
    AllocatorFromGarbageCollectedMock* allocatorFromGarbageCollectedMock =  allocatorFromGarbageCollectedMock_ptr.get();

    GarbageCollectorAcceptorMock garbageCollectorAcceptorMock;

    EXPECT_CALL(*allocator_from_arena_from_arena, Init()).Times(1);
    EXPECT_CALL(*allocatorFromGarbageCollectedMock, Init()).Times(1);

    auto test_data_node1 = std::make_unique<foundation::mem::Cell>();
    auto test_data_node2 = std::make_unique<foundation::mem::Cell>();
    auto test_data_node3 = std::make_unique<foundation::mem::Cell>();
    auto test_data_node4 = std::make_unique<foundation::mem::Cell>();
    {
        InSequence s;


        EXPECT_CALL(*allocator_from_arena_from_arena, Allocate())
                .WillOnce(Return(test_data_node1.get() ))
                .WillOnce(Return(test_data_node2.get() ))
                .WillOnce(Return(test_data_node3.get() ))
                .WillOnce(Throw(foundation::mem::mgr::CouldNotAllocateMemoryInArenaException()));


        EXPECT_CALL(*allocatorFromGarbageCollectedMock, Allocate())
                .WillOnce(Throw(foundation::mem::mgr::CouldNotAllocateMemoryException()))
                .RetiresOnSaturation();

        EXPECT_CALL(*allocator_from_arena_from_arena,MarkAllAsFree()).Times(1);

        EXPECT_CALL(garbageCollectorAcceptorMock, AcceptGarbageCollectorVisitor(_))
                .Times(1);

        EXPECT_CALL(*allocator_from_arena_from_arena,CollectAllFreeNodes(_)).Times(1);

        EXPECT_CALL(*allocatorFromGarbageCollectedMock, Allocate())
                .Times(1)
                .WillOnce(Return(test_data_node4.get() ));
    }

    foundation::mem::mgr::SimpleMemoryManagement memoryManagement(std::move(allocator_from_arena_from_arena_ptr), std::move(allocatorFromGarbageCollectedMock_ptr));
    memoryManagement.RegisterGarbageCollectorAcceptor(&garbageCollectorAcceptorMock);
    memoryManagement.Init();
    auto node1 = memoryManagement.Allocate();
    auto node2 = memoryManagement.Allocate();
    auto node3 = memoryManagement.Allocate();
    auto node4 = memoryManagement.Allocate();
    EXPECT_TRUE(node4.has_value());
    EXPECT_EQ(node4.value(), test_data_node4.get());
}

TEST(MemoryManagementTest, allocator_from_arena_mark_all_free) {
    foundation::mem::mgr::AllocatorFromArena allocatorFromArena(5);
    allocatorFromArena.Init();

    auto node1 = allocatorFromArena.Allocate();
    EXPECT_TRUE(node1.has_value());
    EXPECT_TRUE(node1.value()->IsBusy());
    auto node2 = allocatorFromArena.Allocate();
    EXPECT_TRUE(node2.has_value());
    EXPECT_TRUE(node2.value()->IsBusy());
    auto node3 = allocatorFromArena.Allocate();
    EXPECT_TRUE(node3.has_value());
    EXPECT_TRUE(node3.value()->IsBusy());
    auto node4 = allocatorFromArena.Allocate();
    EXPECT_TRUE(node4.has_value());
    EXPECT_TRUE(node4.value()->IsBusy());
    auto node5 = allocatorFromArena.Allocate();
    EXPECT_TRUE(node5.has_value());
    EXPECT_TRUE(node5.value()->IsBusy());

    allocatorFromArena.MarkAllAsFree();

    EXPECT_FALSE(node1.value()->IsBusy());
    EXPECT_FALSE(node2.value()->IsBusy());
    EXPECT_FALSE(node3.value()->IsBusy());
    EXPECT_FALSE(node4.value()->IsBusy());
    EXPECT_FALSE(node5.value()->IsBusy());

}


/***
 *  Precondition:
 *       Current state: allocation from arena, number of available cell is 4
 *
 *  1. make first 3 allocations,
 *  2. 4th allocation fails
 *  3.Switched to allocate from garbage collected pool
 *  4. Allocation from  garbage collected pool failed
 *  5. Run garbage collection
 *  5.1 All cells are busy
 *  Expected:
 *      6. 4th allocation from  garbage collected pool fails
 */
TEST(MemoryManagementIntegrationTest, AllocatorFromArena_garbage_collection_no_free_memory) {
    auto allocator_from_arena_from_arena = std::make_unique<foundation::mem::mgr::AllocatorFromArena>(4);
    auto allocator_from_garbage_collected =  std::make_unique<foundation::mem::mgr::AllocatorFromGarbageCollected>();
    GarbageCollectorAcceptorMock garbageCollectorAcceptorMock;

    foundation::mem::mgr::SimpleMemoryManagement management(std::move(allocator_from_arena_from_arena), std::move(allocator_from_garbage_collected));
    management.Init();
    management.RegisterGarbageCollectorAcceptor(&garbageCollectorAcceptorMock);
    auto node1 = management.Allocate();
    auto node2 = management.Allocate();
    auto node3 = management.Allocate();
    auto node4 = management.Allocate();

    EXPECT_CALL(garbageCollectorAcceptorMock, AcceptGarbageCollectorVisitor(_))
            .WillOnce(testing::WithArg<0> (testing::Invoke([&node1,&node2,&node3,node4](foundation::mem::mgr::GarbageCollectorVisitor *visitor){
                visitor->MarkCellsAsUsed(node1.value());
                visitor->MarkCellsAsUsed(node2.value());
                visitor->MarkCellsAsUsed(node3.value());
                visitor->MarkCellsAsUsed(node4.value());
                      }))
            );

    auto node5 = management.Allocate();
    EXPECT_FALSE(node5.has_value());
}


/***
 *  Precondition:
 *       Current state: allocation from arena, number of available cell is 4
 *
 *  1. make first 4 allocations,
 *  2. 4th allocation fails
 *  3.Switched to allocate from garbage collected pool
 *  4. Allocation from  garbage collected pool failed
 *  5. Run garbage collection
 *  5.1 node1 and node2 are marked as busy
    5.2 node3 and node4 are added to garbaged collected pool
 *  Expected:
 *      6. 5th and 6th allocation from  garbage collected pool succeeds
 */
TEST(MemoryManagementIntegrationTest, garbage_collected_cells_allocation_succsess) {
    auto allocator_from_arena_from_arena = std::make_unique<foundation::mem::mgr::AllocatorFromArena>(4);
    auto allocator_from_garbage_collected =  std::make_unique<foundation::mem::mgr::AllocatorFromGarbageCollected>();
    GarbageCollectorAcceptorMock garbageCollectorAcceptorMock;

    foundation::mem::mgr::SimpleMemoryManagement management(std::move(allocator_from_arena_from_arena), std::move(allocator_from_garbage_collected));
    management.Init();
    management.RegisterGarbageCollectorAcceptor(&garbageCollectorAcceptorMock);
    auto node1 = management.Allocate();
    auto node2 = management.Allocate();
    auto node3 = management.Allocate();
    auto node4 = management.Allocate();

    EXPECT_CALL(garbageCollectorAcceptorMock, AcceptGarbageCollectorVisitor(_))
            .WillOnce(testing::WithArg<0> (testing::Invoke([&node1,&node2,&node3,node4](foundation::mem::mgr::GarbageCollectorVisitor *visitor){
                          visitor->MarkCellsAsUsed(node1.value());
                          visitor->MarkCellsAsUsed(node2.value());
                      }))
            );

    auto node5 = management.Allocate();
    EXPECT_TRUE(node5.has_value());

    auto node6 = management.Allocate();
    EXPECT_TRUE(node6.has_value());

    EXPECT_TRUE(node5.value()->head_ == node3.value()->head_ || node5.value()->head_ == node4.value()->head_);
}