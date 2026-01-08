#ifndef CELL_FACTORY_H
#define CELL_FACTORY_H

#include <string>
#include <optional>

#include "atom.h"
#include "mem/cell.h"

namespace lisp_runtime {

    class CellFactory {
    public:
        virtual ~CellFactory() = default;
        [[nodiscard]] virtual const mem::Cell*      CreateNumber(int number) = 0;
        [[nodiscard]] virtual  mem::CellList *CreateListCell(mem::Cell*  head, mem::Cell*  tail) = 0;
        [[nodiscard]] virtual  mem::CellList *CreateListCell(mem::Cell*  head) = 0;
    };

} // lisp_runtime

#endif //CELL_FACTORY_H
