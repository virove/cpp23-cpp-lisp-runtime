#ifndef VARIABLES_H
#define VARIABLES_H

#include <optional>

#include "atom.h"
#include "mem/cell.h"
#include "cell_adaptor.h"

namespace lisp_runtime::vars {

    class Variables;

    class Variables {
    public:
        virtual ~Variables() = default;

        virtual void Init() = 0;

        virtual void Shutdown() = 0;

        [[nodiscard]]
        virtual lisp_runtime::CellAdaptor CreateVariableBinding(lisp_runtime::CellAdaptor formal_parameters, lisp_runtime::CellAdaptor arguments, lisp_runtime::CellAdaptor context) = 0;

        virtual void SetVariableBinding(CellAdaptor adaptor) = 0;

        [[nodiscard]]
        virtual std::optional<lisp_runtime::CellAdaptor> GetVariableValue(lisp_runtime::CellAdaptor expression, lisp_runtime::CellAdaptor context) const= 0;
    };

} // lisp_runtime::vars


#endif //VARIABLES_H
