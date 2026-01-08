#include <stdexcept>

#include "simple_cell_factory.h"

const lisp_runtime::mem::Cell *lisp_runtime::SimpleCellFactory::CreateNumber(int number) {
        auto allocated_memory = alloc_operation_->Allocate();

        return new (allocated_memory) lisp_runtime::mem::CellNumber(number);
}

lisp_runtime::SimpleCellFactory::SimpleCellFactory(std::unique_ptr<mem::mgr::AllocOperation> alloc_operation, const RuntimeDefines &runtime_defines)
        : alloc_operation_(std::move(alloc_operation)), runtime_defines_(runtime_defines) {
}

lisp_runtime::SimpleCellFactory::~SimpleCellFactory()  = default;

lisp_runtime::mem::CellList *
lisp_runtime::SimpleCellFactory::CreateListCell(mem::Cell *head, mem::Cell *tail) {
    auto allocated_memory = alloc_operation_->Allocate();

    return new (allocated_memory) lisp_runtime::mem::CellList(head, tail);
}

lisp_runtime::mem::CellList *lisp_runtime::SimpleCellFactory::CreateListCell(mem::Cell *head) {
    return CreateListCell(head, RuntimeDefines::NIL());
}





