#ifndef LISP_RUNTIME_APP_H
#define LISP_RUNTIME_APP_H

#include <memory>
#include "lisp_runtime.h"
#include "runtime_defines.h"

namespace app{

    class App final {
    public:
        explicit App(std::unique_ptr<lisp_runtime::LispRuntime> lisp_runtime,
                     std::shared_ptr<lisp_runtime::RuntimeDefines> runtime_defines)
        : lisp_runtime_{std::move(lisp_runtime)}, defines_{std::move(runtime_defines)}
        {}

        virtual ~App() = default;

        void Init();
        void Run();
        void Shutdown() ;
    private:
        std::unique_ptr<lisp_runtime::LispRuntime> lisp_runtime_;
        std::shared_ptr<lisp_runtime::RuntimeDefines> defines_;
    };

} // namespace app


#endif //LISP_RUNTIME_APP_H
