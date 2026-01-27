#ifndef SIMPLE_MEMORY_MANAGEMENT_H
#define SIMPLE_MEMORY_MANAGEMENT_H

#include "mem/free_node.h"
#include "mem/allocator_from_arena.h"
#include "mem/allocator_from_garbage_collected.h"
#include "garbage_collector_visitor.h"
#include "garbage_collector_acceptor.h"
#include "memory_management.h"

namespace foundation::mem {

        class Cell;

        namespace mgr {

            class SimpleMemoryManagement : public foundation::mem::mgr::MemoryManagement{
            public:
                SimpleMemoryManagement(std::unique_ptr<AllocatorFromArena> allocator_from_arena_from_arena,
                                       std::unique_ptr<AllocatorFromGarbageCollected> allocator_from_garbage_collected);

                void Init()override;

                void Shutdown() override {
                }

                std::optional<foundation::mem::Cell*>   Allocate() override;
                void ReturnCellToPool(foundation::mem::Cell* node) override;

                void MarkCellsAsUsed(const foundation::mem::Cell *node) override;
                void RegisterGarbageCollectorAcceptor(GarbageCollectorAcceptor* acceptor) { garbage_collector_acceptor_ = acceptor; }
            private:
                void RunGarbageCollection();

                AllocOperation* active_allocator_{nullptr};
                std::unique_ptr<foundation::mem::mgr::AllocatorFromArena> allocator_from_arena_from_arena_;
                std::unique_ptr<AllocatorFromGarbageCollected> allocator_from_garbage_collected_;
                GarbageCollectorAcceptor* garbage_collector_acceptor_;
            };

        } // mgr
} // foundation::mem

#endif //SIMPLE_MEMORY_MANAGEMENT_H
