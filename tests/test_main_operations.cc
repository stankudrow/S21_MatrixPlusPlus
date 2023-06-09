#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

TEST(MatrixMainOperations, EqMatrices) {
  size_t rows = 2, cols = 3;
  S21Matrix<double> mtx1 = S21Matrix<double>(rows, cols);
  S21Matrix<double> mtx2 = S21Matrix<double>(cols, rows);
  S21Matrix<double> mtx3 = S21Matrix<double>(rows, cols);

  ASSERT_FALSE(mtx1 == mtx2);
  ASSERT_TRUE(mtx1 == mtx1);

  size_t row = 0, col = 0;
  double value = 0.3;

  mtx1(row, col) = value;
  ASSERT_FALSE(mtx1 == mtx3);
  mtx3(row, col) = value;
  ASSERT_TRUE(mtx1 == mtx3);

  row = 0, col = 1, value = -0.9;
  mtx1(row, col) = value;
  ASSERT_FALSE(mtx1 == mtx3);
  mtx3(row, col) = value;
  ASSERT_TRUE(mtx1 == mtx3);

  row = 1, col = 0, value = 1.6;
  mtx1(row, col) = value;
  ASSERT_FALSE(mtx1 == mtx3);
  mtx3(row, col) = value;
  ASSERT_TRUE(mtx1 == mtx3);

  row = 1, col = 1, value = -3.2;
  mtx1(row, col) = value;
  ASSERT_FALSE(mtx1 == mtx3);
  mtx3(row, col) = value;
  ASSERT_TRUE(mtx1 == mtx3);

  ASSERT_EQ(S21Matrix<short>(), S21Matrix<short>());
}

TEST(MatrixMainOperations, SumMatrices) {
  size_t rows = 2, cols = 2;
  S21Matrix<double> mtx1 = S21Matrix<double>(rows, cols);
  S21Matrix<double> mtx2 = S21Matrix<double>(rows, cols);
  S21Matrix<double> ans = S21Matrix<double>(rows, cols);

  int r00 = 1.09, r01 = -2.51, r10 = 3.67, r11 = -4.21;
  mtx1(0, 0) = r00, mtx1(1, 1) = r11;
  mtx2(0, 1) = r01, mtx2(1, 0) = r10;
  ans(0, 0) = r00, ans(0, 1) = r01, ans(1, 0) = r10, ans(1, 1) = r11;
  ASSERT_EQ(mtx1 + mtx2, ans);

  mtx1 += mtx2;
  ASSERT_EQ(mtx1, ans);

  ASSERT_THROW(mtx1 + S21Matrix<double>(rows, cols + 1, 1), std::logic_error);
  ASSERT_THROW(mtx1 + S21Matrix<double>(rows + 1, cols, 1), std::logic_error);
}

TEST(MatrixMainOperations, SubMatrices) {
  size_t rows = 2, cols = 2;
  S21Matrix<double> mtx1 = S21Matrix<double>(rows, cols);
  S21Matrix<double> mtx2 = S21Matrix<double>(rows, cols);
  S21Matrix<double> ans = S21Matrix<double>(rows, cols);

  int r00 = 1.09, r01 = -2.51, r10 = 3.67, r11 = -4.21;
  mtx1(0, 0) = r00, mtx1(1, 1) = r11;
  mtx2(0, 1) = r01, mtx2(1, 0) = r10;
  ans(0, 0) = r00, ans(0, 1) = -r01, ans(1, 0) = -r10, ans(1, 1) = r11;
  ASSERT_EQ(mtx1 - mtx2, ans);

  mtx1 -= mtx2;
  ASSERT_EQ(mtx1, ans);

  ASSERT_THROW(mtx1 - S21Matrix<double>(rows, cols + 1, 1), std::logic_error);
  ASSERT_THROW(mtx1 - S21Matrix<double>(rows + 1, cols, 1), std::logic_error);
}

TEST(MatrixMainOperations, MulMatrixByNumber) {
  size_t rows = 2, cols = 2;
  S21Matrix<double> mtx = S21Matrix<double>(rows, cols);
  S21Matrix<double> ans = S21Matrix<double>(rows, cols);

  double r00 = -1, r01 = 1, r10 = -2, r11 = -2;
  double factor = 3;

  mtx(0, 0) = r00, mtx(0, 1) = r01, mtx(1, 0) = r10, mtx(1, 1) = r11;
  ans(0, 0) = factor * r00;
  ans(0, 1) = factor * r01;
  ans(1, 0) = factor * r10;
  ans(1, 1) = factor * r11;

  ASSERT_EQ(mtx * factor, ans);
  ASSERT_EQ(factor * mtx, ans);

  mtx *= factor;
  ASSERT_EQ(mtx, ans);
}

TEST(MatrixMainOperations, MulMatrixByMatrix) {
  size_t rows = 2, cols = 2;
  S21Matrix<double> mtx1(rows, cols);
  S21Matrix<double> mtx2(rows, cols);
  S21Matrix<double> ans(rows, cols);

  mtx1(0, 0) = 1, mtx1(0, 1) = 2, mtx1(1, 0) = 3, mtx1(1, 1) = 4;
  mtx2(0, 0) = 4, mtx2(0, 1) = 3, mtx2(1, 0) = 2, mtx2(1, 1) = 1;
  ans(0, 0) = 1. * 4 + 2 * 2;
  ans(0, 1) = 1. * 3 + 2 * 1;
  ans(1, 0) = 3. * 4 + 4 * 2;
  ans(1, 1) = 3. * 3 + 4 * 1;
  ASSERT_EQ(mtx1 * mtx2, ans);

  mtx1 *= mtx2;
  ASSERT_EQ(mtx1, ans);

  ASSERT_THROW(mtx1 * S21Matrix<double>(cols + 1, cols), std::logic_error);
}

