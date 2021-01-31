#pragma once

#include <cmath>

#include "matrix.hpp"
#include "vector.hpp"


class Projection {
  private:
    float m_fov;
    float m_ratio; //width / heigh
    float m_nearZ;
    float m_farZ;

  public:
    Projection();
    Projection(const float fov, const float ratio, const float nearZ, const float farZ);

    Matrix<4, 4> getProjectionMatrix() const;
};


Projection::Projection() {
  m_fov = static_cast<float>(M_PI)/2.0f;
  m_ratio = 1.0f;
  m_nearZ = 0.001f;
  m_farZ = 100.0f;
}

Projection::Projection(const float fov, const float ratio, const float nearZ, const float farZ) {
  m_fov = fov;
  m_ratio = ratio;
  m_nearZ = nearZ;
  m_farZ = farZ;
}

Matrix<4, 4> Projection::getProjectionMatrix() const {
  float zRange = m_nearZ - m_farZ;
  float tanHalfFov = static_cast<float>(tan(m_fov/2.0f));

  Matrix<4, 4> projectionMatrix;
  projectionMatrix.set(0, 0, 1.0f/(tanHalfFov*m_ratio));
  projectionMatrix.set(1, 1, 1.0f/tanHalfFov);
  projectionMatrix.set(2, 2, (-m_nearZ - m_farZ)/zRange);
  projectionMatrix.set(2, 3, 1.0f);
  projectionMatrix.set(3, 2, 2.0f*m_farZ*m_nearZ/zRange);

  return projectionMatrix;
}

