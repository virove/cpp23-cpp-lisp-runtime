#ifndef STREAM_H
#define STREAM_H

#include <sstream>
#include <list>

namespace lisp_runtime {

    template <typename StdStream>
    class Stream{
    public:
        explicit  Stream(const std::string& param) : std_stream_{  param }
        {};

        unsigned char  get();

        void putback(unsigned char);

        [[nodiscard]] bool eof()const{
            bool stream_eof = std_stream_.eof();
            bool buffer_empty = buffer.empty();

            return stream_eof
                   && buffer_empty;
        };

    private:
        using Buffer = std::list<unsigned char>;
        Buffer buffer;
        StdStream std_stream_;
    };

    template<typename StdStream>
    void Stream<StdStream>::putback(unsigned char ch) {
        buffer.push_back(static_cast<char >(ch));
    }

    template<typename StdStream>
    unsigned char Stream<StdStream>::get() {
        if(!buffer.empty()){
            auto ch = buffer.back();
            buffer.pop_back();

            return ch;
        }
        else {
            return std_stream_.get();
        };
    }


} // lisp_runtime  

#endif //STREAM_H
