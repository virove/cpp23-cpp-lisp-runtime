#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <memory>

#include "lisp_runtime.h"
#include "cell_adaptor.h"
#include "atom_factory.h"

namespace lisp_runtime::utilities {

    std::string to_str(const foundation::AtomFactory& atom_factory, lisp_runtime::CellAdaptor expression);

} // namespace lisp_runtime::utilities

#endif //UTILITIES_H
