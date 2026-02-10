#ifndef RUNTIME_H
#define RUNTIME_H

#include <memory>

#include "lisp_runtime.h"
#include "cell_factory.h"

namespace lisp_runtime {

    class Runtime {
    public:
        virtual ~Runtime() = default;
        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        [[nodiscard]]
        virtual std::shared_ptr<LispRuntime> GetLispRuntime() const = 0;

        [[nodiscard]]
        virtual lisp_runtime::CellFactory* CellFactory() const = 0;
    };

} // lisp_runtime  

#endif //RUNTIME_H
