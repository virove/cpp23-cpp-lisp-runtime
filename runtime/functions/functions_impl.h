#ifndef FUNCTIONS_IMPL_H
#define FUNCTIONS_IMPL_H

#include <list>
#include "functions.h"
#include "cell_factory.h"

namespace lisp_runtime {
    namespace functions {

        class FunctionsImpl : public Functions{
        public:
            FunctionsImpl(std::unique_ptr<CellFactory> cell_factory,
                          std::shared_ptr<foundation::AtomFactory> atom_factory) :
                          cell_factory_(std::move(cell_factory)),
                          atom_factory_{std::move(atom_factory)}{
            }

            void Init() override;

            void Shutdown() override {}

            FindFunctionResult FindFunction(const CellAdaptor &function_name) override;

            void RegisterFunction(foundation::Function* function) override;
        private:
            std::list<std::unique_ptr<foundation::Function>> to_be_registered_;
            std::unique_ptr<CellFactory> cell_factory_;

            std::shared_ptr<foundation::AtomFactory> atom_factory_;

            foundation::ATOM atom_fn_;

            foundation::ATOM atom_builtin_function_property_;
        };

    } // functions
} // lisp_runtime  

#endif //FUNCTIONS_IMPL_H
