#ifndef LISP_RUNTIME_APP_H
#define LISP_RUNTIME_APP_H


#include "lisp_runtime.h"

namespace app{

    class App final {
    public:
        explicit App(lisp_runtime::LispRuntime* lisp_runtime)
        : lisp_runtime_{lisp_runtime}
        {  }
        ~App() = default;

        void Init();
        void Run();
        void Shutdown() ;
    private:
        lisp_runtime::LispRuntime* lisp_runtime_;
    };

} // namespace app


#endif //LISP_RUNTIME_APP_H
