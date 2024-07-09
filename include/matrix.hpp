#ifndef MACHINE_LEARNING_MATRIX_HPP
#define MACHINE_LEARNING_MATRIX_HPP
#include <cstdint>
#include <vector>
class Matrix {
public:
    explicit Matrix(size_t _rows, size_t _cols);
    explicit Matrix(std::vector<double> const &_vec);
    [[nodiscard]] std::vector<double> vector() const;
    Matrix(Matrix const&) = default;

    void print() const;

    double&at(size_t _row, size_t _col);
    [[nodiscard]] double at(size_t _row, size_t _col) const;

    [[nodiscard]] size_t rows() const;
    [[nodiscard]] size_t cols() const;

    Matrix operator+(Matrix const&_other) const;
    Matrix operator-(Matrix const&_other) const;
    Matrix operator*(Matrix const&_other) const;

    Matrix &operator=(Matrix const&_other);

    Matrix operator*(double _scalar) const;

private:
    std::vector<std::vector<double>> m_data;
    size_t const m_rows;
    size_t const m_cols;

    [[nodiscard]] bool addable(Matrix const&_other) const;
    [[nodiscard]] bool multiplyable(Matrix const&_other) const;
};
#endif //MACHINE_LEARNING_MATRIX_HPP
