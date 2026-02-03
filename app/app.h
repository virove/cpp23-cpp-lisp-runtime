#ifndef LISP_RUNTIME_APP_H
#define LISP_RUNTIME_APP_H

#include <memory>

#include "defines.h"
#include "foundation.h"
#include "runtime.h"

namespace app{

    class App final {
    public:
        explicit App(std::shared_ptr<foundation::Foundation> foundation, std::shared_ptr<lisp_runtime::Runtime> runtime)
        : foundation_{std::move(foundation)}, runtime_{std::move(runtime)}
        {}

        virtual ~App() = default;

        void Init();
        void Run();
        void Shutdown() ;
    private:
        std::shared_ptr<foundation::Foundation> foundation_;
        std::shared_ptr<lisp_runtime::Runtime> runtime_;
    };

} // namespace app


#endif //LISP_RUNTIME_APP_H
