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

    MOCK_METHOD( std::optional<foundation::mem::Cell *>, GetProperty,(foundation::ATOM atom, foundation::ATOM property_name) ,( const override));

    MOCK_METHOD( std::optional<foundation::AtomFactory::FuncType>, GetFunctionProperty,(foundation::ATOM atom, foundation::ATOM property_name) ,( const override));

    MOCK_METHOD(void, SetProperty, (foundation::ATOM atom, foundation::ATOM name, foundation::mem::Cell * value) , (  override));

    MOCK_METHOD( void, SetFunctionProperty,(foundation::ATOM atom, foundation::ATOM name,
            foundation::mem::Cell *(*fun)(const foundation::mem::Cell *)) ,(  override));

    MOCK_METHOD( std::optional<std::string>, GetPropertyString,(foundation::ATOM atom, foundation::ATOM name) ,(  const override));

    MOCK_METHOD( void, SetPropertyString,(foundation::ATOM atom, foundation::ATOM name,
            const std::string &value) ,(  override));
};


#endif //ATOM_FACTORY_MOCK_H
