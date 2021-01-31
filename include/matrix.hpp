#pragma once

#include <iostream>
#include <cmath>
#include <array>
#include <string>


template<unsigned int N, unsigned int M>
class Matrix {
  private:
    std::array<float, N*M> m_values;

  public:
    Matrix<N, M>();
    Matrix<N, M>(const std::array<float, N*M> &values);

    Matrix<N, M> copy() const;
    
    void set(unsigned int i, unsigned int j, float v);
    float get(unsigned int i, unsigned int j) const;
    const std::array<float, N*M> getValues() const;
    const float* getData() const;

    Matrix<N, M> mul(Matrix<N, M> rhs) const;
};

template<unsigned int N, unsigned M>
std::ostream& operator<<(std::ostream& stream, const Matrix<N, M>& matrix);


template<>
Matrix<4, 4>::Matrix() {
  m_values.fill(0.0f);
  m_values[0] = 1.0f;
  m_values[5] = 1.0f;
  m_values[10] = 1.0f;
  m_values[15] = 1.0f;
}

template<unsigned int N, unsigned int M>
Matrix<N, M>::Matrix() {
  m_values.fill(0.0f);
  for (unsigned int i = 0; i < std::min(N, M); i++) {
    this->set(i, i, 1.0f);
  }
}

template<unsigned int N, unsigned int M>
Matrix<N, M>::Matrix(const std::array<float, N*M> &values) {
  for (unsigned int i = 0; i < N*M; i++) {
    m_values[i] = values[i];
  }
}

template<unsigned int N, unsigned int M>
Matrix<N, M> Matrix<N, M>::copy() const {
  Matrix<N, M> copy;
  
  for (unsigned int i = 0; i < N; i++) {
    for (unsigned int j = 0; j < M; j++) {
      copy.set(i, j, this->get(i, j));
    }
  }

  return copy;
}

//TODO: validate indices

template<>
void Matrix<4, 4>::set(unsigned int i, unsigned int j, float v) {
  m_values[4*i + j] = v;
}

template<>
float Matrix<4, 4>::get(unsigned int i, unsigned int j) const {
  return m_values[4*i + j];
}

template<>
const std::array<float, 16> Matrix<4, 4>::getValues() const {
  return m_values;
}

template<unsigned int N, unsigned int M>
const float* Matrix<N, M>::getData() const {
  return m_values.data();
}

template<>
Matrix<4, 4> Matrix<4, 4>::mul(Matrix<4, 4> rhs) const {
  Matrix<4, 4> result;

  for (unsigned int i = 0; i < 4; i++) {
    for (unsigned int j = 0; j < 4; j++) {
      float value = 0;
      for (unsigned int k = 0; k < 4; k++) {
        value += this->get(i, k)*rhs.get(k, j);
      }
      result.set(i, j, value);
    }
  }

  return result;
}

template<>
std::ostream& operator<<(std::ostream& stream, const Matrix<4, 4>& matrix) {
  for (unsigned int j = 0; j < 4; j++) {
    for (unsigned int i = 0; i < 4; i++) {
      stream << matrix.get(i, j) << " ";
    }
    stream << "\n";
  };

  return stream;
}
