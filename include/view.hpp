#pragma once

#include <iostream>

#include "vector.hpp"
#include "matrix.hpp"

class View {
  private:
    Vector<3> m_origin;
    Vector<3> m_focus;
    Vector<3> m_up;
  public:
    View(const Vector<3> &origin, const Vector<3> &focus, const Vector<3> &up);

    void setOrigin(const Vector<3> &origin);
    void setFocus(const Vector<3> &focus);
    void setUp(const Vector<3> &up);

    void normalize();

    Matrix<4, 4> getViewMatrix() const;
};


View::View(const Vector<3> &origin, const Vector<3> &focus, const Vector<3> &up) {
  m_origin = origin.copy();
  m_focus = focus.copy();
  m_up = up.copy();
}

void View::setOrigin(const Vector<3> &origin) {
  m_origin = origin.copy();
}

void View::setFocus(const Vector<3> &focus) {
  m_focus = focus.copy();
}

void View::setUp(const Vector<3> &up) {
  m_up = up.copy();
}

void View::normalize() {
  Vector<3> dir = m_origin.to(m_focus).normalized();

  m_focus = m_origin.add(dir);
}

Matrix<4, 4> View::getViewMatrix() const {
  std::cout << "Yo\n";
  std::cout << m_origin << "\n";
  std::cout << m_focus << "\n";
  std::cout << m_up << "\n";

  Vector<3> z = m_origin.to(m_focus).normalized();
  Vector<3> y = m_up.copy();
  Vector<3> x = y.cross(z);

  std::cout << "getViewMatrixFirst\n";
  std::cout << x << "\n";
  std::cout << y << "\n";
  std::cout << z << "\n";

  y = z.cross(x).normalized();
  x = x.normalized();

  std::cout << "getViewMatrixSecond\n";
  std::cout << x << "\n";
  std::cout << y << "\n";
  std::cout << z << "\n";

  Matrix<4, 4> viewMatrix;
  viewMatrix.set(0, 0, x.get(0));
  viewMatrix.set(1, 0, x.get(1));
  viewMatrix.set(2, 0, x.get(2));
  viewMatrix.set(3, 0, -x.dot(m_origin));
  viewMatrix.set(0, 1, y.get(0));
  viewMatrix.set(1, 1, y.get(1));
  viewMatrix.set(2, 1, y.get(2));
  viewMatrix.set(3, 1, -y.dot(m_origin));
  viewMatrix.set(0, 2, z.get(0));
  viewMatrix.set(1, 2, z.get(1));
  viewMatrix.set(2, 2, z.get(2));
  viewMatrix.set(3, 2, -z.dot(m_origin));
  viewMatrix.set(0, 3, 0.0f);
  viewMatrix.set(1, 3, 0.0f);
  viewMatrix.set(2, 3, 0.0f);
  viewMatrix.set(3, 3, 1.0f);

  return viewMatrix; 
}
