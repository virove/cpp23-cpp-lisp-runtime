#ifndef RUNTIME_DEFINES_H
#define RUNTIME_DEFINES_H

#include <memory>

#include "mem/cell.h"
#include "atom_factory.h"

namespace foundation{

    class Defines {
    public:
        explicit Defines(std::shared_ptr<foundation::AtomFactory> atom_factory)
            : atom_factory_{std::move(atom_factory)}{
        }

        void Init(){
            nil_ = atom_factory_->GetOrCreate("NIL");
            t_ = atom_factory_->GetOrCreate("T");
        }

        static constexpr unsigned long k_ATOM_NAME = 1;
        static constexpr unsigned long k_NIL_AtomValue = 2;
        static constexpr unsigned  long k_T_AtomValue = 3;

        inline  static foundation::mem::Cell *  NIL(){ return nil_; }
        inline  static  foundation::mem::Cell *  T(){ return t_; }

    private:
        inline static foundation::mem::Cell * nil_{nullptr};
        inline static foundation::mem::Cell * t_{nullptr};

        std::shared_ptr<foundation::AtomFactory> atom_factory_;
    };

} //namespace foundation

#endif //RUNTIME_DEFINES_H
