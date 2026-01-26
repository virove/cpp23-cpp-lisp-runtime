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

using ::testing::_;
namespace di = boost::di;

using ::testing::InSequence;

TEST(FoundationTest, InitTest) {
    AtomFactoryMock* atom_factory_mock = new AtomFactoryMock;
    MemoryManagementMock* memory_management_mock = new MemoryManagementMock();

    auto injector = di::make_injector(
            di::bind<foundation::AtomFactory>().in(di::singleton).to(
                    [atom_factory_mock](){ return std::shared_ptr<foundation::AtomFactory>(atom_factory_mock);}
                    ),

            di::bind<foundation::mem::mgr::MemoryManagement>().in(di::singleton).to(
                    [memory_management_mock](){ return std::shared_ptr<foundation::mem::mgr::MemoryManagement>(memory_management_mock);}
                    )
    );

    auto foundation = injector.create<std::unique_ptr<foundation::FoundationImpl>>();

    {
        InSequence s;

        EXPECT_CALL(*memory_management_mock, Init());
        EXPECT_CALL(*atom_factory_mock, Init());
    }

    foundation->Init();

    {
        InSequence s;

        EXPECT_CALL(*memory_management_mock, Shutdown());
        EXPECT_CALL(*atom_factory_mock, Shutdown());
    }

    foundation->Shutdown();

}