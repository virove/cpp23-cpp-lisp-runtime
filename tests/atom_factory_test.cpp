#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <boost/di.hpp>

#include "mem/simple_memory_management.h"
#include "atom_factory_impl.h"
#include "mocks/memory_management_mock.h"
#include "defines.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

namespace di = boost::di;

TEST(AtomFactoryTest, InitTest) {
    MemoryManagementMock* p_management_mock = new MemoryManagementMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::MemoryManagement>().to<>([p_management_mock]() {
                return std::shared_ptr<MemoryManagementMock>{p_management_mock};
            }),
            di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>()
    );

    std::unique_ptr<foundation::mem::Cell> allocated_cell = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;
    std::unique_ptr<foundation::mem::Cell> allocated_cell2 = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;
    std::unique_ptr<foundation::mem::Cell> allocated_cell3 = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;

    EXPECT_CALL(*p_management_mock, Allocate()).Times(3)
            .WillOnce(Return(allocated_cell.get()))
            .WillOnce(Return(allocated_cell2.get()))
            .WillOnce(Return(allocated_cell3.get())
            );

    auto atom_factory = injector.create<std::unique_ptr<foundation::AtomFactoryImpl>>();
    EXPECT_TRUE(atom_factory.operator bool());



    atom_factory->Init();
    EXPECT_EQ(allocated_cell->GetType(), foundation::mem::Cell::Type::AtomType);
    EXPECT_EQ(allocated_cell->atom_, foundation::k_ATOM_PROPERTY_NAME);


    EXPECT_EQ(allocated_cell2->GetType(), foundation::mem::Cell::Type::AtomType);
    EXPECT_EQ(allocated_cell2->atom_, foundation::k_NIL_AtomValue);

    EXPECT_EQ(allocated_cell3->GetType(), foundation::mem::Cell::Type::AtomType);
    EXPECT_EQ(allocated_cell3->atom_, foundation::k_T_AtomValue);
}


TEST(AtomFactoryTest, add_new_atom) {
    MemoryManagementMock* p_management_mock = new MemoryManagementMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::MemoryManagement>().to<>([p_management_mock]() {
                return std::shared_ptr<MemoryManagementMock>{p_management_mock};
            }),
            di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>()
    );

    auto atom_factory = injector.create<std::unique_ptr<foundation::AtomFactoryImpl>>();
    EXPECT_TRUE(atom_factory.operator bool());


    std::unique_ptr<foundation::mem::Cell> allocated_cell = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;
    std::unique_ptr<foundation::mem::Cell> allocated_cell2 = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;
    std::unique_ptr<foundation::mem::Cell> allocated_cell3 = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;
    std::unique_ptr<foundation::mem::Cell> allocated_cell4 = std::make_unique<foundation::mem::Cell>(foundation::mem::Cell::Type::NO_TYPE_SPECIFIED) ;

    EXPECT_CALL(*p_management_mock, Allocate()).Times(4)
            .WillOnce(Return(allocated_cell.get()))
            .WillOnce(Return(allocated_cell2.get()))
            .WillOnce(Return(allocated_cell3.get()))
            .WillOnce(Return(allocated_cell4.get())
            );          // #3


    atom_factory->Init();

    EXPECT_TRUE(atom_factory->GetOrCreate("some_new_symbol").value()->atom_ > foundation::k_T_AtomValue);
    EXPECT_EQ(atom_factory->GetOrCreate("some_new_symbol"),allocated_cell4.get());

}


TEST(AtomFactoryTest, query_atom_property_atom) {
    MemoryManagementMock* p_management_mock = new MemoryManagementMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::MemoryManagement>().to<>([p_management_mock]() {
                return std::shared_ptr<MemoryManagementMock>{p_management_mock};
            }),
            di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>()
    );

    auto atom_factory = injector.create<std::unique_ptr<foundation::AtomFactoryImpl>>();
    
    EXPECT_CALL(*p_management_mock, Allocate()).WillRepeatedly(Return(new foundation::mem::Cell() ));
    
    EXPECT_EQ(atom_factory->GetOrCreate("test"), atom_factory->GetOrCreate("test"));
    auto test_property = atom_factory->GetOrCreate("test_property_name");

    auto atom_test = atom_factory->GetOrCreate("test").value()->atom_;
    atom_factory->SetPropertyString(atom_test, test_property.value()->atom_, std::string("test_property_value"));
    auto atom_test_property_value = atom_factory->GetOrCreate("test_property_name").value()->atom_;
    auto actual_property = atom_factory->GetPropertyString(atom_test, atom_test_property_value);

    EXPECT_TRUE(actual_property.has_value());
    EXPECT_EQ(actual_property.value(), "test_property_value");
}

