

#ifndef GARBAGE_COLLECTOR_ACCEPTOR_H
#define GARBAGE_COLLECTOR_ACCEPTOR_H

#include "garbage_collector_visitor.h"

namespace foundation {
    namespace mem {

        class Cell;

        namespace mgr {

            class GarbageCollectorAcceptor{
            public:
                virtual ~GarbageCollectorAcceptor()  = default;

                virtual void AcceptGarbageCollectorVisitor(foundation::mem::mgr::GarbageCollectorVisitor *visitor) = 0;
            };

        } // mgr
    } // mem
} // foundation



#endif //GARBAGE_COLLECTOR_ACCEPTOR_H
