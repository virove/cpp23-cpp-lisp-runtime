#ifndef SIMPLE_LISP_RUNTIME_H
#define SIMPLE_LISP_RUNTIME_H

#include <memory>

#include "lisp_runtime.h"
#include "cell_factory.h"
#include "atom_factory.h"

namespace lisp_runtime{

    class SimpleLispRuntime : public LispRuntime {
    public:
        explicit SimpleLispRuntime(std::unique_ptr<lisp_runtime::CellFactory> cell_factory, std::shared_ptr< foundation::AtomFactory> atom_factory)
            : cell_factory_{std::move(cell_factory)}, atom_factory_{std::move(atom_factory)}{
        }

        ~SimpleLispRuntime() override;

        void Init() override;

        void Shutdown() override;

        SExpr Eval(SExpr expression, SExpr context) override;

        SExpr Apply(SExpr function, SExpr arguments, SExpr context) override;
    public:
        std::unique_ptr<lisp_runtime::CellFactory> cell_factory_;
        std::shared_ptr< foundation::AtomFactory> atom_factory_;
    };

} // namespace foundation

#endif //SIMPLE_LISP_RUNTIME_H
