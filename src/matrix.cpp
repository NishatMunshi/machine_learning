#include "../include/matrix.hpp"
#include <iostream>
#include <ctime>


Matrix::Matrix(size_t _rows, size_t _cols)
        : m_rows(_rows), m_cols(_cols) {
    if (_rows == 0 and _cols == 0) {
        throw std::invalid_argument("cannot create 0x0 matrix");
    }

    for (size_t i = 0; i < _rows; ++i) {
        m_data.emplace_back();
        for (size_t j = 0; j < _cols; ++j) {
            m_data.back().emplace_back(0.0);
        }
    }
}


bool Matrix::addable(Matrix const &_other) const {
    return m_rows == _other.m_rows and m_cols == _other.m_cols;
}


bool Matrix::multiplyable(Matrix const &_other) const {
    return m_cols == _other.m_rows;
}


void Matrix::print() const {
    for (const auto &row: m_data) {
        std::cout << '[';
        for (size_t j = 0; j < row.size(); ++j) {
            std::cout << row.at(j) << (j == row.size() - 1 ? "" : " ");
        }
        std::cout << "]\n";
    }
}


double &Matrix::at(const size_t _row, const size_t _col) {
    return m_data.at(_row).at(_col);
}


double Matrix::at(const size_t _row, const size_t _col) const {
    return m_data.at(_row).at(_col);
}


Matrix Matrix::operator+(Matrix const &_other) const {
    if (not addable(_other)) {
        throw std::invalid_argument("matrix sizes not compatible for addition");
    }

    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            result.at(i, j) = this->at(i, j) + _other.at(i, j);
        }
    }

    return result;
}


Matrix Matrix::operator-(Matrix const &_other) const {
    if (not addable(_other)) {
        throw std::invalid_argument("matrix sizes not compatible for subtraction");
    }

    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            result.at(i, j) = this->at(i, j) - _other.at(i, j);
        }
    }

    return result;
}


Matrix Matrix::operator*(Matrix const &_other) const {
    if (not multiplyable(_other)) {
        throw std::invalid_argument("matrix sizes not compatible for multiplication");
    }

    auto const m = m_rows;
    auto const n = m_cols;
    auto const p = _other.m_cols;

    Matrix result(m, p);
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < p; ++k) {
                result.at(i, k) += this->at(i, j) * _other.at(j, k);
            }
        }
    }
    return result;
}

Matrix& Matrix::operator=(Matrix const &_other) {
    if(not addable(_other)) {
        throw std::invalid_argument("matrix size not compatible for assignment");
    }

    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            this->m_data.at(i).at(j) = _other.m_data.at(i).at(j);
        }
    }

    return *this;
}

size_t Matrix::rows() const {
    return m_rows;
}

size_t Matrix::cols() const {
    return m_cols;
}

Matrix::Matrix(std::vector<double> const &_vec)
        : m_rows(1), m_cols(_vec.size()) {
    m_data.push_back(_vec);
}

std::vector<double> Matrix::vector() const {
    if(m_rows not_eq 1) {
        std::cerr << "WARNING: attempt to vectorize non row matrix. Returning 0th row ...\n";
    }

    return m_data.front();
}

Matrix Matrix::operator*(double const _scalar) const {
    Matrix result = *this;
    for(auto &row : result.m_data) {
        for(auto &elem : row) {
            elem *= _scalar;
        }
    }

    return result;
}
