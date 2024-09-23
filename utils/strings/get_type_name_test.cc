#include "utils/strings/get_type_name.h"

#include <gtest/gtest.h>

#include <vector>

namespace ghost {

namespace {
struct MyAnonymousStruct {};
}  // namespace

namespace internal {
struct MyInternalStruct {};
}  // namespace internal

TEST(GetTypeName, BasicTypes) {
  EXPECT_EQ(GetTypeName<int>(), "int");
  EXPECT_EQ(GetTypeName<double>(), "double");
  EXPECT_EQ(GetTypeName<long>(), "long");  // NOLINT
  EXPECT_EQ(GetTypeName<unsigned>(), "unsigned int");
}

TEST(GetTypeName, PointerTypes) {
  EXPECT_EQ(GetTypeName<int*>(), "int *");
  EXPECT_EQ(GetTypeName<double*>(), "double *");
  EXPECT_EQ(GetTypeName<long*>(), "long *");  // NOLINT
  EXPECT_EQ(GetTypeName<unsigned*>(), "unsigned int *");
}

TEST(GetTypeName, CVQualifiedTypes) {
  EXPECT_EQ(GetTypeName<const int>(), "const int");
  EXPECT_EQ(GetTypeName<const int*>(), "const int *");
  EXPECT_EQ(GetTypeName<const int* const>(), "const int *const");

  EXPECT_EQ(GetTypeName<volatile int>(), "volatile int");
  EXPECT_EQ(GetTypeName<volatile int*>(), "volatile int *");
  EXPECT_EQ(GetTypeName<volatile int* const>(), "volatile int *const");
}

TEST(GetTypeName, ReferencesTypes) {
  EXPECT_EQ(GetTypeName<int&>(), "int &");
  EXPECT_EQ(GetTypeName<int&&>(), "int &&");
}

TEST(GetTypeName, CustomStruct) {
  EXPECT_EQ(GetTypeName<internal::MyInternalStruct>(),
            "ghost::internal::MyInternalStruct");

  EXPECT_EQ(GetTypeName<MyAnonymousStruct>(),
            "ghost::(anonymous namespace)::MyAnonymousStruct");
}

}  // namespace ghost
