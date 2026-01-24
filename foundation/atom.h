#ifndef LISP_RUNTIME_ATOM_H
#define LISP_RUNTIME_ATOM_H

namespace foundation {
    using ATOM = unsigned long;

    static constexpr ATOM k_ATOM_PROPERTY_NAME = 1;
    static constexpr ATOM k_NIL_AtomValue = 2;
    static constexpr ATOM k_T_AtomValue = 3;
}

#endif //LISP_RUNTIME_ATOM_H