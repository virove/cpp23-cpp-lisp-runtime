#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

#include "mem/cell.h"

namespace foundation {

    class Function{
    public:
        virtual ~Function()  = default;

        virtual foundation::mem::Cell* operator()(const foundation::mem::Cell*) = 0;

        [[nodiscard]]
        virtual std::string GetName()const = 0;
    };
} // foundation  

#endif //FUNCTION_H
