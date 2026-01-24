#include "allocator_from_arena.h"
#include "mem/free_node.h"

namespace foundation {
    namespace mem {
        namespace mgr {

            AllocatorFromArena::AllocatorFromArena(std::size_t number_of_preallocated_nodes) :
                    number_of_preallocated_nodes_{number_of_preallocated_nodes},
                    number_of_allocation_{0}
            {
            }

            void AllocatorFromArena::Init() {
                allocated_memory_ = std::make_unique<foundation::mem::Cell[]>(number_of_preallocated_nodes_);
            }

            std::optional<foundation::mem::Cell*>  AllocatorFromArena::Allocate() {
                if(number_of_allocation_ < number_of_preallocated_nodes_){
                    foundation::mem::Cell* allocated_node = &allocated_memory_[number_of_allocation_++];

                    return {allocated_node};
                }
                else{
                    throw CouldNotAllocateMemoryInArenaException();
                }
            }

            void AllocatorFromArena::MarkAllAsFree() {
                auto array_nodes  = allocated_memory_.get();

                for(int i = 0; i < number_of_preallocated_nodes_ ;  i++ ){
                    auto& node = array_nodes[i];

                    node.SetBusy(false);
                }
            }

            void AllocatorFromArena::CollectAllFreeNodes(FreeNode* gc_allocator) const {
                std::optional<foundation::mem::Cell*>  result{};
                auto array_nodes  = allocated_memory_.get();

                for(int i = 0; i < number_of_preallocated_nodes_ ;  i++ ){
                    auto& node = array_nodes[i];

                    if(!node.IsBusy()){
                        gc_allocator->ReturnCellToPool(&node);
                    }
                }
            }

        } //            namespace mgr
    } //        namespace mem
} //            namespace foundation

