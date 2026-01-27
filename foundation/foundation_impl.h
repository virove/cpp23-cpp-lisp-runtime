#ifndef FOUNDATION_IMPL_H
#define FOUNDATION_IMPL_H

#include <memory>

#include "foundation.h"
#include "atom_factory.h"
#include "mem/memory_management.h"
#include "defines.h"

namespace foundation {

    class FoundationImpl : public Foundation{
    public:
        FoundationImpl(std::shared_ptr<foundation::mem::mgr::MemoryManagement> memory_management,
                       std::shared_ptr<AtomFactory>    atom_factory,
                       std::shared_ptr<Defines>        defines)
                   : memory_management_{std::move(memory_management)},
                   atom_factory_{std::move(atom_factory)},
                   defines_{std::move(defines)}
                   {
                   }
        void Init() override;
        void Shutdown() override;
    private:
        std::shared_ptr<foundation::mem::mgr::MemoryManagement> memory_management_;
        std::shared_ptr<AtomFactory>    atom_factory_;
        std::shared_ptr<Defines>        defines_;
    };

} // foundation

#endif //FOUNDATION_IMPL_H
