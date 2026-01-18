

#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

namespace foundation {
    namespace mem {
        namespace mgr {

            class MemoryManagement :   public  AllocOperation, protected GarbageCollectorVisitor, protected  FreeNode{
            };

        } // mgr
    } // mem
} // foundation

#endif //MEMORY_MANAGEMENT_H
