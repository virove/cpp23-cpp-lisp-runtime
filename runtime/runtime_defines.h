#ifndef RUNTIME_DEFINES_H
#define RUNTIME_DEFINES_H

#include "mem/cell.h"

namespace lisp_runtime{

    class RuntimeDefines {
    public:
        RuntimeDefines() {
            nil_ = new mem::Cell();
            t_ = new mem::Cell;
        }

        inline  static lisp_runtime::mem::Cell *  NIL(){ return nil_; }
        inline  static  lisp_runtime::mem::Cell *  T(){ return t_; }

    private:
        inline static lisp_runtime::mem::Cell * nil_;
        inline static lisp_runtime::mem::Cell * t_;
    };

} //namespace lisp_runtime

#endif //RUNTIME_DEFINES_H
