#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

TEST(MatrixAccessors, GetRowsCols) {
  size_t rows = 0, cols = 0;
  S21Matrix<int> mtx = S21Matrix<int>(rows, cols);

  ASSERT_EQ(mtx.GetRows(), rows);
  ASSERT_EQ(mtx.GetCols(), cols);

  rows = 2, cols = 3;
  mtx = S21Matrix<int>(rows, cols);

  ASSERT_EQ(mtx.GetRows(), rows);
  ASSERT_EQ(mtx.GetCols(), cols);
}

TEST(MatrixAccessors, GetSetElement) {
  size_t rows = 2, cols = 2;
  S21Matrix<int> mtx(rows, cols);

  mtx(0, 0) = -1, mtx(0, 1) = 1;
  mtx(1, 0) = 2, mtx(1, 1) = -2;

  ASSERT_EQ(mtx(0, 0), -1);
  ASSERT_EQ(mtx(0, 1), 1);
  ASSERT_EQ(mtx(1, 0), 2);
  ASSERT_EQ(mtx(1, 1), -2);

  ASSERT_THROW(mtx(0, 2), std::out_of_range);
  ASSERT_THROW(mtx(2, 0), std::out_of_range);

  const S21Matrix<int> const_mtx = S21Matrix<int>(1, 1);
  ASSERT_EQ(const_mtx(0, 0), 0);  // const get
}

TEST(MatrixAccessors, GetDimTuple) {
  ASSERT_TRUE(S21Matrix<short>().GetDim() == std::make_tuple(0, 0));
  ASSERT_TRUE(S21Matrix<short>(1, 2).GetDim() == std::make_tuple(1, 2));
  ASSERT_TRUE(S21Matrix<short>(2, 1).GetDim() == std::make_tuple(2, 1));
}

TEST(MatrixAccessors, SetRows) {
  S21Matrix<int> mtx(0, 0);

  mtx.SetRows(0);

  mtx = S21Matrix<int>(1, 1, 1);
  mtx.SetRows(2);
  ASSERT_EQ(mtx(0, 0), 1);
  ASSERT_EQ(mtx(1, 0), 0);

  mtx.SetRows(1);
  ASSERT_EQ(mtx(0, 0), 1);
  ASSERT_EQ(mtx.GetRows(), 1);
}

TEST(MatrixAccessors, SetCols) {
  S21Matrix<int> mtx(0, 0);

  mtx.SetCols(0);

  mtx = S21Matrix(1, 1, 1);
  mtx.SetCols(2);
  ASSERT_EQ(mtx(0, 0), 1);
  ASSERT_EQ(mtx(0, 1), 0);

  mtx.SetCols(1);
  ASSERT_EQ(mtx(0, 0), 1);
  ASSERT_EQ(mtx.GetCols(), 1);
}

TEST(MatrixAccessors, SetDim) {
  S21Matrix<double> mtx(0, 0);

  mtx.SetDim(1, 1);
  ASSERT_EQ(mtx.GetDim(), std::make_tuple(1, 1));
  ASSERT_EQ(mtx(0, 0), 0);

  mtx(0, 0) = 1;
  mtx.SetDim(1, 2);
  ASSERT_EQ(mtx(0, 0), 1);
  ASSERT_EQ(mtx(0, 1), 0);

  mtx(0, 1) = 2;
  mtx.SetDim(2, 2);
  ASSERT_EQ(mtx(0, 0), 1);
  ASSERT_EQ(mtx(0, 1), 2);
  ASSERT_EQ(mtx(1, 0), 0);
  ASSERT_EQ(mtx(1, 1), 0);

  mtx(1, 0) = 3, mtx(1, 1) = 4;
  mtx.SetDim(3, 3);
  ASSERT_EQ(mtx(0, 0), 1);
  ASSERT_EQ(mtx(0, 1), 2);
  ASSERT_EQ(mtx(0, 2), 0);
  ASSERT_EQ(mtx(1, 0), 3);
  ASSERT_EQ(mtx(1, 1), 4);
  ASSERT_EQ(mtx(1, 2), 0);
  ASSERT_EQ(mtx(2, 0), 0);
  ASSERT_EQ(mtx(2, 1), 0);
  ASSERT_EQ(mtx(2, 2), 0);

  mtx.SetDim(2, 2);
  ASSERT_EQ(mtx(0, 0), 1);
  ASSERT_EQ(mtx(0, 1), 2);
  ASSERT_EQ(mtx(1, 0), 3);
  ASSERT_EQ(mtx(1, 1), 4);

  mtx.SetDim(1, 1);
  ASSERT_EQ(mtx(0, 0), 1);
  ASSERT_EQ(mtx.GetRows(), 1);
  ASSERT_EQ(mtx.GetCols(), 1);

  mtx.SetDim(0, 0);
  ASSERT_EQ(mtx.GetRows(), 0);
  ASSERT_EQ(mtx.GetCols(), 0);
}
