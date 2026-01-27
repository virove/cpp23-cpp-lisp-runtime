#ifndef LISP_RUNTIME_APP_H
#define LISP_RUNTIME_APP_H

#include <memory>
#include "lisp_runtime.h"
#include "defines.h"
#include "foundation_impl.h"

namespace app{

    class App final {
    public:
        explicit App(std::unique_ptr<lisp_runtime::LispRuntime> lisp_runtime,
                     std::unique_ptr<foundation::Foundation> foundation)
        : lisp_runtime_{std::move(lisp_runtime)}, foundation_{std::move(foundation)}
        {}

        virtual ~App() = default;

        void Init();
        void Run();
        void Shutdown() ;
    private:
        std::unique_ptr<foundation::Foundation> foundation_;
        std::unique_ptr<lisp_runtime::LispRuntime> lisp_runtime_;
    };

} // namespace app


#endif //LISP_RUNTIME_APP_H
