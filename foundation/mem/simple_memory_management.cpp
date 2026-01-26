

#include "simple_memory_management.h"


namespace foundation {
    namespace mem {

        class Cell;

        namespace mgr {


            std::optional<foundation::mem::Cell *> SimpleMemoryManagement::Allocate() {
                auto allocated_node = std::optional<foundation::mem::Cell *>();

                try{
                    const std::optional<foundation::mem::Cell *> &anOptional = active_allocator_->Allocate();
                    return anOptional;
                }catch(const CouldNotAllocateMemoryInArenaException& e) {
                    active_allocator_ = allocator_from_garbage_collected_.get();
                }
                try {
                    return active_allocator_->Allocate();
                }
                catch (const CouldNotAllocateMemoryException& memoryException){
                    RunGarbageCollection();
                }
                try {
                    return active_allocator_->Allocate();
                }
                catch (const CouldNotAllocateMemoryException& memoryException){
                    return {};
                }
            }

            void SimpleMemoryManagement::ReturnCellToPool(foundation::mem::Cell *node) {
                allocator_from_garbage_collected_->ReturnCellToPool(node);
            }

            SimpleMemoryManagement::SimpleMemoryManagement(std::unique_ptr<AllocatorFromArena> allocator_from_arena_from_arena,
                                                           std::unique_ptr<AllocatorFromGarbageCollected> allocator_from_garbage_collected )
                    : allocator_from_arena_from_arena_(std::move(allocator_from_arena_from_arena)),
                      allocator_from_garbage_collected_(std::move(allocator_from_garbage_collected))
            {
            }

            void SimpleMemoryManagement::Init() {
                allocator_from_arena_from_arena_->Init();
                allocator_from_garbage_collected_->Init();

                active_allocator_ = allocator_from_arena_from_arena_.get();
            }



            void SimpleMemoryManagement::MarkCellsAsUsed(const foundation::mem::Cell *node) {
                auto* pointer_to_set_busy_status = const_cast<foundation::mem::Cell *>(node);

                pointer_to_set_busy_status->SetBusy(true);
            }

            void SimpleMemoryManagement::RunGarbageCollection() {
                allocator_from_arena_from_arena_->MarkAllAsFree();
                garbage_collector_acceptor_->AcceptGarbageCollectorVisitor(this);
                allocator_from_arena_from_arena_->CollectAllFreeNodes(allocator_from_garbage_collected_.get());
            }

        } // mgr
    } // mem
} // foundation
