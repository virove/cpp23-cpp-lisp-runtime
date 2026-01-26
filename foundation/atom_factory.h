#ifndef ATOM_FACTORY_H
#define ATOM_FACTORY_H


#include <optional>
#include <string>
#include "atom.h"
#include "mem/cell.h"

namespace foundation{

    class AtomFactory {
    public:
        virtual ~AtomFactory()  = default;
        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual std::optional<std::string>  GetAtomName(foundation::ATOM atom) const = 0;
        virtual std::optional<foundation::mem::Cell*> GetOrCreate(const std::string& atomname) = 0;

        template<typename T>
        std::optional<T> GetProperty(foundation::ATOM,foundation::ATOM property_name)const;

        template<typename T>
        void SetProperty(foundation::ATOM, foundation::ATOM name, T&& value);
    };

} // foundation

#endif //ATOM_FACTORY_H
