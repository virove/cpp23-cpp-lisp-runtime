#ifndef VARIABLES_IMPL_H
#define VARIABLES_IMPL_H

#include <memory>
#include <optional>

#include "variables.h"
#include "cell_factory.h"

namespace lisp_runtime::vars {
    class VariablesImpl : public Variables{
    public:
        explicit VariablesImpl(std::unique_ptr<lisp_runtime::CellFactory> cell_factory)
                : cell_factory_{std::move(cell_factory)} {}


        void Init() override;

        void Shutdown() override;
        lisp_runtime::CellAdaptor CreateVariableBinding(lisp_runtime::CellAdaptor previous_binding,
                                              lisp_runtime::CellAdaptor formal_parameters,
                                              lisp_runtime::CellAdaptor arguments) override;

        void SetVariableBinding(CellAdaptor adaptor) override;

        std::optional<lisp_runtime::CellAdaptor> GetVariableValue(lisp_runtime::CellAdaptor expression, lisp_runtime::CellAdaptor context) const override;


    private:
        std::unique_ptr<lisp_runtime::CellFactory>  cell_factory_;
    };
} // lisp_runtime::vars

#endif //VARIABLES_IMPL_H
