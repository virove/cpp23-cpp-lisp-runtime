#ifndef RUNTIME_DEFINES_H
#define RUNTIME_DEFINES_H

#include <memory>

#include "mem/cell.h"
#include "atom_factory.h"

namespace foundation{

    class Defines {
    public:
        Defines() = default;

        void Init(foundation::mem::Cell * nil, foundation::mem::Cell * t){
            nil_ = nil;
            t_ = t;
        }

        inline  static foundation::mem::Cell *  NIL(){ return nil_; }
        inline  static  foundation::mem::Cell *  T(){ return t_; }
    private:
        inline static foundation::mem::Cell * nil_{nullptr};
        inline static foundation::mem::Cell * t_{nullptr};
    };

} //namespace foundation

#endif //RUNTIME_DEFINES_H
