#ifndef ATOM_FACTORY_H
#define ATOM_FACTORY_H

#include <optional>
#include <string>

#include "atom.h"
#include "mem/cell.h"

namespace foundation{

    class AtomFactory {
    public:
        using FuncType = foundation::mem::Cell* (*)( const foundation::mem::Cell*);

        virtual ~AtomFactory()  = default;

        virtual void Init() = 0;

        virtual void Shutdown() = 0;

        virtual std::optional<std::string>  GetAtomName(foundation::ATOM atom) const = 0;

        virtual std::optional<foundation::mem::Cell*> GetOrCreate(const std::string& atomname) = 0;

        virtual std::optional<foundation::mem::Cell*> GetProperty(foundation::ATOM,foundation::ATOM property_name)const = 0;

        virtual std::optional<std::string> GetPropertyString(foundation::ATOM,foundation::ATOM property_name)const = 0;

        virtual void SetPropertyString(foundation::ATOM atom, foundation::ATOM name, const std::string& value) = 0;

        virtual std::optional<FuncType> GetFunctionProperty(foundation::ATOM atom, foundation::ATOM property_name) const = 0;

        virtual void SetProperty(foundation::ATOM, foundation::ATOM name, foundation::mem::Cell* value) = 0;

        virtual void SetFunctionProperty(foundation::ATOM, foundation::ATOM name, FuncType) = 0;
    };



} // foundation

#endif //ATOM_FACTORY_H
