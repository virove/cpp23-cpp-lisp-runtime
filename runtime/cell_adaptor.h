#ifndef CELL_ADAPTOR_H
#define CELL_ADAPTOR_H

#include <cassert>

namespace lisp_runtime {

    inline bool IsNIL(const std::optional<const foundation::mem::Cell*>& node) {
        return !node.has_value()
        || (node.value()->GetType() == foundation::mem::Cell::Type::AtomType && node.value()->atom_ == 2 );
    }

    class CellAdaptor {

    public:
        CellAdaptor(const foundation::mem::Cell *cell) : cell_{cell}
        {}

        CellAdaptor(const CellAdaptor&) = default;

        struct Iterator{
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = foundation::mem::Cell*;
            using pointer           = const foundation::mem::Cell*;  // or also value_type*
            using reference         = const pointer&;  // or also value_type&

            explicit Iterator(const foundation::mem::Cell *node) : node_(node) {}
            explicit Iterator() : node_(std::nullopt ) {}
            reference operator*() const { return node_.value(); }
            pointer operator->() { return node_.value(); }

            // Prefix
            Iterator& operator++() {
                assert((node_.has_value() && node_.value()->GetType() == foundation::mem::Cell::Type::ListType) || IsNIL(node_));

                if(! IsNIL(node_)){
                    node_ = node_.value()->tail_ ;
                }

                return *this;
            }


            // Postfix increment
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator== (const Iterator& a, const Iterator& b) {
                if(a.node_ == b.node_
                   || IsNIL(a.node_) == IsNIL(b.node_)
                        ){
                    return true;
                }
                else{
                    if(!a.node_.has_value() && b.node_.has_value() && b.node_.value()->GetType() == foundation::mem::Cell::Type::AtomType && b.node_.value()->atom_ == foundation::k_NIL_AtomValue
                       || !b.node_.has_value() && a.node_.has_value() && a.node_.value()->GetType() == foundation::mem::Cell::Type::AtomType && a.node_.value()->atom_ == foundation::k_NIL_AtomValue ){
                        return true;
                    } else{
                        return false;
                    }
                }
            }
            friend bool operator!= (const Iterator& a, const Iterator& b) { return !(a == b); };


            std::optional<const foundation::mem::Cell*> node_;
        };


        Iterator begin() const { return cell_? Iterator(cell_) : Iterator(); }
        Iterator end()const   { return Iterator(); }

        const foundation::mem::Cell * GetHead() const{
            return *begin();
        }

        bool IsEmpty() {
            return cell_ != nullptr && cell_->GetType() == foundation::mem::Cell::Type::AtomType && cell_->atom_ == 2;
        }
    private:
        const foundation::mem::Cell* cell_;
    };

} // lisp_runtime  

#endif //CELL_ADAPTOR_H
