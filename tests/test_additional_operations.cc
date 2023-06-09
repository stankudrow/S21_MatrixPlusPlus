#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

TEST(MatrixAdditionalOperations, IsEqualSize) {
  ASSERT_TRUE(S21Matrix<char>().IsEqualSize(S21Matrix<char>(0, 0)));
  ASSERT_TRUE(S21Matrix<char>(1, 1).IsEqualSize(S21Matrix<char>(1, 1)));
  ASSERT_FALSE(S21Matrix<char>(1, 1).IsEqualSize(S21Matrix<char>(2, 1)));
  ASSERT_FALSE(S21Matrix<char>(1, 1).IsEqualSize(S21Matrix<char>(1, 2)));
}

TEST(MatrixAdditionalOperations, IsSquare) {
  ASSERT_TRUE(S21Matrix<char>(0, 0).IsSquare());
  ASSERT_TRUE(S21Matrix<char>(1, 1).IsSquare());
  ASSERT_FALSE(S21Matrix<char>(1, 2).IsSquare());
  ASSERT_FALSE(S21Matrix<char>(2, 1).IsSquare());
}

TEST(MatrixAdditionalOperations, MatrixAsBoolean) {
  ASSERT_FALSE(static_cast<bool>(S21Matrix<char>()));
  ASSERT_TRUE(static_cast<bool>(S21Matrix<char>(1, 1)));
}

TEST(MatrixAdditionalOperations, MinorMatrix) {
  size_t rows = 2, cols = 2;
  double value = 1;
  S21Matrix<double> mtx(rows, cols, value);
  S21Matrix<double> res(rows - 1, cols - 1, value);

  ASSERT_EQ(mtx.MinorMatrix(0, 0), res);
  ASSERT_EQ(mtx.MinorMatrix(0, 1), res);
  ASSERT_EQ(mtx.MinorMatrix(1, 0), res);
  ASSERT_EQ(mtx.MinorMatrix(1, 1), res);

  rows = 3, cols = 3, value = 2;
  mtx = S21Matrix<double>(rows, cols, value);
  res = S21Matrix<double>(rows - 1, cols - 1, value);

  ASSERT_EQ(mtx.MinorMatrix(0, 0), res);
  ASSERT_EQ(mtx.MinorMatrix(0, 1), res);
  ASSERT_EQ(mtx.MinorMatrix(0, 2), res);
  ASSERT_EQ(mtx.MinorMatrix(1, 0), res);
  ASSERT_EQ(mtx.MinorMatrix(1, 1), res);
  ASSERT_EQ(mtx.MinorMatrix(1, 2), res);
  ASSERT_EQ(mtx.MinorMatrix(2, 0), res);
  ASSERT_EQ(mtx.MinorMatrix(2, 1), res);
  ASSERT_EQ(mtx.MinorMatrix(2, 2), res);
}

TEST(MatrixAdditionalOperations, Minor) {
  size_t rows = 2, cols = 2;
  double res, value;
  res = value = 1;
  S21Matrix<double> mtx(rows, cols, value);

  ASSERT_EQ(mtx.Minor(0, 0), value);
  ASSERT_EQ(mtx.Minor(0, 1), value);
  ASSERT_EQ(mtx.Minor(1, 0), value);
  ASSERT_EQ(mtx.Minor(1, 1), value);

  rows = 3, cols = 3, value = 2;
  mtx = S21Matrix<double>(rows, cols, value);
  res = 0;

  ASSERT_EQ(mtx.Minor(0, 0), res);
  ASSERT_EQ(mtx.Minor(0, 1), res);
  ASSERT_EQ(mtx.Minor(0, 2), res);
  ASSERT_EQ(mtx.Minor(1, 0), res);
  ASSERT_EQ(mtx.Minor(1, 1), res);
  ASSERT_EQ(mtx.Minor(1, 2), res);
  ASSERT_EQ(mtx.Minor(2, 0), res);
  ASSERT_EQ(mtx.Minor(2, 1), res);
  ASSERT_EQ(mtx.Minor(2, 2), res);
}

TEST(MatrixAdditionalOperations, ReprOut) {
  S21Matrix<double> mtx;
  std::string repr;

  mtx = S21Matrix<double>(0, 0);
  repr = std::string("Matrix{size: (0, 0), matrix: []}");
  std::cout << mtx << std::endl;
  ASSERT_TRUE(mtx.Repr() == repr);

  mtx = S21Matrix<double>(1, 0);
  repr = std::string("Matrix{size: (1, 0), matrix: [...]}");
  std::cout << mtx << std::endl;
  ASSERT_TRUE(mtx.Repr() == repr);

  mtx = S21Matrix<double>(0, 1);
  repr = std::string("Matrix{size: (0, 1), matrix: [...]}");
  std::cout << mtx << std::endl;
  ASSERT_TRUE(mtx.Repr() == repr);

  mtx = S21Matrix<double>(1, 1);
  repr = std::string("Matrix{size: (1, 1), matrix: [[0.000000]]}");
  std::cout << mtx << std::endl;
  ASSERT_TRUE(mtx.Repr() == repr);

  mtx = S21Matrix<double>(1, 2);
  repr = std::string("Matrix{size: (1, 2), matrix: [[0.000000, 0.000000]]}");
  std::cout << mtx << std::endl;
  ASSERT_TRUE(mtx.Repr() == repr);

  mtx = S21Matrix<double>(2, 1);
  repr = std::string("Matrix{size: (2, 1), matrix: [[0.000000]; [0.000000]]}");
  std::cout << mtx << std::endl;
  ASSERT_TRUE(mtx.Repr() == repr);

  mtx = S21Matrix<double>(2, 2);
  repr = "Matrix{size: (2, 2), matrix: ";
  repr += "[[0.000000, 0.000000]; [0.000000, 0.000000]]}";
  std::cout << mtx << std::endl;
  ASSERT_TRUE(mtx.Repr() == repr);
}
