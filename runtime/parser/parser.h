#ifndef PARSER_H
#define PARSER_H

#include <list>

#include "defines.h"
#include "cell_factory.h"
#include "cell_adaptor.h"
#include "stream.h"

namespace lisp_runtime {

    template <typename StdStream>
    class Parser {
    public:
        explicit Parser( CellFactory* node_factory) :  node_factory_{node_factory}
        {};

        CellAdaptor Parse(Stream<StdStream>* stream);
    private:

        using ParseResult = std::tuple<bool, CellAdaptor>;
        using ParserFunction = ParseResult (Parser::* )(Stream<StdStream>& stream);

        ParseResult ParseExpression(Stream<StdStream>& stream_  );
        ParseResult ParseExpressionList(Stream<StdStream>& stream  );
        ParseResult ParseExpressionAtom(Stream<StdStream>& stream);
        ParseResult ParseExpressionNumber(Stream<StdStream>& stream);

        std::list<ParserFunction> expressions_parsers{&Parser::ParseExpressionAtom, &Parser::ParseExpressionNumber,
                                                      &Parser::ParseExpressionList}  ;

        Parser::ParseResult ParseTail(Stream<StdStream>& stream_);

        CellAdaptor cons(CellFactory* pFactory, CellAdaptor pNode, CellAdaptor pNode1);

        void skipSpaces(Stream<StdStream>& stream_);

        [[nodiscard]] bool IsSymbolCharacter(unsigned char ch) const;

        Parser::ParseResult readTail(Stream<StdStream>& stream_);

        CellFactory *node_factory_;
    };




    template <typename StdStream>
    CellAdaptor Parser<StdStream>::Parse(Stream<StdStream>* stream) {
        skipSpaces(*stream);
        for(auto parse_function : expressions_parsers){
            auto [success, result] = (this->*parse_function)(*stream);
            if(success){
                return result;
            }
        }
        throw std::runtime_error("Failed to parse expression");
    }


    template <typename StdStream>
    typename Parser<StdStream>::ParseResult Parser<StdStream>::ParseExpression(Stream<StdStream>& stream_) {


        if(stream_.get() == '(' ){
            if( stream_.peek() == ')'){
                stream_.get();
                return Parser::ParseResult(true, foundation::Defines::NIL());
            }

            auto result = cons(node_factory_, std::get<1>(ParseExpression(node_factory_, stream_)),
                               std::get<1>(ParseTail(node_factory_,stream_)));

            return {true, result};
        }
        else{
            auto [status, result] = ParseExpressionNumber(stream_);
            if(status){
                return {true, result};
            }
            auto [status2, result2] = ParseExpressionAtom(node_factory_, stream_);
            if(status2){
                return {true, result2};
            }

            throw std::runtime_error("Failed to parse expression");
        }
    }
    template <typename StdStream>
    typename Parser<StdStream>::ParseResult Parser<StdStream>::ParseTail(Stream<StdStream>& stream_) {
        if( stream_.peek() == ')'){
            stream_.get();
            return Parser::ParseResult(true , foundation::Defines::NIL());
        }
        else{
            if( stream_.peek() == '('){

                if(stream_.peek() == ')'){
                    stream_.get();

                    auto result = cons(node_factory_, CellAdaptor{foundation::Defines::NIL()}, std::get<1>(ParseTail(node_factory_, stream_)));
                    return {true,result};
                }
                else{
                    auto result = cons(node_factory_,
                                       cons(node_factory_, std::get<1>(ParseExpression(node_factory_, stream_)), std::get<1>(ParseTail(node_factory_, stream_) )),
                                       std::get<1>(ParseTail(node_factory_, stream_))
                    );
                    return {true, result};
                }
            }
            else{
                CellAdaptor result(foundation::Defines::NIL());

                auto [status1, result1] = ParseExpressionNumber(stream_);
                if(status1){
                    result = result1;
                }
                auto [status2, result2] = ParseExpressionAtom(stream_);
                if(status2){
                    result = result2;
                }
                if(!(status1 || status2)){
                    throw std::runtime_error("Failed to parse expression");
                }

                return {true, cons(node_factory_,result, std::get<1>(ParseTail(node_factory_, stream_)))};
            }
        }
    }

    template <typename Stream>
    bool Parser<Stream>::IsSymbolCharacter(
            unsigned char ch) const {
        return ch == '=' || ch == '<' || ch == '>'
               || ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
               isalpha(ch);
    }

