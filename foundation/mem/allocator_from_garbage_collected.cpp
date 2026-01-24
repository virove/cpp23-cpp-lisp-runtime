#include "allocator_from_garbage_collected.h"


namespace foundation {
    namespace mem {
        namespace mgr {

            std::optional<foundation::mem::Cell*>  AllocatorFromGarbageCollected::Allocate() {
                if( head_!= nullptr){
                    const foundation::mem::Cell* allocated = head_;

                    head_ = head_->tail_;

                    return {const_cast<foundation::mem::Cell*>(allocated)};
                } else{
                    throw CouldNotAllocateMemoryException();
                }
            }

            void AllocatorFromGarbageCollected::ReturnCellToPool(foundation::mem::Cell* node) {
                node->tail_ = head_;
                head_ = node;
            }
        } //    namespace mgr
    } //        namespace mem
} //            namespace foundation