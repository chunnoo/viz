#pragma once

#include <iostream>
#include <cmath>
#include <array>


template<unsigned int N>
class Vector {
  private:
    std::array<float, N> m_values;

  public:
    Vector();
    Vector(const std::array<float, N> &values);

    Vector<N> copy() const;

    void set(const unsigned int i, const float v);
    float get(const unsigned int i) const;

    float length() const;
    Vector<N> normalized() const;

    float dot(const Vector<N> &rhs) const;
    Vector<3> cross(const Vector<N> &rhs) const;
    Vector<N> to(const Vector<N> &rhs) const;
    Vector<N> add(const Vector<N> &rhs) const;
};

template<unsigned int N>
std::ostream& operator<<(std::ostream &stream, const Vector<N> &vector);


template<unsigned int N>
Vector<N>::Vector() {
  m_values.fill(0.0f);
}

template<unsigned int N>
Vector<N>::Vector(const std::array<float, N> &values) {
  for (unsigned int i = 0; i < N; i++) {
    m_values[i] = values[i];
  }
}

template<unsigned int N>
Vector<N> Vector<N>::copy() const {
  Vector<N> copy;

  for (unsigned int i = 0; i < N; i++) {
    copy.set(i, m_values[i]);
  }

  return copy;
}

//TODO: validate indices

template<unsigned int N>
void Vector<N>::set(const unsigned int i, const float v) {
  m_values[i] = v;
}

template<unsigned int N>
float Vector<N>::get(const unsigned int i) const {
  return m_values[i];
}

template<unsigned int N>
float Vector<N>::length() const {
  float squaredSum = 0.0f;

  for (unsigned int i = 0; i < N; i++) {
    squaredSum += m_values[i]*m_values[i];
  }

  return squaredSum;
}

template<unsigned int N>
Vector<N> Vector<N>::normalized() const {
  float length = this->length();
  Vector<N> normalized;

  for (unsigned int i = 0; i < N; i++) {
    normalized.set(i, m_values[i]/length);
  }

  return normalized;
}

template<unsigned int N>
float Vector<N>::dot(const Vector<N> &rhs) const {
  float result = 0;

  for (unsigned int i = 0; i < N; i++) {
    result += this->get(i)*rhs.get(i);
  }

  return result;
}

template<>
Vector<3> Vector<3>::cross(const Vector<3> &rhs) const {
  Vector<3> result;

  result.set(0, m_values[1]*rhs.get(2) - rhs.get(1)*m_values[2]);
  result.set(1, m_values[2]*rhs.get(0) - rhs.get(2)*m_values[0]);
  result.set(2, m_values[0]*rhs.get(1) - rhs.get(1)*m_values[0]);

  return result;
}

template<>
Vector<3> Vector<2>::cross(const Vector<2> &rhs) const {
  Vector<3> result;

  result.set(0, 0);
  result.set(1, 0);
  result.set(2, m_values[0]*rhs.get(1) - rhs.get(1)*m_values[0]);

  return result;
}

template<unsigned int N>
Vector<3> Vector<N>::cross(const Vector<N> &rhs) const {
  Vector<3> result;

  result.set(0, m_values[1]*rhs.get(2) - rhs.get(1)*m_values[2]);
  result.set(1, m_values[2]*rhs.get(0) - rhs.get(0)*m_values[2]);
  result.set(2, m_values[0]*rhs.get(1) - rhs.get(1)*m_values[0]);

  return result;
}

template<unsigned int N>
Vector<N> Vector<N>::to(const Vector<N> &rhs) const {
  Vector<N> result;

  for (unsigned int i = 0; i < N; i++) {
    result.set(i, rhs.get(i) - m_values[i]);
  }

  return result;
}

template<unsigned int N>
Vector<N> Vector<N>::add(const Vector<N> &rhs) const {
  Vector<N> sum;

  for (unsigned int i = 0; i < N; i++) {
    sum.set(i, m_values[i] + rhs.get(i));
  }

  return sum;
}

template<unsigned int N>
std::ostream& operator<<(std::ostream &stream, const Vector<N> &vector) {
  for (unsigned int i = 0; i < N; i++) {
    stream << vector.get(i) << " ";
  };

  return stream;
}