TEST(MatrixMainOperations, Transpose) {
  S21Matrix<double> mtx;
  S21Matrix<double> trans = mtx.Transpose();

  ASSERT_EQ(mtx, trans);

  mtx = S21Matrix<double>(1, 1, 21);
  trans = mtx.Transpose();
  ASSERT_EQ(mtx, trans);

  int rows = 2, cols = 2;
  mtx = S21Matrix<double>(rows, cols);
  S21Matrix<double> ans = S21Matrix<double>(rows, cols);

  mtx(0, 0) = 1, mtx(0, 1) = -2, mtx(1, 0) = 3, mtx(1, 1) = -4;
  ans(0, 0) = 1, ans(0, 1) = 3, ans(1, 0) = -2, ans(1, 1) = -4;
  trans = mtx.Transpose();
  ASSERT_EQ(trans, ans);
}

TEST(MatrixMainOperations, CalcComplements) {
  ASSERT_THROW(S21Matrix<double>(0, 1).CalcComplements(), std::logic_error);
  ASSERT_THROW(S21Matrix<double>(1, 0).CalcComplements(), std::logic_error);
  ASSERT_THROW(S21Matrix<double>(1, 2).CalcComplements(), std::logic_error);
  ASSERT_THROW(S21Matrix<double>(2, 1).CalcComplements(), std::logic_error);

  size_t rows = 2, cols = 2;
  double value = 1;
  S21Matrix mtx(rows, cols, value);
  S21Matrix res(rows, cols, value);

  res(0, 1) = -value, res(1, 0) = -value;
  ASSERT_EQ(mtx.CalcComplements(), res);

  rows = 3, cols = 3;
  mtx = S21Matrix<double>(rows, cols);
  mtx(0, 0) = 1, mtx(0, 1) = 2, mtx(0, 2) = 1;
  mtx(1, 0) = 3, mtx(1, 1) = 2, mtx(1, 2) = 4;
  mtx(2, 0) = 1, mtx(2, 1) = 1, mtx(2, 2) = 3;

  res = S21Matrix<double>(rows, cols);
  res(0, 0) = 2, res(0, 1) = -5, res(0, 2) = 1;
  res(1, 0) = -5, res(1, 1) = 2, res(1, 2) = 1;
  res(2, 0) = 6, res(2, 1) = -1, res(2, 2) = -4;

  ASSERT_EQ(mtx.CalcComplements(), res);
}

TEST(MatrixMainOperations, Determinant) {
  S21Matrix<double> mtx;

  ASSERT_EQ(S21Matrix<double>().Determinant(), 1);
  ASSERT_EQ(S21Matrix<double>(1, 1).Determinant(), 0);
  ASSERT_EQ(S21Matrix<double>(2, 2).Determinant(), 0);
  ASSERT_EQ(S21Matrix<double>(3, 3).Determinant(), 0);

  mtx = S21Matrix<double>(2, 2);
  mtx(0, 0) = 3, mtx(0, 1) = 2, mtx(1, 0) = 1, mtx(1, 1) = 1;
  ASSERT_LT(std::fabs(mtx.Determinant() - 1),
            std::numeric_limits<double>::epsilon());

  mtx = S21Matrix<double>(3, 3);
  mtx(0, 0) = 1, mtx(0, 1) = -2, mtx(0, 2) = 3;
  mtx(1, 0) = -4, mtx(1, 1) = 5, mtx(1, 2) = -6;
  mtx(2, 0) = 7, mtx(2, 1) = -8, mtx(2, 2) = 9;
  ASSERT_LT(mtx.Determinant(), std::numeric_limits<double>::epsilon());

  mtx = S21Matrix<double>(4, 4);
  mtx(0, 0) = 0, mtx(0, 1) = -1, mtx(0, 2) = 2, mtx(0, 3) = -3;
  mtx(1, 0) = 4, mtx(1, 1) = -5, mtx(2, 2) = 6, mtx(1, 3) = -7;
  mtx(2, 0) = 8, mtx(2, 1) = -9, mtx(2, 2) = 10, mtx(2, 3) = -11;
  mtx(3, 0) = 12, mtx(3, 1) = -13, mtx(2, 2) = 14, mtx(3, 3) = -15;
  ASSERT_LT(mtx.Determinant(), std::numeric_limits<double>::epsilon());
}

TEST(MatrixMainOperations, InverseMatrix) {
  S21Matrix<double> mtx;
  S21Matrix<double> inv;
  S21Matrix<double> res;
  size_t rows, cols;

  ASSERT_EQ(S21Matrix<double>().InverseMatrix(), S21Matrix<double>());

  // the determinant is zero
  ASSERT_THROW(S21Matrix<double>(1, 1).InverseMatrix(), std::logic_error);

  // the determinant is zero
  ASSERT_THROW(S21Matrix<double>(1, 3).InverseMatrix(), std::logic_error);

  // the determinant is zero
  ASSERT_THROW(S21Matrix<double>(3, 1).InverseMatrix(), std::logic_error);

  rows = 2, cols = 2;
  mtx = S21Matrix<double>(rows, cols);
  mtx(0, 0) = 1, mtx(0, 1) = -2, mtx(1, 0) = 3, mtx(1, 1) = -4;

  res = S21Matrix<double>(rows, cols);
  res(0, 0) = -4, res(0, 1) = 2, res(1, 0) = -3, res(1, 1) = 1;
  res.MulNumber(1. / 2);

  inv = mtx.InverseMatrix();

  ASSERT_EQ(inv, res);
  ASSERT_LT(std::fabs(inv.Determinant() - 0.5),
            std::numeric_limits<double>::epsilon());
}
