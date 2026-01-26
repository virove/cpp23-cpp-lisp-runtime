#ifndef FOUNDATION_H
#define FOUNDATION_H

namespace foundation {

    class Foundation {
    public:
        virtual ~Foundation()  = default;
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
    };

} // foundation

#endif //FOUNDATION_H
