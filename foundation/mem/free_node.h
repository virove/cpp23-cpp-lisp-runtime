

#ifndef FREE_NODE_H
#define FREE_NODE_H

namespace foundation {
    namespace mem {

        class Cell;

        namespace mgr {


            class FreeNode {
            public:
                virtual ~FreeNode() = default;
                virtual void ReturnCellToPool(foundation::mem::Cell* node) = 0;
            };

        } // mgr
    } // mem
} // foundation



#endif //FREE_NODE_H
