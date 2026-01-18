

#ifndef GARBAGE_COLLECTOR_VISITOR_H
#define GARBAGE_COLLECTOR_VISITOR_H

#include "cell.h"

namespace foundation {
    namespace mem {
        namespace mgr {

            class GarbageCollectorVisitor {
            public:
                virtual ~GarbageCollectorVisitor() = default;
                virtual void MarkCellsAsUsed(const foundation::mem::Cell*) = 0;
            };

        } // mgr
    } // mem
} // foundation

#endif //GARBAGE_COLLECTOR_VISITOR_H
