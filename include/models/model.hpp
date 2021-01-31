#pragma once

#include <vector>
#include <cmath>

#include "matrix.hpp"


class Model {
  protected:
    std::vector<float> m_vertices;
    Matrix<4, 4> m_modelMatrix;
    size_t m_numVertices;

  public: 
    const std::vector<float> getVertices() const;
    Matrix<4, 4> getModelMatrix() const;
    size_t getVerticesSize() const;
    size_t getNumVertices() const;

    void setModelMatrix(const Matrix<4, 4> &modelMatrix);

    void translate(const Vector<3> &translation);
    void rotateX(const float theta);
    void rotateY(const float theta);
    void rotateZ(const float theta);
};

const std::vector<float> Model::getVertices() const {
  return m_vertices;
}

Matrix<4, 4> Model::getModelMatrix() const {
  return m_modelMatrix;
}

size_t Model::getVerticesSize() const {
  return m_vertices.size();
}

size_t Model::getNumVertices() const {
  return m_numVertices;
}

void Model::setModelMatrix(const Matrix<4, 4> &modelMatrix) {
  m_modelMatrix = modelMatrix.copy();
}

void Model::translate(const Vector<3> &translation) {
  Matrix<4, 4> translationMatrix = {{
    1.0f, 0.0f, 0.0f, translation.get(0),
    0.0f, 1.0f, 0.0f, translation.get(1),
    0.0f, 0.0f, 1.0f, translation.get(2),
    0.0f, 0.0f, 0.0f, 1.0f
  }};

  m_modelMatrix = m_modelMatrix.mul(translationMatrix);
}

void Model::rotateX(const float theta) {
  Matrix<4, 4> rotationMatrix = {{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, std::cos(theta), -std::sin(theta), 0.0f,
    0.0f, std::sin(theta), std::cos(theta), 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  }};

  m_modelMatrix = m_modelMatrix.mul(rotationMatrix);
}

void Model::rotateY(const float theta) {
  Matrix<4, 4> rotationMatrix = {{
    std::cos(theta), 0.0f, std::sin(theta), 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    -std::sin(theta), 0.0f, std::cos(theta), 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  }};

  m_modelMatrix = m_modelMatrix.mul(rotationMatrix);
}

void Model::rotateZ(const float theta) {
  Matrix<4, 4> rotationMatrix = {{
    std::cos(theta), -std::sin(theta), 0.0f, 0.0f,
    std::sin(theta), std::cos(theta), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  }};

  m_modelMatrix = m_modelMatrix.mul(rotationMatrix);
}
