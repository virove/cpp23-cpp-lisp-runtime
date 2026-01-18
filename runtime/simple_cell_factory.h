#ifndef SIMPLE_CELL_FACTORY_H
#define SIMPLE_CELL_FACTORY_H

#include <memory>

#include "cell_factory.h"
#include "mem/alloc_operation.h"
#include "defines.h"

namespace lisp_runtime {


    class MemoryAllocationNoMemoryError : public std::runtime_error{
    public:
        explicit MemoryAllocationNoMemoryError(const std::string &arg) : runtime_error(arg) {
        }
    };

    class SimpleCellFactory : public CellFactory{
    public:
        SimpleCellFactory(std::unique_ptr<foundation::mem::mgr::AllocOperation> alloc_operation,
                          std::shared_ptr<foundation::AtomFactory> atom_factory,
                          std::shared_ptr<foundation::Defines> runtime_defines);

        ~SimpleCellFactory() override;

        [[nodiscard]]const foundation::mem::Cell *CreateNumber(int number) override;
        [[nodiscard]]    foundation::mem::CellList *CreateListCell(foundation::mem::Cell*  head, foundation::mem::Cell*  tail) override ;
        [[nodiscard]]    foundation::mem::CellList *CreateListCell(foundation::mem::Cell*  head) override;
        [[nodiscard]]    foundation::mem::Cell *GetOrCreate(const std::string &atomname) override;

    private:
        std::unique_ptr<foundation::mem::mgr::AllocOperation> alloc_operation_;
        std::shared_ptr<foundation::Defines> runtime_defines_;
        std::shared_ptr<foundation::AtomFactory> atom_factory_;
    };

} // foundation

#endif //SIMPLE_CELL_FACTORY_H
