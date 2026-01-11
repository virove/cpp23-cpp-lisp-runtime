#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <boost/di.hpp>

#include "mem/alloc_operation.h"
#include "simple_cell_factory.h"
#include "atom_factory_impl.h"
#include "mocks/memory_management_mock.h"

using ::testing::AtLeast;
using ::testing::Return;

namespace di = boost::di;

class AllocOperationMock : public foundation::mem::mgr::AllocOperation{
public:
    ~AllocOperationMock() override = default;

    MOCK_METHOD(void, Init,(),(override));
    MOCK_METHOD(foundation::mem::Cell *, Allocate, (), (override));
};

TEST(SimpleCellFactoryTest, create_number_cell) {
    auto* alloc_operation_mock = new AllocOperationMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::AllocOperation>().to([alloc_operation_mock](){ return  std::unique_ptr<foundation::mem::mgr::AllocOperation>{alloc_operation_mock}; }),
            di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>(),
            di::bind<foundation::MemoryManagement>().to<MemoryManagementMock>()
    );

    auto allocated_cell = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;

    EXPECT_CALL(*alloc_operation_mock, Allocate())
    .WillOnce(Return(allocated_cell.get() ));

    auto cell_factory = injector.create<std::unique_ptr<lisp_runtime::SimpleCellFactory>>();

    const foundation::mem::Cell*   cell = cell_factory->CreateNumber(1) ;


    EXPECT_EQ(cell->GetType(), foundation::mem::Cell::Type::NumberType);
    EXPECT_EQ(cell->number_, 1);
}

TEST(SimpleCellFactoryTest, create_list_cell) {
    auto* alloc_operation_mock = new AllocOperationMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::AllocOperation>().to([alloc_operation_mock](){ return  std::unique_ptr<foundation::mem::mgr::AllocOperation>{alloc_operation_mock}; }),
            di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>(),
            di::bind<foundation::MemoryManagement>().to<MemoryManagementMock>()
    );

    auto allocated_cell = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;
    auto allocated_cell2 = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;

    EXPECT_CALL(*alloc_operation_mock, Allocate()).Times(2)
            .WillOnce(Return(allocated_cell.get()))
            .WillOnce(Return(allocated_cell2.get() )
            );

    auto cell_factory = injector.create<std::unique_ptr<lisp_runtime::SimpleCellFactory>>();

    const foundation::mem::Cell*   cell = cell_factory->CreateListCell(foundation::Defines::T(), foundation::Defines::NIL()) ;


    EXPECT_EQ(cell->GetType(), foundation::mem::Cell::Type::ListType);
    EXPECT_EQ(cell->head_, foundation::Defines::T());
    EXPECT_EQ(cell->tail_, foundation::Defines::NIL());


    const foundation::mem::Cell*   cell2 = cell_factory->CreateListCell(foundation::Defines::T(), foundation::Defines::NIL()) ;

    EXPECT_EQ(cell2, allocated_cell2.get());
    EXPECT_EQ(cell2->GetType(), foundation::mem::Cell::Type::ListType);
    EXPECT_EQ(cell2->head_, foundation::Defines::T());
    EXPECT_EQ(cell2->tail_, foundation::Defines::NIL());
}