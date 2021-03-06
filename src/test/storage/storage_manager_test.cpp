#include <memory>
#include <string>
#include <vector>

#include "../base_test.hpp"
#include "gtest/gtest.h"

#include "../lib/storage/storage_manager.hpp"
#include "../lib/storage/table.hpp"

namespace opossum {

class StorageStorageManagerTest : public BaseTest {
 protected:
  void SetUp() override {
    auto& sm = StorageManager::get();
    auto t1 = std::make_shared<Table>();
    auto t2 = std::make_shared<Table>(4);

    sm.add_table("first_table", t1);
    sm.add_table("second_table", t2);
  }
};

TEST_F(StorageStorageManagerTest, GetTable) {
  auto& sm = StorageManager::get();
  auto t3 = sm.get_table("first_table");
  auto t4 = sm.get_table("second_table");
  EXPECT_THROW(sm.get_table("third_table"), std::exception);
}

TEST_F(StorageStorageManagerTest, AddTable) {
  auto& sm = StorageManager::get();
  auto t = std::make_shared<Table>();
  EXPECT_THROW(sm.add_table("first_table", t), std::exception);
}

TEST_F(StorageStorageManagerTest, DropTable) {
  auto& sm = StorageManager::get();
  sm.drop_table("first_table");
  EXPECT_THROW(sm.get_table("first_table"), std::exception);
  EXPECT_THROW(sm.drop_table("first_table"), std::exception);
}

TEST_F(StorageStorageManagerTest, ResetTable) {
  StorageManager::get().reset();
  auto& sm = StorageManager::get();
  EXPECT_THROW(sm.get_table("first_table"), std::exception);
}

TEST_F(StorageStorageManagerTest, DoesNotHaveTable) {
  auto& sm = StorageManager::get();
  EXPECT_EQ(sm.has_table("third_table"), false);
}

TEST_F(StorageStorageManagerTest, HasTable) {
  auto& sm = StorageManager::get();
  EXPECT_EQ(sm.has_table("first_table"), true);
}

TEST_F(StorageStorageManagerTest, TableNames) {
  auto& sm = StorageManager::get();
  std::vector<std::string> expected_values{"first_table", "second_table"};
  EXPECT_EQ(sm.table_names(), expected_values);
}

TEST_F(StorageStorageManagerTest, Print) {
  auto& sm = StorageManager::get();
  auto t1 = sm.get_table("first_table");
  t1->add_column("test_column", "string");

  std::string expected_result =
      "Database contains 2 tables.\n"
      "first_table with 1 column and 0 rows.\n"
      "second_table with 0 columns and 0 rows.\n";

  std::ostringstream stream;
  sm.print(stream);

  EXPECT_EQ(stream.str(), expected_result);
}
}  // namespace opossum
