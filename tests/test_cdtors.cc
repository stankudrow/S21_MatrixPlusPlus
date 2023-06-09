#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

TEST(MatrixConstructors, DefaultConstructor) {
  S21Matrix<long int> mtx{};
  ASSERT_EQ(mtx.GetCols(), 0);
  ASSERT_EQ(mtx.GetRows(), 0);
}

TEST(MatrixConstructors, ParametrisedConstructor) {
  size_t rows = 0, cols = 0;
  S21Matrix<double> mtx1{rows, cols};
  ASSERT_EQ(mtx1.GetCols(), 0);
  ASSERT_EQ(mtx1.GetRows(), 0);

  rows = 1, cols = 1;
  double value = 0;
  S21Matrix<double> mtx2{rows, cols};
  ASSERT_EQ(mtx2.GetCols(), cols);
  ASSERT_EQ(mtx2.GetRows(), rows);
  ASSERT_EQ(mtx2(0, 0), value);

  rows = 2, cols = 2, value = 42;
  S21Matrix mtx3{rows, cols, value};
  ASSERT_EQ(mtx3.GetCols(), cols);
  ASSERT_EQ(mtx3.GetRows(), rows);
  ASSERT_EQ(mtx3(0, 0), value);
  ASSERT_EQ(mtx3(0, 1), value);
  ASSERT_EQ(mtx3(1, 0), value);
  ASSERT_EQ(mtx3(1, 1), value);

  ASSERT_NO_THROW(S21Matrix<int>(0, 1));
  ASSERT_NO_THROW(S21Matrix<int>(1, 0));
}

TEST(MatrixConstructors, CopyConstructor) {
  size_t rows = 0, cols = 0;
  S21Matrix<double> mtx{rows, cols};
  S21Matrix<double> mtx_copy{mtx};

  ASSERT_EQ(mtx_copy.GetCols(), mtx.GetCols());
  ASSERT_EQ(mtx_copy.GetRows(), mtx.GetRows());

  rows = 2, cols = 3;
  double value = 42;
  mtx = S21Matrix(rows, cols, value);
  mtx_copy = mtx;
  ASSERT_EQ(mtx, mtx_copy);
}

TEST(MatrixConstructors, MoveConstructor) {
  size_t rows = 1, cols = 1;
  int value = 21;
  S21Matrix<int> mtx = S21Matrix(rows, cols, value);
  S21Matrix<int> mtx_move = S21Matrix(std::move(mtx));

  ASSERT_EQ(mtx_move.GetCols(), cols);
  ASSERT_EQ(mtx_move.GetRows(), rows);
  ASSERT_EQ(mtx_move(0, 0), value);

  ASSERT_EQ(mtx.GetCols(), 0);
  ASSERT_EQ(mtx.GetRows(), 0);

  mtx = std::move(mtx_move);
  ASSERT_EQ(mtx.GetCols(), cols);
  ASSERT_EQ(mtx.GetRows(), rows);
  ASSERT_EQ(mtx(0, 0), value);
}
