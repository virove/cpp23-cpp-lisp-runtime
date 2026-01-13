#include <stdexcept>

#include "simple_cell_factory.h"

const foundation::mem::Cell *lisp_runtime::SimpleCellFactory::CreateNumber(int number) {
        auto allocated_memory = alloc_operation_->Allocate();

        return new (allocated_memory) foundation::mem::CellNumber(number);
}

lisp_runtime::SimpleCellFactory::SimpleCellFactory(std::unique_ptr<foundation::mem::mgr::AllocOperation> alloc_operation,
                                                    std::shared_ptr<foundation::AtomFactory> atom_factory,
                                                    std::shared_ptr<foundation::Defines> runtime_defines)

        : alloc_operation_(std::move(alloc_operation)),
          atom_factory_(std::move(atom_factory)),
        runtime_defines_(std::move(runtime_defines))
{
}

lisp_runtime::SimpleCellFactory::~SimpleCellFactory()  = default;

foundation::mem::CellList *
lisp_runtime::SimpleCellFactory::CreateListCell(foundation::mem::Cell *head, foundation::mem::Cell *tail) {
    auto allocated_memory = alloc_operation_->Allocate();

    return new (allocated_memory) foundation::mem::CellList(head, tail);
}

foundation::mem::CellList *lisp_runtime::SimpleCellFactory::CreateListCell(foundation::mem::Cell *head) {
    return CreateListCell(head, foundation::Defines::NIL());
}

foundation::mem::Cell *lisp_runtime::SimpleCellFactory::GetOrCreate(const std::string &atomname) {
    return atom_factory_->GetOrCreate(atomname);
}





