#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "mem/alloc_operation.h"
#include "mem/free_node.h"
#include "garbage_collector_visitor.h"

namespace foundation {
    namespace mem {
        namespace mgr {

            class MemoryManagement :   public  AllocOperation, protected GarbageCollectorVisitor,   protected FreeNode{
            };

        } // mgr
    } // mem
} // foundation

#endif //MEMORY_MANAGEMENT_H
