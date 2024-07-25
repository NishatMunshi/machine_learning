#pragma once

#include <cstdint>
#include <vector>

class matrix {
public:
    explicit matrix(size_t _rows, size_t _cols);
    explicit matrix(std::vector<double> const &_vec);
    matrix(matrix const &) = default;

public:
    void print() const;

public:
    [[nodiscard]] double &at(size_t _row, size_t _col);
    [[nodiscard]] double at(size_t _row, size_t _col) const;
    [[nodiscard]] size_t rows() const;
    [[nodiscard]] size_t cols() const;
    [[nodiscard]] std::vector<double> vector() const;

public:
    matrix operator+(matrix const &_other) const;
    matrix operator-(matrix const &_other) const;
    matrix operator*(matrix const &_other) const;
    matrix &operator=(matrix const &_other);
    matrix operator*(double _scalar) const;

private:
    std::vector<std::vector<double>> m_data;
    size_t const m_rows;
    size_t const m_cols;

    [[nodiscard]] bool addable(matrix const &_other) const;
    [[nodiscard]] bool multiplyable(matrix const &_other) const;
};