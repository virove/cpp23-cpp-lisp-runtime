#include <gtest/gtest.h>

#include <boost/di.hpp>
#include <boost/di/extension/scopes/shared.hpp>

#include "defines.h"
#include "cell_adaptor.h"
#include "mem/memory_management.h"
#include "mem/allocator_from_arena.h"
#include "mem/allocator_from_garbage_collected.h"
#include "mocks/garbage_collector_acceptor_mock.h"
#include "mem/simple_memory_management.h"
#include "parser/stream.h"
#include "parser/parser.h"
#include "lisp_runtime.h"
#include "cell_factory.h"
#include "simple_lisp_runtime.h"
#include "simple_cell_factory.h"
#include "atom_factory_impl.h"
#include "foundation_impl.h"

namespace di = boost::di;

using ::testing::_;

constexpr std::size_t kPreallocatedMemory_CellNumber = 1000;

namespace {
    auto make_test_injector() {
        return di::make_injector<di::extension::shared_config>(
                di::bind<std::size_t>().named(foundation::mem::mgr::number_of_preallocated_nodes).to(
                        static_cast<std::size_t>(kPreallocatedMemory_CellNumber)),
                di::bind<lisp_runtime::LispRuntime>().to<lisp_runtime::SimpleLispRuntime>()                 .in(di::extension::shared),
                di::bind<lisp_runtime::CellFactory>().to<lisp_runtime::SimpleCellFactory>(),
                di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>()                       .in(di::extension::shared),
                di::bind<foundation::mem::mgr::AllocatorFromArena>,
                di::bind<foundation::Foundation>().to<foundation::FoundationImpl>()                         .in(di::extension::shared),
                di::bind<foundation::mem::mgr::MemoryManagement>().to<foundation::mem::mgr::SimpleMemoryManagement>(),
                di::bind<foundation::Defines>                                                               .in(di::extension::shared)

                );
    }


    class TestContext{
    public:
        using Std_StringStreamType = decltype(std::istringstream("Aa123"));

        TestContext()
                        :
                        injector{make_test_injector()}

        {
            foundation_ =  injector.create<std::shared_ptr<foundation::Foundation>>();
            cell_factory_ = injector.create<std::unique_ptr<lisp_runtime::CellFactory>>();
            atom_factory_ = injector.create<std::shared_ptr< foundation::AtomFactory>>();
            foundation_->Init();
        }
        ~TestContext(){
            foundation_->Shutdown();
        }

        auto CreateParser(){
            return std::make_unique<lisp_runtime::Parser< Std_StringStreamType>>(cell_factory_.get()) ;
        }

        lisp_runtime::CellFactory* CellFactory() const {
            return cell_factory_.get();
        }

    private:
        decltype(make_test_injector()) injector;
        std::shared_ptr<foundation::Foundation> foundation_;
        std::unique_ptr<lisp_runtime::CellFactory> cell_factory_;
        std::shared_ptr< foundation::AtomFactory> atom_factory_;
    };

} // noname namespace


TEST(ParserTest, readSymbol) {
    TestContext wrapper;

    std::istringstream stream("Aa123");
    lisp_runtime::Stream<TestContext::Std_StringStreamType> stream1(&stream);

    auto parser =   wrapper.CreateParser();

    auto result = parser->Parse(&stream1);

    EXPECT_TRUE(!result.IsEmpty());
    EXPECT_TRUE(result.GetHead()->GetType() == foundation::mem::Cell::Type::AtomType);
    EXPECT_EQ(result.GetHead()->atom_,  wrapper.CellFactory()->GetOrCreate("Aa123")->atom_);
}


TEST(ParserTest, readNumber) {
    TestContext wrapper;

    std::istringstream stream("1234");
    lisp_runtime::Stream<TestContext::Std_StringStreamType> stream1(&stream);

    auto parser =   wrapper.CreateParser();
    auto result = parser->Parse(&stream1);

    EXPECT_TRUE(!result.IsEmpty());
    EXPECT_TRUE(result.GetHead()->GetType() == foundation::mem::Cell::Type::NumberType);
    EXPECT_EQ(result.GetHead()->number_,  1234);
}

TEST(ParserTest, readList1) {
    TestContext wrapper;

    std::istringstream stream("( abc )");
    lisp_runtime::Stream<TestContext::Std_StringStreamType> stream1(&stream);

    auto parser =   wrapper.CreateParser();
    auto result = parser->Parse(&stream1);

    EXPECT_TRUE(!result.IsEmpty());
    EXPECT_TRUE(result.GetHead()->head_->GetType() ==  foundation::mem::Cell::Type::AtomType);
    EXPECT_EQ(result.GetHead()->head_->atom_, wrapper.CellFactory()->GetOrCreate("abc")->atom_);
}


TEST(ParserTest, readListNestedNIL) {
    TestContext wrapper;

    std::istringstream stream( "( () )");
    lisp_runtime::Stream<TestContext::Std_StringStreamType> stream1(&stream);

    auto parser =   wrapper.CreateParser();
    auto result = parser->Parse(&stream1);

    EXPECT_TRUE(!result.IsEmpty());

    EXPECT_TRUE(result.GetHead()->GetType() == foundation::mem::Cell::Type::ListType);
    auto nested  =  result.GetHead()->head_;
    EXPECT_TRUE(nested->GetType() == foundation::mem::Cell::Type::AtomType);
    EXPECT_TRUE(nested->atom_ == 2);

    auto nested_tail = result.GetHead()->tail_;
    EXPECT_TRUE(nested_tail->GetType() == foundation::mem::Cell::Type::AtomType);
    EXPECT_TRUE(nested_tail->atom_ == 2);
}

TEST(ParserTest, readList) {
    TestContext wrapper;

    std::istringstream stream( "( abc 1234 )");
    lisp_runtime::Stream<TestContext::Std_StringStreamType> stream1(&stream);

    auto parser =   wrapper.CreateParser();
    auto result = parser->Parse(&stream1);

    EXPECT_TRUE(!result.IsEmpty());
    EXPECT_TRUE(result.GetHead()->head_->GetType() == foundation::mem::Cell::Type::AtomType);
    EXPECT_EQ(result.GetHead()->head_->atom_, wrapper.CellFactory()->GetOrCreate("abc")->atom_);

    auto second_item = result.GetHead()->tail_->head_;
    EXPECT_TRUE(second_item->GetType() == foundation::mem::Cell::Type::NumberType);
    EXPECT_EQ(second_item->number_, 1234);
}



TEST(ParserTest, readbadFormat2) {
    TestContext wrapper;

    std::string list = "a)";
    std::istringstream stream(list);
    lisp_runtime::Stream<TestContext::Std_StringStreamType> stream1(&stream);

    auto parser =   wrapper.CreateParser();
    auto result = parser->Parse(&stream1);

    EXPECT_EQ(result.GetHead()->GetType(),foundation::mem::Cell::Type::AtomType );
    EXPECT_EQ(result.GetHead()->atom_, wrapper.CellFactory()->GetOrCreate("a")->atom_);
}
