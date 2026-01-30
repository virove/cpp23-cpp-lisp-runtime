#include <stdexcept>
#include "utilities.h"
#include "mem/cell.h"
#include "cell_factory.h"
#include "atom_factory.h"

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
} // namespace lisp_runtime::utilities