/*************************************************************************************************
 * Tests for tkrzw_file_std.h
 *
 * Copyright 2020 Google LLC
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License.  You may obtain a copy of the License at
 *     https://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.  See the License for the specific language governing permissions
 * and limitations under the License.
 *************************************************************************************************/

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "tkrzw_file.h"
#include "tkrzw_file_test_common.h"
#include "tkrzw_file_std.h"
#include "tkrzw_file_util.h"
#include "tkrzw_lib_common.h"
#include "tkrzw_sys_config.h"

using namespace testing;

// Main routine
int main(int argc, char** argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class StdFileTest : public CommonFileTest<tkrzw::StdFile> {};

TEST_F(StdFileTest, Attributes) {
  tkrzw::StdFile file;
  EXPECT_FALSE(file.IsMemoryMapping());
  EXPECT_TRUE(file.IsAtomic());
}

TEST_F(StdFileTest, EmptyFile) {
  EmptyFileTest();
}

TEST_F(StdFileTest, SimpleRead) {
  SimpleReadTest();
}

TEST_F(StdFileTest, SimpleWrite) {
  SimpleWriteTest();
}

TEST_F(StdFileTest, ReallocWrite) {
  ReallocWriteTest();
}

TEST_F(StdFileTest, ImplicitClose) {
  ImplicitCloseTest();
}

TEST_F(StdFileTest, OpenOptions) {
  OpenOptionsTest();
}

TEST_F(StdFileTest, OrderedThread) {
  OrderedThreadTest();
}

TEST_F(StdFileTest, RandomThread) {
  RandomThreadTest();
}

TEST_F(StdFileTest, FileReader) {
  FileReaderTest();
}

TEST_F(StdFileTest, FlatRecord) {
  FlatRecordTest();
}

TEST_F(StdFileTest, Rename) {
  RenameTest();
}

TEST_F(StdFileTest, CriticalSection) {
  tkrzw::TemporaryDirectory tmp_dir(true, "tkrzw-");
  const std::string file_path = tmp_dir.MakeUniquePath();
  tkrzw::StdFile file;
  EXPECT_EQ(-1, file.Lock());
  EXPECT_EQ(-1, file.Unlock());
  EXPECT_EQ(tkrzw::Status::SUCCESS, file.Open(file_path, true, tkrzw::File::OPEN_TRUNCATE));
  EXPECT_EQ(tkrzw::Status::SUCCESS, file.Write(0, "abc", 3));
  EXPECT_EQ(3, file.Lock());
  EXPECT_EQ(tkrzw::Status::SUCCESS, file.WriteInCriticalSection(2, "xyz", 3));
  EXPECT_EQ(tkrzw::Status::SUCCESS, file.WriteInCriticalSection(5, "123", 3));
  char buf[8];
  EXPECT_EQ(tkrzw::Status::SUCCESS, file.ReadInCriticalSection(0, buf, 8));
  EXPECT_EQ("abxyz123", std::string(buf, 8));
  EXPECT_EQ(8, file.Unlock());
  EXPECT_EQ(tkrzw::Status::SUCCESS, file.Close());
}

// END OF FILE