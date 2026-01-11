#ifndef SIMPLE_CELL_FACTORY_H
#define SIMPLE_CELL_FACTORY_H

#include <memory>

#include "cell_factory.h"
#include "mem/alloc_operation.h"
#include "defines.h"

namespace lisp_runtime {

    class SimpleCellFactory : public CellFactory{
    public:
        SimpleCellFactory(std::unique_ptr<foundation::mem::mgr::AllocOperation> alloc_operation, std::shared_ptr<foundation::Defines> runtime_defines);

        ~SimpleCellFactory() override;

        [[nodiscard]]const foundation::mem::Cell *CreateNumber(int number) override;

        [[nodiscard]]    foundation::mem::CellList *CreateListCell(foundation::mem::Cell*  head, foundation::mem::Cell*  tail) override ;
        [[nodiscard]]    foundation::mem::CellList *CreateListCell(foundation::mem::Cell*  head) override;


    private:
        std::unique_ptr<foundation::mem::mgr::AllocOperation> alloc_operation_;
        std::shared_ptr<foundation::Defines> runtime_defines_;
    };

} // foundation

#endif //SIMPLE_CELL_FACTORY_H
