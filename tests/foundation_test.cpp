#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/di.hpp>
#include <iostream>

#include "app.h"
#include "atom_factory_impl.h"
#include "mem/simple_memory_management.h"
#include "mocks/memory_management_mock.h"
#include "foundation_impl.h"
#include "mocks/atom_factory_mock.h"

namespace di = boost::di;

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

TEST(FoundationTest, InitTest) {
    AtomFactoryMock* atom_factory_mock = new AtomFactoryMock;
    MemoryManagementMock* memory_management_mock = new MemoryManagementMock();
    auto allocated_cell = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;
    auto allocated_cell2 = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;

    auto injector = di::make_injector(
            di::bind<foundation::AtomFactory>().to(
                    [atom_factory_mock](){ return std::shared_ptr<foundation::AtomFactory>(atom_factory_mock);}
                    ),


            di::bind<foundation::mem::mgr::MemoryManagement>().to<>(
                    [memory_management_mock](){ return std::shared_ptr<foundation::mem::mgr::MemoryManagement>(memory_management_mock);}
                    ),
            di::bind<foundation::Foundation>().in(di::singleton).to<foundation::FoundationImpl>()
    );

    auto foundation = injector.create<std::unique_ptr<foundation::FoundationImpl>>();

    {
        InSequence s;

        EXPECT_CALL(*memory_management_mock, Init());
        EXPECT_CALL(*atom_factory_mock, Init());
        EXPECT_CALL(*atom_factory_mock, GetOrCreate("NIL")).WillOnce(Return(allocated_cell.get()));
        EXPECT_CALL(*atom_factory_mock, GetOrCreate("T")).WillOnce(Return(allocated_cell2.get()));
    }

    foundation->Init();

    {
        InSequence s;

        EXPECT_CALL(*atom_factory_mock, Shutdown());
        EXPECT_CALL(*memory_management_mock, Shutdown());
    }

    foundation->Shutdown();

}