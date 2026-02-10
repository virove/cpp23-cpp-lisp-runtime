#ifndef BASE_TEST_CONTEXT_H
#define BASE_TEST_CONTEXT_H

#include <sstream>
#include <memory>
#include "atom_factory.h"
#include "cell_factory.h"
#include "foundation.h"
#include "parser/parser.h"

namespace test_support{

    using Std_StringStreamType = decltype(std::istringstream(""));

    template <auto InjectorFactoryFunction>
    class BaseTestContext {
        public:


        decltype(auto) ExecuteInjectorFactoryFunction() {
            return InjectorFactoryFunction();
        }


        BaseTestContext()
                    : injector{ExecuteInjectorFactoryFunction()}

            {
                foundation_ =  injector.template create<std::shared_ptr<foundation::Foundation>>();
                cell_factory_ = injector.template create<std::unique_ptr<lisp_runtime::CellFactory>>();
                atom_factory_ = injector.template create<std::shared_ptr< foundation::AtomFactory>>();
                foundation_->Init();
            }

            ~BaseTestContext(){
                foundation_->Shutdown();
            }

            auto CreateParser(){
                return std::make_unique< lisp_runtime::Parser<  Std_StringStreamType> >(cell_factory_.get()) ;
            }

            lisp_runtime::CellFactory* CellFactory() const {
                return cell_factory_.get();
            }

            std::shared_ptr<foundation::AtomFactory> GetAtomFactory() const {
                return atom_factory_;
            }

        protected:
            using ReturnTypeInjectorFactoryFunction = std::invoke_result_t<decltype(InjectorFactoryFunction)>;

            ReturnTypeInjectorFactoryFunction injector;
        private:
            std::shared_ptr<foundation::Foundation> foundation_;
            std::unique_ptr<lisp_runtime::CellFactory> cell_factory_;
            std::shared_ptr< foundation::AtomFactory> atom_factory_;
    };
}


#endif //BASE_TEST_CONTEXT_H
