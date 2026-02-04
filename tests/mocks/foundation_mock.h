#ifndef FOUNDATION_MOCK_H
#define FOUNDATION_MOCK_H

#include <gmock/gmock.h>
#include "foundation_impl.h"

class FoundationMock : public foundation::Foundation{
public:
    MOCK_METHOD( void, Init,() ,(override));
    MOCK_METHOD( void, Shutdown,() ,(override));
    MOCK_METHOD( std::shared_ptr<foundation::AtomFactory>,GetAtomFactory,() ,(override));
};


#endif //FOUNDATION_MOCK_H
