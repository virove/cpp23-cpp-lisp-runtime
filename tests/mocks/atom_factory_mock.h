#ifndef ATOM_FACTORY_MOCK_H
#define ATOM_FACTORY_MOCK_H

#include <gmock/gmock.h>

#include "atom_factory.h"

class AtomFactoryMock : public foundation::AtomFactory{
public:
    MOCK_METHOD( void, Init,() ,(override));

    MOCK_METHOD( void, Shutdown,() ,(override));

    MOCK_METHOD( std::optional<std::string>, GetAtomName,(foundation::ATOM atom) ,(const override));

    MOCK_METHOD( std::optional<foundation::mem::Cell *>, GetOrCreate,(const std::string &atomname) ,( override));
};


#endif //ATOM_FACTORY_MOCK_H
