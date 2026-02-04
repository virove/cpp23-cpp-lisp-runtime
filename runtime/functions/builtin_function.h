#ifndef BUILTIN_FUNCTION_H
#define BUILTIN_FUNCTION_H

#include <stdexcept>
#include "cell_adaptor.h"
#include "runtime.h"
#include "defines.h"

namespace lisp_runtime::functions {


    class BuiltinAtomPredicate : public foundation::Function{
    public:
        foundation::mem::Cell *operator()(const foundation::mem::Cell *cell) override;

        [[nodiscard]]
        std::string GetName() const override { return "atom";}
    };

    class BuiltinFunctionBase : public foundation::Function{
    public:
        BuiltinFunctionBase() = delete;

        explicit BuiltinFunctionBase(lisp_runtime::Runtime* runtime) : runtime_{runtime}{
        }

    protected:
        std::shared_ptr<lisp_runtime::Runtime> runtime_;
    };

    class BuiltinArithmeticFunctionBase : public BuiltinFunctionBase{
    public:
        explicit BuiltinArithmeticFunctionBase(lisp_runtime::Runtime* runtime) : BuiltinFunctionBase{
                runtime} {
        }
        virtual int Operation(int arg1, int arg2)const = 0;

        foundation::mem::Cell *operator()(const foundation::mem::Cell *arguments) override {
            auto first_argument = CellAdaptor(arguments).Car();
            auto second_argument = CellAdaptor(arguments).Cdr().Car();

            if(first_argument.GetType() == foundation::mem::Cell::Type::NumberType
               &&  second_argument.GetType() == foundation::mem::Cell::Type::NumberType){
                auto amount = Operation(first_argument.GetThisCell()->number_, second_argument.GetThisCell()->number_);

                return        runtime_->CellFactory()->CreateNumber(amount);
            } else return foundation::Defines::NIL();

        }
    };

    class BuiltinPlus : public BuiltinArithmeticFunctionBase  {
    public:
        explicit BuiltinPlus(lisp_runtime::Runtime* runtime) :  BuiltinArithmeticFunctionBase(runtime){}

        [[nodiscard]]
        int Operation(int first_argument,
                      int second_argument) const override { return first_argument  + second_argument ; }

        [[nodiscard]]
        std::string  GetName() const {
            return "+";
        }
    };

    class BuiltinMinus : public BuiltinArithmeticFunctionBase  {
    public:
        explicit BuiltinMinus(lisp_runtime::Runtime* runtime) :  BuiltinArithmeticFunctionBase( runtime){}

        [[nodiscard]]
        int Operation(int first_argument,
                      int second_argument) const override{ return first_argument  - second_argument ; }

        [[nodiscard]]
        std::string  GetName() const override {
            return "-";
        }
    };

    class BuiltinMultiplication : public BuiltinArithmeticFunctionBase  {
    public:
        explicit BuiltinMultiplication(lisp_runtime::Runtime* runtime) :  BuiltinArithmeticFunctionBase( runtime ){}

        [[nodiscard]]
        int Operation(int first_argument,
                      int second_argument) const override{ return first_argument  * second_argument ; }

        [[nodiscard]]
        std::string  GetName() const override{
            return "*";
        }
    };

    class BuiltinDivision : public BuiltinArithmeticFunctionBase  {
    public:
        explicit BuiltinDivision(lisp_runtime::Runtime* runtime) :  BuiltinArithmeticFunctionBase( runtime ){}

        [[nodiscard]]
        int Operation(int first_argument,
                      int second_argument) const override{
            if(second_argument==0){
                throw std::runtime_error("Not allowed division on zero");
            }
            return first_argument / second_argument;
        }

        [[nodiscard]]
        std::string  GetName() const override{
            return "/";
        }
    };


   class BuiltinComparisonEqual : public foundation::Function{
    public:
    foundation::mem::Cell *operator()(const foundation::mem::Cell *cell_arguments) override {
        CellAdaptor arguments = {cell_arguments};
        auto first_argument = arguments.Car();
        auto second_argument = arguments.Cdr().Car();

        if(first_argument.GetType() == foundation::mem::Cell::Type::NumberType &&  second_argument.GetType() == foundation::mem::Cell::Type::NumberType){
            auto result = first_argument.GetHead()->number_ == second_argument.GetHead()->number_;

            return        result ? foundation::Defines::T() : foundation::Defines::NIL();
        } else return foundation::Defines::NIL();
    }

    [[nodiscard]]
    std::string GetName() const override { return "=";}
};



} // list_runtime

#endif //BUILTIN_FUNCTION_H
