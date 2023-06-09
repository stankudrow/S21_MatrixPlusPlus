#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

TEST(EmptyMatrixUnderTest, EmptyMatrixBehaviour) {
  S21Matrix<long double> empty;
  S21Matrix<long double> other(0, 0);

  ASSERT_EQ(empty.GetRows(), 0);
  ASSERT_EQ(empty.GetCols(), 0);
  ASSERT_EQ(empty.GetDim(), std::make_tuple(0, 0));

  // equality, [] == []
  ASSERT_EQ(empty, other);

  // [] + [] = []
  ASSERT_EQ(empty + other, other);

  // [] - [] = []
  ASSERT_EQ(empty - other, other);

  // [] * 2 = [] + [] = []
  ASSERT_EQ(empty * 2, other);

  // [] * 2 = [] + [] = []
  ASSERT_EQ(2 * empty, other);

  // [] * [] = [] - may be implementation-defined
  ASSERT_EQ(empty * other, other);

  // [].Transpose() = [] (nothing leads to nothing)
  ASSERT_EQ(empty.Transpose(), other);

  // There is no room to get a minor
  ASSERT_THROW(empty.MinorMatrix(0, 0), std::out_of_range);
  // No MinorMatrix, no Minor as a determinant
  ASSERT_THROW(empty.Minor(0, 0), std::out_of_range);
  // Hovewer, a matrix of algebraic complements can be computed - it is 0x0.
  ASSERT_EQ(empty.CalcComplements(), other);

  // The determinant of an empty matrix is 1, seriously!
  ASSERT_EQ(empty.Determinant(), 1);

  // The inverse is empty because of the underlying implementations
  ASSERT_EQ(empty.InverseMatrix(), other);
}
