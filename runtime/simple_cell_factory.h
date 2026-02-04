#ifndef SIMPLE_CELL_FACTORY_H
#define SIMPLE_CELL_FACTORY_H

#include <memory>

#include "mem/memory_management.h"
#include "cell_factory.h"
#include "defines.h"

namespace lisp_runtime {


    class MemoryAllocationNoMemoryError : public std::runtime_error{
    public:
        explicit MemoryAllocationNoMemoryError(const std::string &arg) : runtime_error(arg) {
        }
    };

    class SimpleCellFactory : public CellFactory{
    public:
        SimpleCellFactory(std::shared_ptr<foundation::mem::mgr::MemoryManagement> memory_management,
                          std::shared_ptr<foundation::AtomFactory> atom_factory,
                          std::shared_ptr<foundation::Defines> runtime_defines);

        ~SimpleCellFactory() override;

        [[nodiscard]]    foundation::mem::Cell *CreateNumber(int number) override;

        foundation::mem::CellList *CreateListCell() override;

        [[nodiscard]]    foundation::mem::CellList *CreateListCell(const foundation::mem::Cell*  head, const foundation::mem::Cell*  tail) override ;

        [[nodiscard]]    foundation::mem::CellList *CreateListCell(foundation::mem::Cell*  head) override;

        [[nodiscard]]    foundation::mem::Cell *GetOrCreate(const std::string &atomname) override;

    private:
        std::shared_ptr<foundation::mem::mgr::MemoryManagement> memory_management_;
        std::shared_ptr<foundation::Defines> runtime_defines_;
        std::shared_ptr<foundation::AtomFactory> atom_factory_;
    };

} // foundation

#endif //SIMPLE_CELL_FACTORY_H
