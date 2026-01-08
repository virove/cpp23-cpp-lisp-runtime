#ifndef SIMPLE_CELL_FACTORY_H
#define SIMPLE_CELL_FACTORY_H

#include <memory>

#include "cell_factory.h"
#include "mem/alloc_operation.h"
#include "runtime_defines.h"

namespace lisp_runtime {

    class SimpleCellFactory : public CellFactory{
    public:
        SimpleCellFactory(std::unique_ptr<mem::mgr::AllocOperation> alloc_operation, const RuntimeDefines &runtime_defines);

        ~SimpleCellFactory() override;

        [[nodiscard]]const mem::Cell *CreateNumber(int number) override;

        [[nodiscard]]    mem::CellList *CreateListCell(mem::Cell*  head, mem::Cell*  tail) override ;
        [[nodiscard]]    mem::CellList *CreateListCell(mem::Cell*  head) override;


    private:
        std::unique_ptr<mem::mgr::AllocOperation> alloc_operation_;
        const lisp_runtime::RuntimeDefines& runtime_defines_;
    };

} // lisp_runtime

#endif //SIMPLE_CELL_FACTORY_H
