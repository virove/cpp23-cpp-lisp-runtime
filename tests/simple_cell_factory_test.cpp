#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <boost/di.hpp>

#include "mem/alloc_operation.h"
#include "simple_cell_factory.h"
#include "atom_factory_impl.h"
#include "mocks/memory_management_mock.h"
#include "mocks/atom_factory_mock.h"
#include "boost/di/extension/scopes/shared.hpp"

using ::testing::AtLeast;
using ::testing::Return;

namespace di = boost::di;



TEST(SimpleCellFactoryTest, create_number_cell) {
    auto* management_mock = new MemoryManagementMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::MemoryManagement>().to([management_mock](){ return  std::shared_ptr<foundation::mem::mgr::MemoryManagement>{management_mock}; }),
            di::bind<foundation::AtomFactory>().to<AtomFactoryMock>().in(di::extension::shared),
            di::bind<lisp_runtime::CellFactory>().to<lisp_runtime::SimpleCellFactory>()
    );

    auto allocated_cell = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;

    EXPECT_CALL(*management_mock, Allocate())
    .WillOnce(Return(allocated_cell.get() ));

    auto cell_factory = injector.create<std::unique_ptr<lisp_runtime::CellFactory>>();

    const foundation::mem::Cell*   cell = cell_factory->CreateNumber(1) ;


    EXPECT_EQ(cell->GetType(), foundation::mem::Cell::Type::NumberType);
    EXPECT_EQ(cell->number_, 1);
}

TEST(SimpleCellFactoryTest, create_list_cell) {
    auto* management_mock = new MemoryManagementMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::MemoryManagement>().to([management_mock](){ return  std::shared_ptr<foundation::mem::mgr::MemoryManagement>{management_mock}; }),
            di::bind<foundation::AtomFactory>().to<AtomFactoryMock>().in(di::extension::shared),
            di::bind<lisp_runtime::CellFactory>().to<lisp_runtime::SimpleCellFactory>()
    );

    auto allocated_cell = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;
    auto allocated_cell2 = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;

    EXPECT_CALL(*management_mock, Allocate()).Times(2)
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