#ifndef ALLOC_OPERATION_H
#define ALLOC_OPERATION_H

#include <optional>
#include "cell.h"

namespace foundation {
    namespace mem {
        namespace mgr {

            class AllocOperation {
            public:
                virtual ~AllocOperation() = default;
                virtual void Init() = 0;
                virtual mem::Cell* Allocate() = 0;
            };
        } // mgr
    } // mem
} // foundation // namespace NAMESPACES_CLOSE

#endif //ALLOC_OPERATION_H
