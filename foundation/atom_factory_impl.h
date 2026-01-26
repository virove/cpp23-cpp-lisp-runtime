#ifndef ATOM_FACTORY_IMPL_H
#define ATOM_FACTORY_IMPL_H

#include <unordered_map>
#include <any>
#include <mutex>
#include <memory>
#include <iostream>

#include "atom_factory.h"
#include "mem/simple_memory_management.h"

namespace foundation {

    class AtomFactoryImpl : public AtomFactory{
    public:
        explicit AtomFactoryImpl(std::shared_ptr<foundation::mem::mgr::MemoryManagement> memory_management)
        : memory_management_{std::move(memory_management)}
        {
        }

        void Init() override {
            GetOrCreate("ATOM_NAME");
            GetOrCreate("NIL");
            GetOrCreate("T");
        }

        void Shutdown() override {
        }

        std::optional<foundation::mem::Cell*> GetOrCreate(const std::string& atomname) override {
            std::lock_guard<std::mutex> _lock(mutex_);

            if(auto find_result = string_to_atom_.find(atomname); find_result == string_to_atom_.end()){
                unsigned long atom = ++last_allocated_atom;
                auto allocated_memory = memory_management_->Allocate();
                if(allocated_memory.has_value()){
                    auto* node_atom = new (allocated_memory.value()) foundation::mem::CellAtom(atom);
                    string_to_atom_.emplace(atomname, node_atom);
                    AtomProperties atom_properties = {{atom_name_properties, atomname}};
                    atom_to_properties_.emplace(atom, atom_properties);

                    return node_atom;
                } else{
                    return std::nullopt;
                }
            } else{
                return find_result->second;
            }
        }

        template<typename T>
        std::optional<T> GetProperty(foundation::ATOM,foundation::ATOM property_name)const;

        template<typename T>
        void SetProperty(foundation::ATOM, foundation::ATOM name, T&& value);

        std::optional<std::string>  GetAtomName(foundation::ATOM atom) const override{
        {
            if(auto find_result = atom_to_properties_.find(atom); find_result != atom_to_properties_.end()){
                auto find_property_result = find_result->second.find(atom_name_properties);

                if(find_property_result != find_result->second.end()){
                    return std::any_cast<std::string>(find_property_result->second);
                } else{
                    return std::nullopt;
                }
            }
            else{
                return std::nullopt;
            }
        }
    }

    private:
        using AtomProperties = std::unordered_map<foundation::ATOM, std::any>;
        using StringToAtom = std::unordered_map<std::string, foundation::mem::CellAtom*>;
        using AtomToProperties= std::unordered_map<foundation::ATOM, AtomProperties>;
        const foundation::ATOM atom_name_properties = {1};

        std::shared_ptr<foundation::mem::mgr::MemoryManagement> memory_management_;

        StringToAtom string_to_atom_;
        AtomToProperties atom_to_properties_;
        unsigned long last_allocated_atom = {0};
        std::mutex mutex_;
    };

    template<typename T>
    void AtomFactoryImpl::SetProperty(foundation::ATOM atom, foundation::ATOM property_name, T &&value)  {
        if( auto iter = atom_to_properties_.find(atom) ; iter != atom_to_properties_.end()){
            auto& properties = iter->second;
            T t = value;
            properties[property_name] = t;
        }
    }

    template<typename T>
    std::optional<T>  AtomFactoryImpl::GetProperty(foundation::ATOM atom, foundation::ATOM property_name ) const {
        if(auto iter = atom_to_properties_.find(atom); iter == atom_to_properties_.end()){
            return std::nullopt;
        }
        else{
            const auto& properties = iter->second;

            if(auto properties_iter = properties.find(property_name); properties_iter == properties.end()){
                return std::nullopt;
            } else{
                if(properties_iter->second.has_value()){
                    auto& orig = properties_iter->second;
                    T value = std::any_cast<T>(orig);
                    return std::optional<T>(value);
                } else{
                    return std::nullopt;
                }
            }
        }
    };
} // namespace foundation

#endif //ATOM_FACTORY_IMPL_H
