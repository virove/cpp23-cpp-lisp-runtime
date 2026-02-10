#ifndef FOUNDATION_H
#define FOUNDATION_H

namespace foundation {

    class AtomFactory;

    class Foundation {
    public:
        virtual ~Foundation()  = default;
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
        virtual std::shared_ptr<AtomFactory> GetAtomFactory() = 0;
    };

} // foundation

#endif //FOUNDATION_H
