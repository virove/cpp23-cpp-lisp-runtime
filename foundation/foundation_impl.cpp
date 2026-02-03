#include <stdexcept>
#include "foundation_impl.h"

namespace foundation {
    void FoundationImpl::Init() {
        memory_management_->Init();

        atom_factory_->Init();

        auto nil = atom_factory_->GetOrCreate("nil");
        auto t = atom_factory_->GetOrCreate("t");

        if(!(nil.has_value() && t.has_value())){
            throw std::runtime_error("Failed to init runtime: allocate memory error");
        }
        defines_->Init(nil.value(), t.value());
    }

    void FoundationImpl::Shutdown() {
//        defines_->Shutdown();
        atom_factory_->Shutdown();
        memory_management_->Shutdown();
    }

} // foundation