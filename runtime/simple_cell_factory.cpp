#include <stdexcept>

#include "simple_cell_factory.h"
#include "mem/memory_management.h"

lisp_runtime::SimpleCellFactory::SimpleCellFactory(std::shared_ptr<foundation::mem::mgr::MemoryManagement> memory_management,
                                                    std::shared_ptr<foundation::AtomFactory> atom_factory,
                                                    std::shared_ptr<foundation::Defines> runtime_defines)

        : memory_management_(std::move(memory_management)),
          atom_factory_(std::move(atom_factory)),
        runtime_defines_(std::move(runtime_defines))
{
}

lisp_runtime::SimpleCellFactory::~SimpleCellFactory()  = default;

foundation::mem::CellList *
lisp_runtime::SimpleCellFactory::CreateListCell(foundation::mem::Cell *head, foundation::mem::Cell *tail) {
    auto an_optional_cell = memory_management_->Allocate();
    if(!an_optional_cell.has_value()){
        throw MemoryAllocationNoMemoryError("Failed to allocate memory");
    }

    return  new (an_optional_cell.value()) foundation::mem::CellList(head, tail) ;
}

const foundation::mem::Cell *lisp_runtime::SimpleCellFactory::CreateNumber(int number) {
    auto allocated_memory = memory_management_->Allocate();

    if(allocated_memory.has_value()){
        return new(allocated_memory.value()) foundation::mem::CellNumber(number);
    } else{
        throw MemoryAllocationNoMemoryError("Failed to allocate memory for the number" + std::to_string( number ));
    }
}


foundation::mem::CellList *lisp_runtime::SimpleCellFactory::CreateListCell(foundation::mem::Cell *head) {
    return CreateListCell(head, foundation::Defines::NIL());
}

foundation::mem::Cell *lisp_runtime::SimpleCellFactory::GetOrCreate(const std::string &atomname) {
    auto an_optional_cell = atom_factory_->GetOrCreate(atomname);
    if(!an_optional_cell.has_value()){
        throw MemoryAllocationNoMemoryError("Failed to allocate memory for symbol" + atomname);
    }
    return an_optional_cell.value();
}



