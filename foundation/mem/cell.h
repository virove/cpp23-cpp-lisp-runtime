#ifndef LISP_RUNTIME_CELL_H
#define LISP_RUNTIME_CELL_H

#include "atom.h"

namespace foundation {

    namespace mem {
        struct Cell {
            enum class Type {
                NO_TYPE_SPECIFIED, AtomType, NumberType, ListType
            };

            Cell() : Cell(Type::NO_TYPE_SPECIFIED) {};

            explicit Cell(Type type) : type_(type), busy_{true}, atom_{0} {};

            [[nodiscard]]Type GetType() const { return type_; }

            [[nodiscard]] bool IsBusy() const {
                return busy_;
            }

            void SetBusy(bool busy) {
                busy_ = busy;
            }

            union {
                struct {
                    const Cell *head_{nullptr};
                    const Cell *tail_{nullptr};
                };
                foundation::ATOM atom_;
                int number_;
            };
            Type type_;
            bool busy_;
        };

        struct CellAtom : Cell {
            CellAtom() = delete;
            explicit CellAtom(foundation::ATOM atom) : Cell(Type::AtomType) {
                atom_ = atom;
            }
        };

        struct CellNumber : Cell {
            CellNumber() = delete;

            explicit CellNumber(int number) : Cell(Type::NumberType) {
                number_ = number;
            }
        };

        struct CellList : Cell {
            CellList() = delete;

            explicit CellList(const Cell *head, const Cell *tail) : Cell(Type::ListType) {
                head_ = head;
                tail_ = tail;
            }
        };
    }
} // namespace foundation

#endif //LISP_RUNTIME_CELL_H
