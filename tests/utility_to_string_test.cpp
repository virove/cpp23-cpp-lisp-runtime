#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include <boost/di.hpp>
#include <boost/di/extension/scopes/shared.hpp>


#include <memory>


#include "mem/alloc_operation.h"
#include "simple_cell_factory.h"
#include "atom_factory_impl.h"
#include "mocks/memory_management_mock.h"
#include "foundation.h"


#include "defines.h"
#include "mem/memory_management.h"
#include "mem/allocator_from_arena.h"
#include "mem/simple_memory_management.h"
#include "parser/stream.h"
#include "parser/parser.h"
#include "lisp_runtime.h"
#include "cell_factory.h"
#include "simple_lisp_runtime.h"
#include "foundation_impl.h"
#include "utilities/utilities.h"

namespace di = boost::di;

using ::testing::AtLeast;
using ::testing::Return;

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
                : injector{make_test_injector()}
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

        std::shared_ptr< foundation::AtomFactory>  GetAtomFactory() {
            return atom_factory_;
        }

    private:
        decltype(make_test_injector()) injector;
        std::shared_ptr<foundation::Foundation> foundation_;
        std::unique_ptr<lisp_runtime::CellFactory> cell_factory_;
        std::shared_ptr< foundation::AtomFactory> atom_factory_;
    };
} // noname namespace

TEST(CustomMiniLispText, to_str) {
    TestContext wrapper;
    auto atom_factory = wrapper.GetAtomFactory();
    auto cell_factory = wrapper.CellFactory();

    auto atom_test = atom_factory->GetOrCreate("atom_test");
    auto string_atom_test = lisp_runtime::utilities::to_str(*atom_factory, {atom_test.value()});
    EXPECT_EQ("atom_test", string_atom_test);

    auto number_11 = cell_factory->CreateNumber(11);
    auto string_number_11 = lisp_runtime::utilities::to_str(*atom_factory, {number_11});
    EXPECT_EQ("11", string_number_11);
    auto *node_points_11 = cell_factory->CreateListCell(number_11);

    auto *node_points_atom_test = cell_factory->CreateListCell(atom_test.value(),node_points_11);

    auto string_two_elements_list = lisp_runtime::utilities::to_str(*atom_factory, node_points_atom_test);
    EXPECT_EQ("(atom_test 11)", string_two_elements_list);
}

TEST(CustomMiniLispText, ExprToStrError) {
    TestContext wrapper;
    auto atom_factory = wrapper.GetAtomFactory();
    auto cell_factory = wrapper.CellFactory();

    auto parser = wrapper.CreateParser();

    lisp_runtime::Stream<std::istringstream> stream1("n");
    lisp_runtime::Stream<std::istringstream> stream2("3");

    auto s1 = parser->Parse(&stream1);
    auto s2 = parser->Parse(&stream2);

    auto* new_pair = cell_factory->CreateListCell();
    new_pair->head_ = s1.GetThisCell();
    new_pair->tail_ = s2.GetThisCell();

    auto* new_links_head = cell_factory->CreateListCell();
    new_links_head->head_ = new_pair;
    new_links_head->tail_ =  foundation::Defines::NIL();

    lisp_runtime::Stream<std::istringstream> stream3("k");
    lisp_runtime::Stream<std::istringstream> stream4("4");

    auto s3 = parser->Parse(&stream3);
    auto s4 = parser->Parse(&stream4);


    auto* new_pair1 = cell_factory->CreateListCell();
    new_pair1->head_ = s3.GetThisCell();
    new_pair1->tail_ = s4.GetThisCell();

    auto* new_links_head1 = cell_factory->CreateListCell();
    new_links_head1->head_ = new_pair1;
    new_links_head1->tail_ =  new_links_head;

    auto result = lisp_runtime::utilities::to_str(*atom_factory, {new_links_head1});
    EXPECT_EQ("((k.4) (n.3))", result);
}