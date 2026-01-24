#ifndef ALLOC_OPERATION_H
#define ALLOC_OPERATION_H

#include <optional>
#include "cell.h"

namespace foundation {
    namespace mem {
        namespace mgr {

            class CouldNotAllocateMemoryException : public  std::exception{
            };

            class CouldNotAllocateMemoryInArenaException : public  std::exception{
            };

            class AllocOperation {
            public:
                virtual ~AllocOperation() = default;
                virtual void Init() = 0;
                virtual std::optional<foundation::mem::Cell*>  Allocate() = 0;
            };
        } // mgr
    } // mem
} // foundation

#endif //ALLOC_OPERATION_H
