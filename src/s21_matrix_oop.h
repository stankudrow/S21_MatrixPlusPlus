#ifndef S21_MATRIXPLUSPLUS_SRC_S21_MATRIX_H_
#define S21_MATRIXPLUSPLUS_SRC_S21_MATRIX_H_

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>

// https://stackoverflow.com/questions/14294267/class-template-for-numeric-types
template <typename T,
          typename =
              typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class S21Matrix {
 public:
  using size_type = std::size_t;

 public:
  S21Matrix() noexcept : _rows(0), _cols(0), _matrix(nullptr) {}

  explicit S21Matrix(size_type rows, size_type cols) : S21Matrix() {
    if (!(!rows && !cols)) {
      _matrix = new T[rows * cols]{};  // default value
      _rows = rows;
      _cols = cols;
    }
  }

  explicit S21Matrix(size_type rows, size_type cols, T value)
      : S21Matrix(rows, cols) {
    std::fill(_matrix, _matrix + (rows * cols), value);
  }

  S21Matrix(const S21Matrix& other) : S21Matrix(other._rows, other._cols) {
    std::copy(other._matrix, other._matrix + (_rows * _cols), _matrix);
  }

  S21Matrix(S21Matrix&& other) noexcept
      : _rows(other._rows), _cols(other._cols), _matrix(other._matrix) {
    other._matrix = nullptr;
    other._rows = 0;
    other._cols = 0;
  }

  S21Matrix& operator=(const S21Matrix& other) {
    if (this != &other) {
      S21Matrix copy{other};
      *this = std::move(copy);
    }
    return *this;
  }

  S21Matrix& operator=(S21Matrix&& other) noexcept {
    if (this != &other) {
      std::swap(_rows, other._rows);
      std::swap(_cols, other._cols);
      std::swap(_matrix, other._matrix);
    }
    return *this;
  }

  ~S21Matrix() noexcept { Clear(); }

  // main operations

  bool EqMatrix(const S21Matrix& other) const noexcept {
    if (!IsEqualSize(other)) return false;
    for (size_type i = 0; i < _rows * _cols; ++i) {
      if (std::fabs(_matrix[i] - other._matrix[i]) > _eps) {
        return false;
      }
    }
    return true;
  }

  void SumMatrix(const S21Matrix& other) {
    CheckIsEqualSize(other);
    for (size_type i = 0; i < _rows * _cols; ++i) {
      _matrix[i] += other._matrix[i];
    }
  }

  void SubMatrix(const S21Matrix& other) {
    CheckIsEqualSize(other);
    for (size_type i = 0; i < _rows * _cols; ++i) {
      _matrix[i] -= other._matrix[i];
    }
  }

  void MulNumber(long double num) noexcept {
    for (size_type i = 0; i < _rows * _cols; ++i) {
      _matrix[i] *= num;
    }
  }

  void MulMatrix(const S21Matrix& other) {
    if (_cols != other._rows) {
      std::string errmsg = std::string("this cols (") + std::to_string(_cols) +
                           std::string(" != other rows (") +
                           std::to_string(other._rows) + std::string(")");
      throw std::logic_error(errmsg);
    }
    S21Matrix new_mtx(_rows, other._cols);
    for (size_type r = 0; r < _rows; ++r) {
      for (size_type m = 0; m < _cols; ++m) {
        for (size_type c = 0; c < other._cols; ++c) {
          new_mtx(r, c) += (*this)(r, m) * other(m, c);
        }
      }
    }
    *this = std::move(new_mtx);
  }

  S21Matrix Transpose() const noexcept {
    S21Matrix mtx(_rows, _cols);
    for (size_type r = 0; r < _rows; ++r) {
      for (size_type c = 0; c < _cols; ++c) {
        mtx(c, r) = (*this)(r, c);
      }
    }
    return mtx;
  }

  S21Matrix CalcComplements() const {
    CheckIsSquareMatrix();
    S21Matrix acomps(_cols, _rows);
    for (size_type r = 0; r < _rows; ++r) {
      for (size_type c = 0; c < _cols; ++c) {
        acomps(r, c) = pow(-1, (r + c) % 2) * Minor(r, c);
      }
    }
    return acomps;
  }

  long double Determinant() const {
    CheckIsSquareMatrix();
    if (!_rows) {
      return 1.0;
    }
    if (_rows == 1) {
      return _matrix[0];
    }
    if (_rows == 2) {
      return _matrix[0] * _matrix[_cols + 1] - _matrix[_cols] * _matrix[1];
    }
    long double det = 1.0;
    S21Matrix mtx{*this};
    for (size_type r = 0; r < _rows; ++r) {
      size_type pivot = r;
      for (size_type s = r + 1; s < _rows; ++s) {
        if (std::abs(mtx(s, r)) > std::abs(mtx(pivot, r))) {
          pivot = s;
        }
      }
      if (std::abs(mtx(pivot, r)) < _eps) {
        return 0.0;
      }
      if (pivot != r) {
        for (size_type c = 0; c < _cols; ++c) {
          std::swap(mtx(pivot, c), mtx(r, c));
        }
        det = -det;
      }
      det *= mtx(r, r);
      for (size_type t = r + 1; t < _rows; ++t) {
        long double norm = mtx(t, r) / mtx(r, r);
        for (size_type u = r; u < _rows; ++u) {
          mtx(t, u) -= mtx(r, u) * norm;
        }
      }
    }
    return det;
  }

  S21Matrix InverseMatrix() const {
    long double det = Determinant();
    if (fabs(det) < _eps) {
      throw std::logic_error("The determinant is zero.");
    }
    return Transpose().CalcComplements() * (1. / det);
  }

  // additional operations

  bool IsEqualSize(const S21Matrix& other) const noexcept {
    return _rows == other._rows && _cols == other._cols;
  }

  bool IsSquare() const noexcept { return _rows == _cols; }

  S21Matrix MinorMatrix(size_type row, size_type col) const {
    CheckIsSquareMatrix();
    if (!_rows) {
      throw std::out_of_range("empty matrix");
    }
    S21Matrix mtx(_rows - 1, _cols - 1);
    for (size_type r = 0; r < _rows; ++r) {
      if (r != row) {
        for (size_type c = 0; c < _cols; ++c) {
          if (c != col) {
            mtx(r - (r > row), c - (c > col)) = (*this)(r, c);
          }
        }
      }
    }
    return mtx;
  }

  long double Minor(size_type row, size_type col) const {
    return MinorMatrix(row, col).Determinant();
  }

  std::string Repr() const noexcept {
    std::string repr = std::string("Matrix{");
    repr += "size: " + GetDimString() + ", matrix: " + GetMatrixString();
    repr += "}";
    return repr;
  }

  // getters
  size_type GetRows() const noexcept { return _rows; }
  size_type GetCols() const noexcept { return _cols; }
  std::tuple<size_type, size_type> GetDim() const noexcept {
    return std::make_tuple(_rows, _cols);
  }

  // setters

  void SetRows(size_type rows) {
    if (_rows != rows) {
      S21Matrix mtx{rows, _cols};
      size_type minrow = std::min(_rows, rows);
      for (size_type r = 0; r < minrow; ++r) {
        for (size_type c = 0; c < _cols; ++c) {
          mtx(r, c) = (*this)(r, c);
        }
      }
      *this = std::move(mtx);
    }
  }

  void SetCols(size_type cols) {
    if (_cols != cols) {
      S21Matrix mtx{_rows, cols};
      size_type mincol = std::min(_cols, cols);
      for (size_type r = 0; r < _rows; ++r) {
        for (size_type c = 0; c < mincol; ++c) {
          mtx(r, c) = (*this)(r, c);
        }
      }
      *this = std::move(mtx);
    }
  }

  void SetDim(size_type rows, size_type cols) {
    if (_rows != rows || _cols != cols) {
      if (_rows == rows && _cols != cols) {
        SetCols(cols);
      } else if (_cols == cols && _rows != rows) {
        SetRows(rows);
      } else {
        S21Matrix mtx{rows, cols};
        size_type minrow = std::min(_rows, rows);
        size_type mincol = std::min(_cols, cols);
        for (size_type r = 0; r < minrow; ++r) {
          for (size_type c = 0; c < mincol; ++c) {
            mtx(r, c) = (*this)(r, c);
          }
        }
        *this = std::move(mtx);
      }
    }
  }

  // operators

  explicit operator bool() const noexcept { return _matrix != nullptr; }
  bool operator==(const S21Matrix& other) const { return EqMatrix(other); }

  S21Matrix operator+(const S21Matrix& other) const {
    S21Matrix mtx(*this);
    mtx.SumMatrix(other);
    return mtx;
  }

  S21Matrix& operator+=(const S21Matrix& other) {
    SumMatrix(other);
    return *this;
  }

  S21Matrix operator-(const S21Matrix& other) const {
    S21Matrix mtx(*this);
    mtx.SubMatrix(other);
    return mtx;
  }

  S21Matrix& operator-=(const S21Matrix& other) {
    SubMatrix(other);
    return *this;
  }

  S21Matrix operator*(long double num) const {
    S21Matrix mtx(*this);
    mtx.MulNumber(num);
    return mtx;
  }

  friend S21Matrix operator*(long double number,
                             const S21Matrix& matrix) noexcept {
    return matrix * number;
  }

  S21Matrix& operator*=(long double num) {
    MulNumber(num);
    return *this;
  }

  S21Matrix operator*(const S21Matrix& other) const {
    S21Matrix mtx(*this);
    mtx.MulMatrix(other);
    return mtx;
  }

  S21Matrix& operator*=(const S21Matrix& other) {
    MulMatrix(other);
    return *this;
  }

  const T& operator()(size_type row, size_type col) const {
    return GetElement(row, col);
  }

  T& operator()(size_type row, size_type col) {
    return const_cast<T&>(GetElement(row, col));
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const S21Matrix& mtx) noexcept {
    os << mtx.Repr();
    return os;
  }

 private:
  void CheckIndexUpperBound(size_type idx, size_type upper) const {
    if (idx >= upper) {
      std::string errmsg = "index ";
      errmsg += std::to_string(idx);
      errmsg += " >= ";
      errmsg += std::to_string(upper);
      throw std::out_of_range(errmsg);
    }
  }

  void CheckIsEqualSize(const S21Matrix& other) const {
    if (!IsEqualSize(other)) {
      std::string errmsg = std::string("Size mismatch: this ") +
                           GetDimString() + std::string(" != other ") +
                           other.GetDimString();
      throw std::logic_error(errmsg);
    }
  }

  void CheckIsSquareMatrix() const {
    if (!IsSquare()) {
      std::string errmsg = std::string("rows = ") + std::to_string(_rows) +
                           std::string(" is not equal to cols = ") +
                           std::to_string(_cols);
      throw std::logic_error(errmsg);
    }
  }

  void Clear() noexcept {
    delete[] _matrix;
    _rows = 0;
    _cols = 0;
    _matrix = nullptr;
  }

  std::string GetDimString() const noexcept {
    std::string dimstr = "(";
    dimstr += std::to_string(_rows);
    dimstr += ", ";
    dimstr += std::to_string(_cols);
    dimstr += ")";
    return dimstr;
  }

  const T& GetElement(size_type row, size_type col) const {
    CheckIndexUpperBound(row, _rows);
    CheckIndexUpperBound(col, _cols);
    return _matrix[row * _cols + col];
  }

  std::string GetMatrixString() const noexcept {
    if (!_matrix) {
      return std::string("[]");
    }
    if (!_rows || !_cols) {
      return std::string("[...]");
    }
    std::string repr = std::string("[");
    for (size_type r = 0; r < _rows; ++r) {
      repr += "[";
      for (size_type c = 0; c < _cols - 1; ++c) {
        repr += std::to_string((*this)(r, c)) + ", ";
      }
      repr += std::to_string((*this)(r, _cols - 1)) + "]";
      if (r != _rows - 1) repr += "; ";
    }
    repr += "]";
    return repr;
  }

  size_type _rows, _cols;
  T* _matrix;
  double _eps{std::numeric_limits<double>::epsilon()};
};

#endif  // S21_MATRIXPLUSPLUS_SRC_S21_MATRIX_H_
