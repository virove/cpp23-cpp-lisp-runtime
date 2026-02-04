#include <stdexcept>
#include "utilities.h"
#include "mem/cell.h"
#include "cell_factory.h"
#include "atom_factory.h"
#include "defines.h"

namespace lisp_runtime::utilities {

    namespace {
        bool IsAListPair(lisp_runtime::CellAdaptor  cell) {
            if( cell.Cdr().GetType() == foundation::mem::Cell::Type::AtomType && cell.Cdr().GetAtom() == 2 ){
                return false;
            }
            else{
                return cell.Cdr().GetType() != foundation::mem::Cell::Type::ListType;
            }
        }
    }

    std::string to_str(const foundation::AtomFactory& atom_factory, lisp_runtime::CellAdaptor expression) {
        if (expression.GetHead()->GetType() == foundation::mem::Cell::Type::AtomType) {
            auto result_optional = atom_factory.GetAtomName(expression.GetHead()->atom_);

            return result_optional.has_value() ? result_optional.value() : "NIL";
        } else if (expression.GetHead()->GetType() == foundation::mem::Cell::Type::NumberType) {
            return std::to_string(expression.GetHead()->number_);
        }
        else if (expression.GetHead()->GetType() == foundation::mem::Cell::Type::ListType) {
            std::string result{"("};

            bool add_separator = false;
            for (auto e: expression) {
                CellAdaptor item{e};

                if(add_separator){
                    result += " ";
                    add_separator = false;
                }

                if(IsAListPair(item)){
                    result += to_str(atom_factory, item.Car()) + "." + to_str(atom_factory, item.Cdr())  ;

                    break;
                } else{
                    result += to_str(atom_factory, item.Car());
                }
                add_separator = true;
            }

            result += ")";
            return result;
        }
        else {
            throw std::runtime_error("Error in utilities::Utilities::to_str, Node specifies unknown type");
        }
    }

    bool eql(lisp_runtime::CellAdaptor node1 , lisp_runtime::CellAdaptor node2) {
        if(node1.IsEmpty() && node2.IsEmpty()){
            return true;
        } else{

            if(node1.GetHead()->GetType() != node2.GetHead()->GetType()){
                return false;
            }
            else{
                switch (node1.GetHead()->GetType()) {
                    case foundation::mem::Cell::Type::AtomType:
                        return node1.GetHead()->atom_ == node2.GetHead()->atom_;
                    case foundation::mem::Cell::Type::NumberType:
                        return node1.GetHead()->number_== node2.GetHead()->number_;
                    default:
                        assert(false);
                }
            }
        }
    }


    lisp_runtime::CellAdaptor assoc(lisp_runtime::CellAdaptor variable_name, lisp_runtime::CellAdaptor context){
        if(context.IsEmpty()){
            return context;
        }

        if(context.GetHead()->type_ == foundation::mem::Cell::Type::ListType )
        {
            auto pair = context.Car();
            if(pair.GetHead()->GetType() == foundation::mem::Cell::Type::ListType){

                auto first_pair = context.Car();
                auto key_of_pair = first_pair.Car();

                if(eql(variable_name, key_of_pair)){
                    return first_pair;
                }
                else{
                    auto rest = context.Cdr();

                    return assoc(variable_name, rest);
                }
            } else{
                return {foundation::Defines::NIL()};
            }
        }
        else{
            return {foundation::Defines::NIL()};
        }
    }

} // namespace lisp_runtime::utilities