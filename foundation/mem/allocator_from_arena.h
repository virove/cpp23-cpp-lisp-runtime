#ifndef ALLOCATOR_FROM_ARENA_H
#define ALLOCATOR_FROM_ARENA_H

#include <memory>
#include <boost/di.hpp>

#include "alloc_operation.h"

namespace foundation {
    namespace mem {
        namespace mgr {
            class FreeNode;
            inline auto number_of_preallocated_nodes = []{};

            class AllocatorFromArena : public foundation::mem::mgr::AllocOperation {
            public:
                ~AllocatorFromArena() override = default;

                void Init() override;
                void Shutdown() override {}

                 BOOST_DI_INJECT(explicit AllocatorFromArena, (named = number_of_preallocated_nodes) std::size_t number_of_preallocated_nodes);

                std::optional<foundation::mem::Cell*> Allocate() override;

                virtual void MarkAllAsFree();

                virtual void CollectAllFreeNodes(FreeNode* gc_allocator) const;

                AllocatorFromArena() = delete;
            private:
                std::size_t number_of_preallocated_nodes_;
                std::unique_ptr<foundation::mem::Cell[]> allocated_memory_;
                int number_of_allocation_;
            };
        } //            namespace mgr
    } //        namespace mem
} //            namespace foundation

#endif //ALLOCATOR_FROM_ARENA_H
