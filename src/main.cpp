#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "vector.hpp"
#include "matrix.hpp"
#include "view.hpp"
#include "projection.hpp"
#include "models/cube.hpp"

#define GLSL(src) "#version 410 \n" #src


int main() {

  Cube cube;
  View view(
      {{0.0f, 0.0f, 0.0f}},
      {{0.0f, 0.0f, 1.0f}},
      {{0.0f, 1.0f, 0.0f}}
  );
  Projection projection;

  Vector<3> lightDirection = {{-1.0, -1.0, 1.0}};
  lightDirection = lightDirection.normalized();

  const GLchar* vertexShaderSource = GLSL(
    in vec3 vPosition;
    in vec3 vNormal;
    out vec4 fColor;

    uniform vec3 lightDirection;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
  
    void main() {
      float lightIntensity = dot(model*vec4(vNormal, 0.0), -vec4(lightDirection, 0.0));
      fColor = vec4(vec3(lightIntensity), 1.0);
      gl_Position = projection*view*model*vec4(vPosition, 1.0);
    }
  );

  const GLchar* fragmentShaderSource = GLSL(
    in vec4 fColor;
    out vec4 outColor;

    void main() {
      outColor = fColor;
    }
  );

  int windowWidth = 512;
  int windowHeight = 512;
  double mouseX;
  double mouseY;

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(
    windowWidth,
    windowHeight,
    "viz",
    nullptr,
    nullptr
  );

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);

  GLuint vao;
  glGenVertexArrays(1, &vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(
      GL_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(cube.getVerticesSize())*6,
      cube.getVertices().data(),
      GL_STATIC_DRAW
  );

  GLint shaderStatus;
  char shaderInfo[512];

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderStatus);
  if (shaderStatus != GL_TRUE) {
    glGetShaderInfoLog(vertexShader, 512, NULL, shaderInfo);
    std::cout << "Vertex shader info:" << shaderInfo << std::endl;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderStatus);
  if (shaderStatus != GL_TRUE) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, shaderInfo);
    std::cout << "Fragment shader info:" << shaderInfo << std::endl;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  GLuint positionAttribute = static_cast<GLuint>(glGetAttribLocation(shaderProgram, "vPosition"));
  glEnableVertexAttribArray(positionAttribute);
  glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
  
  GLuint normalAttribute = static_cast<GLuint>(glGetAttribLocation(shaderProgram, "vNormal"));
  glEnableVertexAttribArray(normalAttribute);
  glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));

  GLint uniLightDirection = glGetUniformLocation(shaderProgram, "lightDirection");
  glUniform3f(uniLightDirection, lightDirection.get(0), lightDirection.get(1), lightDirection.get(2));

  GLint uniModel = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, cube.getModelMatrix().getData());
  
  GLint uniView = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, view.getViewMatrix().getData());
  
  GLint uniProjection = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(uniProjection, 1, GL_FALSE, projection.getProjectionMatrix().getData());

  double time = glfwGetTime();

  while(!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // IO
    time = glfwGetTime();

    glfwGetCursorPos(window, &mouseX, &mouseY);
    mouseX = mouseX/static_cast<double>(windowWidth)*2.0 - 1.0;
    mouseY = 1.0 - mouseY/static_cast<double>(windowHeight)*2.0;

    // Model View Setup

    cube.setModelMatrix(Matrix<4, 4>());
    cube.rotateX(static_cast<float>(std::fmod(time/2.0, M_PI*2)));
    cube.rotateY(static_cast<float>(std::fmod(time/2.0, M_PI*2)));

    view.setOrigin({{
      0.0, 0.0, -5.0f
    }});
    view.setFocus({{
        static_cast<float>(mouseX),
        static_cast<float>(mouseY),
        -4.0f
    }});
    view.setUp({{0.0f, 1.0f, 0.0f}});
    view.normalize();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(vao);

    glUseProgram(shaderProgram);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Matrix<4, 4> identMat;
    
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, cube.getModelMatrix().getData());
    glUniformMatrix4fv(uniView, 1, GL_FALSE, view.getViewMatrix().getData());
    glUniformMatrix4fv(uniProjection, 1, GL_FALSE, projection.getProjectionMatrix().getData());

    glDrawArrays(GL_TRIANGLES, 0, 36);// static_cast<GLsizei>(cube.getNumVertices()));
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glfwTerminate();

  return 0;
}
