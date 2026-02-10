#ifndef FORM_H
#define FORM_H

#include <memory>

#include "cell_adaptor.h"
#include "cell_factory.h"
#include "lisp_runtime.h"
#include "foundation.h"

namespace lisp_runtime::forms {

        class Form {
        public:
            Form( std::shared_ptr<foundation::Foundation> foundation, CellFactory *cell_factory)
                : cell_factory_(cell_factory),
                    foundation_{std::move(foundation)}
            {}

            virtual ~Form() = default;

            virtual void Init() = 0;

            virtual void Shutdown() = 0;

            virtual lisp_runtime::CellAdaptor Eval(LispRuntime* lisp_runtime,CellAdaptor expression, CellAdaptor context )const = 0;

            [[nodiscard]]
            virtual std::string Name() const = 0;

            [[nodiscard]]
            CellFactory *GetCellFactory() const {
                return cell_factory_;
            }

            [[nodiscard]]
            const std::shared_ptr<foundation::Foundation> &GetFoundation() const {
                return foundation_;
            }

        private:
            lisp_runtime::CellFactory* cell_factory_;
            std::shared_ptr<foundation::Foundation> foundation_;
        };

} // lisp_runtime forms

#endif //FORM_H
