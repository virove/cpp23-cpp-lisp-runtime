#ifndef CELL_ADAPTOR_H
#define CELL_ADAPTOR_H

#include <optional>
#include <cassert>

#include "mem/cell.h"

namespace lisp_runtime {

    inline bool IsNIL(std::optional<const foundation::mem::Cell*> node) {
        return !node.has_value() || (node.value()->GetType() == foundation::mem::Cell::Type::AtomType && node.value()->atom_ == 2 );
    }

    class CellAdaptor {
    public:
        /**
         * Intentionally skip declaring explicit
         * @param cell
         */
        CellAdaptor(const foundation::mem::Cell *cell) : cell_{cell}
        {
            assert(cell_ != nullptr);
        }

        CellAdaptor(const CellAdaptor&) = default;

        struct Iterator{
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = foundation::mem::Cell*;
            using pointer           = const foundation::mem::Cell*;  // or also value_type*
            using reference         = const pointer&;  // or also value_type&

            explicit Iterator(const foundation::mem::Cell *node) : cell_(node) {}
            explicit Iterator() : cell_(std::nullopt ) {}
            reference operator*() const { return cell_.value(); }
            pointer operator->() { return cell_.value(); }

            // Prefix
            Iterator& operator++() {
                assert((cell_.has_value() && cell_.value()->GetType() == foundation::mem::Cell::Type::ListType) || IsNIL(cell_));

                if(! IsNIL(cell_)){
                    cell_ = cell_.value()->tail_ ;
                }

                return *this;
            }

            // Postfix increment
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator== (const Iterator& a, const Iterator& b) {
                if(a.cell_ == b.cell_
                   || IsNIL(a.cell_) == IsNIL(b.cell_)
                        ){
                    return true;
                }
                else{
                    if(!a.cell_.has_value() && b.cell_.has_value() && b.cell_.value()->GetType() == foundation::mem::Cell::Type::AtomType && b.cell_.value()->atom_ == foundation::k_NIL_AtomValue
                       || !b.cell_.has_value() && a.cell_.has_value() && a.cell_.value()->GetType() == foundation::mem::Cell::Type::AtomType && a.cell_.value()->atom_ == foundation::k_NIL_AtomValue ){
                        return true;
                    } else{
                        return false;
                    }
                }
            }
            friend bool operator!= (const Iterator& a, const Iterator& b) { return !(a == b); };


            std::optional<const foundation::mem::Cell*> cell_;
        };


        [[nodiscard]] Iterator begin() const { return cell_? Iterator(cell_) : Iterator(); }
        [[nodiscard]] Iterator end()const   { return Iterator(); }

        [[nodiscard]]
        const foundation::mem::Cell * GetHead() const{
            return *begin();
        }

        bool IsEmpty() {
            return cell_ != nullptr && cell_->GetType() == foundation::mem::Cell::Type::AtomType && cell_->atom_ == 2;
        }

        [[nodiscard]]
        CellAdaptor Cdr() const{
            assert(cell_->type_ == foundation::mem::Cell::Type::ListType);

            return {cell_->tail_};
        }

        [[nodiscard]]
        inline foundation::mem::Cell::Type GetType()const{
            return cell_->GetType();
        }

        [[nodiscard]]
        inline foundation::ATOM GetAtom() const {
            return cell_->atom_;
        }


        [[nodiscard]]
        inline lisp_runtime::CellAdaptor Car() const{
            assert(cell_->type_ == foundation::mem::Cell::Type::ListType);

            return cell_->head_;
        }

        [[nodiscard]]
        inline const foundation::mem::Cell *GetThisCell() const {
            return cell_;
        }

    private:
        const foundation::mem::Cell* cell_;
    };

} // lisp_runtime  

#endif //CELL_ADAPTOR_H