    template <typename StdStream>
    typename Parser<StdStream>::ParseResult Parser<StdStream>::ParseExpressionAtom(Stream<StdStream>& stream_) {
        std::string symbol;

        const auto peeked_ch = stream_.get();
        if(IsSymbolCharacter(static_cast<unsigned char>(peeked_ch))){
            symbol.push_back(static_cast<char>(peeked_ch));

            while(true){
                auto ch = static_cast<unsigned char>(stream_.get());
                if(stream_.eof()) {
                    break;
                }

                if( IsSymbolCharacter(ch) || std::isdigit(ch)){
                    symbol.push_back(static_cast<char>(ch));
                }
                else{
                    stream_.putback(ch);
                    break;
                }
            }
            return { true, CellAdaptor(node_factory_->GetOrCreate(symbol))};
        }
        else{
            stream_.putback(peeked_ch);

            return {false, CellAdaptor(foundation::Defines::NIL())};
        }
    }

    template <typename Stream>
    CellAdaptor Parser<Stream>::cons(CellFactory *factory, CellAdaptor head, CellAdaptor tail) {
        auto* node = factory->CreateListCell(foundation::Defines::NIL());
        node->head_ = head.GetHead();
        node->tail_ = tail.GetHead();

        return CellAdaptor(node);
    }

    template <typename StdStream>
    typename Parser<StdStream>::ParseResult Parser<StdStream>::ParseExpressionNumber(Stream<StdStream>& stream_) {
        std::string number;

        skipSpaces(stream_);
        if(stream_.eof()){
            return {  false , CellAdaptor(foundation::Defines::NIL())};
        }

        auto ch = static_cast<unsigned char>( stream_.get());
        if(std::isdigit(ch)){
            number.push_back(static_cast< char>(ch));



            while(true){
                auto next_ch = static_cast<unsigned char>( stream_.get());
                if(stream_.eof()){
                    break;
                }
                if( std::isdigit(next_ch)){
                    number.push_back(static_cast< char>(next_ch));
                }
                else{
                    stream_.putback(next_ch);
                    break;
                }
            }

            try{
                return {true, CellAdaptor{node_factory_->CreateNumber(std::stoi(number))}};
            }
            catch (const std::invalid_argument& e) {
                throw std::runtime_error("Error of parsing nnumber in expression. Number:" + number + ". "+ e.what());
            } catch (const std::out_of_range& e) {
                throw std::runtime_error("Error of parsing nnumber in expression. Number:" + number + ". "+ e.what());
            }
        }
        else{
            stream_.putback(ch);

            return {false, CellAdaptor( foundation::Defines::NIL())};
        }
    }

    template <typename StdStream>
    void Parser<StdStream>::skipSpaces(Stream<StdStream>& stream_) {
        while(!stream_.eof()){
            const auto ch = stream_.get();
            if(! isspace(static_cast<int >(ch))){
                stream_.putback(ch);
                break;
            }
        }
    }

    template <typename StdStream>
    typename Parser<StdStream>::ParseResult Parser<StdStream>::ParseExpressionList(Stream<StdStream>& stream_) {
        skipSpaces(stream_);

        if(stream_.eof()){
            return {false, CellAdaptor( foundation::Defines::NIL())};
        }

        auto ch = stream_.get();
        if(ch == '('){
            return readTail(stream_);
        }
        else{
            stream_.putback(ch);

            return {false, CellAdaptor(foundation::Defines::NIL())};
        }
    }

    template <typename StdStream>
    typename Parser<StdStream>::ParseResult Parser<StdStream>::readTail(Stream<StdStream>& stream_) {
        skipSpaces(stream_);

        if(stream_.eof()){
            throw std::runtime_error("Unexpected EOF");
        }

        auto ch = stream_.get();
        if(ch == ')'){

            return { true, CellAdaptor(foundation::Defines::NIL())};
        } else{
            stream_.putback(ch);
        }

        auto [number_success,number] = ParseExpressionNumber(stream_);
        if(!number_success){
            auto [ symbol_success ,symbol]= ParseExpressionAtom(stream_);

            if(!symbol_success){
                auto [list_success, list] = ParseExpressionList(stream_);

                if(list_success){
                    auto [tail_success, tail] = readTail(stream_);

                    auto result = cons(node_factory_, list, tail);
                    return { true, CellAdaptor(result) };
                } else{
                    return {false, CellAdaptor(foundation::Defines::NIL()) };
                }
            }
            else{
                auto [tail_success, tail] = readTail(stream_);

                auto result = cons(node_factory_, symbol, tail );
                return {true, result};
            }
        } else{
            auto [tail_success, tail] = readTail(stream_);

            return { true, CellAdaptor(cons(node_factory_,number, tail))};
        }

    }


} // lisp_runtime

#endif //PARSER_H