TEST(AtomFactoryTest, query_atom_property_atom_overwrite) {
    MemoryManagementMock* p_management_mock = new MemoryManagementMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::MemoryManagement>().to<>([p_management_mock]() {
                return std::shared_ptr<MemoryManagementMock>{p_management_mock};
            }),
            di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>()
    );

    auto atom_factory = injector.create<std::unique_ptr<foundation::AtomFactoryImpl>>();

    EXPECT_CALL(*p_management_mock, Allocate()).WillRepeatedly(Return(new foundation::mem::Cell() ));
 
    atom_factory->Init();

    EXPECT_EQ(atom_factory->GetOrCreate("test"), atom_factory->GetOrCreate("test"));
    auto test_property = atom_factory->GetOrCreate("test_property_name");

    auto atom_test = atom_factory->GetOrCreate("test").value()->atom_;
    atom_factory->SetPropertyString(atom_test, test_property.value()->atom_, std::string("test_property_value"));
    auto atom_test_property_value = atom_factory->GetOrCreate("test_property_name").value()->atom_;
    auto actual_property = atom_factory->GetPropertyString(atom_test, atom_test_property_value);
    EXPECT_TRUE(actual_property.has_value());
    EXPECT_EQ(actual_property.value(), "test_property_value");

    atom_factory->SetPropertyString(atom_test, test_property.value()->atom_, std::string("NEW_test_property_value"));
    auto actual_property2 = atom_factory->GetPropertyString(atom_test, atom_test_property_value);
    EXPECT_TRUE(actual_property2.has_value());
    EXPECT_EQ(actual_property2.value(), "NEW_test_property_value");
}

TEST(AtomFactoryTest, query_atom_property_atom_failed) {
    MemoryManagementMock* p_management_mock = new MemoryManagementMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::MemoryManagement>().to<>([p_management_mock]() {
                return std::shared_ptr<MemoryManagementMock>{p_management_mock};
            }),
            di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>()
    );

    auto atom_factory = injector.create<std::unique_ptr<foundation::AtomFactoryImpl>>();

    EXPECT_CALL(*p_management_mock, Allocate()).WillRepeatedly(Return(new foundation::mem::Cell() ));

    atom_factory->Init();

    EXPECT_EQ(atom_factory->GetOrCreate("test"), atom_factory->GetOrCreate("test"));
    auto test_property = atom_factory->GetOrCreate("test_property_name");

    atom_factory->SetPropertyString( atom_factory->GetOrCreate("test").value()->atom_, test_property.value()->atom_, std::string("test_property_value"));
    // 1. no such atom 123456789

    auto actual_property = atom_factory->GetPropertyString(123456789,atom_factory->GetOrCreate("test_property_name").value()->atom_);
    EXPECT_FALSE(actual_property.has_value());

    // 2. no such property

    auto actual_property2 = atom_factory->GetPropertyString(atom_factory->GetOrCreate("test").value()->atom_,atom_factory->GetOrCreate("NO_SUCH__property").value()->atom_);
    EXPECT_FALSE(actual_property2.has_value());
}


TEST(AtomFactoryTest, get_atom_name) {
    MemoryManagementMock* p_management_mock = new MemoryManagementMock();

    auto injector = di::make_injector(
            di::bind<foundation::mem::mgr::MemoryManagement>().to<>([p_management_mock]() {
                return std::shared_ptr<MemoryManagementMock>{p_management_mock};
            }),
            di::bind<foundation::AtomFactory>().to<foundation::AtomFactoryImpl>()
    );

    auto atom_factory = injector.create<std::unique_ptr<foundation::AtomFactoryImpl>>();

    EXPECT_CALL(*p_management_mock, Allocate()).WillRepeatedly(Return(new foundation::mem::Cell() ));

    atom_factory->Init();

    EXPECT_EQ(atom_factory->GetOrCreate("test"), atom_factory->GetOrCreate("test"));
    auto atom_name = atom_factory->GetAtomName(atom_factory->GetOrCreate("test").value()->atom_);
    EXPECT_TRUE(atom_name.has_value());
    EXPECT_EQ(atom_name.value(), "test");
}