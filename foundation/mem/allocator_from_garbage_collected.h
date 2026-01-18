#ifndef ALLOCATOR_FROM_GARBAGE_COLLECTED_H
#define ALLOCATOR_FROM_GARBAGE_COLLECTED_H

#include <optional>

#include "alloc_operation.h"
#include "mem/free_node.h"

namespace foundation {
    namespace mem {
        namespace mgr {

            class AllocatorFromGarbageCollected : public foundation::mem::mgr::AllocOperation, public foundation::mem::mgr::FreeNode{
            public:
                void Init() override{};
                std::optional<foundation::mem::Cell*> Allocate() override;
                void ReturnCellToPool(foundation::mem::Cell* node) override;

            private:
                const foundation::mem::Cell* head_{nullptr};
            };

        } //    namespace mgr
    } //        namespace mem
} //            namespace foundation

#endif //ALLOCATOR_FROM_GARBAGE_COLLECTED_H
