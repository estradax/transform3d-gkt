#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <cmath>

#include <GL/glut.h>

class Matrix {
public:
  using MatrixType = std::vector<std::vector<float>>;

  Matrix();

  Matrix(const MatrixType&);

  void Print() const;

  Matrix Dot(const Matrix&);
  Matrix Add(const Matrix&);

  MatrixType Data() const;
  float X() const;
  float Y() const;
  float Z() const;

private:
  void AssertXYZ() const;

private:
  MatrixType data_;
};

Matrix::Matrix() {}

Matrix::Matrix(const Matrix::MatrixType& data) : data_{data} {}

void Matrix::AssertXYZ() const {
  if (data_.size() != 1 && data_[0].size() != 3) {
    std::cout << "failed to assert xyz\n";
    return;
  }
}

float Matrix::X() const {
  AssertXYZ();

  return data_[0][0];
}

float Matrix::Y() const {
  AssertXYZ();

  return data_[0][1];
}

float Matrix::Z() const {
  AssertXYZ();

  return data_[0][2];
}

Matrix::MatrixType Matrix::Data() const {
  return data_;
}

Matrix Matrix::Dot(const Matrix& rhs) {
    int rows1 = data_.size();
    int cols1 = data_[0].size();
    int rows2 = rhs.Data().size();
    int cols2 = rhs.Data()[0].size();

    // Check if multiplication is possible
    if (cols1 != rows2) {
      std::cout << "Matrix multiplication not possible!" << std::endl;
      return {{}};
    }

    // Initialize the result matrix with zeros
    std::vector<std::vector<float>> result(rows1, std::vector<float>(cols2, 0));

    // Perform matrix multiplication
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += data_[i][k] * rhs.Data()[k][j];
            }
        }
    }

    return result;
}

Matrix Matrix::Add(const Matrix& rhs) {
  int rows = data_.size();
  int cols = data_[0].size();

  // Check if matrices have the same dimensions
  if (rows != rhs.Data().size() || cols != rhs.Data()[0].size()) {
    std::cout << "Matrix addition not possible! Matrices have different dimensions." << std::endl;
    return {{}};
  }

  // Initialize the result matrix with zeros
  std::vector<std::vector<float>> result(rows, std::vector<float>(cols, 0));

  // Perform matrix addition
  for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
          result[i][j] = data_[i][j] + rhs.Data()[i][j];
      }
  }

  return result;
}

void Matrix::Print() const {
  for (const auto& row : data_) {
      for (int value : row) {
        std::cout << value << " ";
      }
      std::cout << std::endl;
  }
}

Matrix M{{
  {1, 0, 0},
  {0, 1, 0},
  {0, 0, 1}
}};

Matrix Tr{{
  {5, 0, 0}
}};

#define RECT_SIZE 4
#define LINE_SIZE 2

Matrix front[RECT_SIZE] = {
  {{{-2, 2, 0}}},
  {{{2, 2, 0}}},
  {{{2, -2, 0}}},
  {{{-2, -2, 0}}},
};

Matrix back[RECT_SIZE] = {
  {{{-2, 2, -5}}},
  {{{2, 2, -5}}},
  {{{2, -2, -5}}},
  {{{-2, -2, -5}}},
};

Matrix top_left_line[LINE_SIZE] = {
  {{{-2, 2, 0}}},
  {{{-2, 2, -5}}},
};

Matrix top_right_line[LINE_SIZE] = {
  {{{2, 2, 0}}},
  {{{2, 2, -5}}},
};

Matrix bottom_left_line[LINE_SIZE] = {
  {{{-2, -2, 0}}},
  {{{-2, -2, -5}}},
};

Matrix bottom_right_line[LINE_SIZE] = {
  {{{2, -2, 0}}},
  {{{2, -2, -5}}},
};

Matrix front_translated[RECT_SIZE];
Matrix back_translated[RECT_SIZE];
Matrix top_left_line_translated[LINE_SIZE];
Matrix top_right_line_translated[LINE_SIZE];
Matrix bottom_left_line_translated[LINE_SIZE];
Matrix bottom_right_line_translated[LINE_SIZE];

Matrix M_rot;
float angle = 0;

void AnimateTranslate(Matrix* from, Matrix* to, std::size_t size) {
  for (std::size_t i = 0; i < size; i++) {
    if (from[i].X() <= to[i].X()) {
      from[i] = from[i].Dot(M).Add({{{0.01, 0, 0}}});
    }
    if (from[i].Y() <= to[i].Y()) {
      from[i] = from[i].Dot(M).Add({{{0, 0.01, 0}}});
    }
    if (from[i].Z() <= to[i].Z()) {
      from[i] = from[i].Dot(M).Add({{{0, 0, 0.01}}});
    }
  }
}

Matrix MatrixRotationX(float angle) {
  auto rad = angle * (M_PI / 180.0);

  return {{
    {1, 0, 0},
    {0, static_cast<float>(std::cos(rad)), static_cast<float>(std::sin(rad))},
    {0, static_cast<float>(-std::sin(rad)), static_cast<float>(std::cos(rad))},
  }};
}

Matrix MatrixRotationY(float angle) {
  auto rad = angle * (M_PI / 180.0);

  return {{
    {static_cast<float>(std::cos(rad)), 0, static_cast<float>(-std::cos(rad))},
    {0, 1, 0},
    {static_cast<float>(std::sin(rad)), 0, static_cast<float>(std::cos(rad))}
  }};
}

Matrix MatrixRotationZ(float angle) {
  auto rad = angle * (M_PI / 180.0);

  return {{
    {static_cast<float>(std::cos(rad)), static_cast<float>(std::sin(rad)), 0},
    {static_cast<float>(-std::sin(rad)), static_cast<float>(std::cos(rad)), 0},
    {0, 0, 1}
  }};
}

void InitGL() {
  M_rot = MatrixRotationZ(angle); 

  for (std::size_t i = 0; i < RECT_SIZE; i++) {
    front_translated[i] = front[i].Dot(M).Add(Tr);
  }

  for (std::size_t i = 0; i < RECT_SIZE; i++) {
    back_translated[i] = back[i].Dot(M).Add(Tr);
  }

  for (std::size_t i = 0; i < LINE_SIZE; i++) {
    top_left_line_translated[i] = top_left_line[i].Dot(M).Add(Tr);
  }

  for (std::size_t i = 0; i < LINE_SIZE; i++) {
    top_right_line_translated[i] = top_right_line[i].Dot(M).Add(Tr);
  }

  for (std::size_t i = 0; i < LINE_SIZE; i++) {
    bottom_left_line_translated[i] = bottom_left_line[i].Dot(M).Add(Tr);
  }

  for (std::size_t i = 0; i < LINE_SIZE; i++) {
    bottom_right_line_translated[i] = bottom_right_line[i].Dot(M).Add(Tr);
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1, 2, 50);
  glMatrixMode(GL_MODELVIEW);
}

float t = 0;

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef(0, 0, -20);
  glRotatef(0, 0, 0, 0);

  glPointSize(10);
  glColor3f(1, 0, 0);
  glBegin(GL_POINTS);
    glVertex3f(0, 0, 0);
  glEnd();
  glColor3f(1, 1, 1);

  M_rot = MatrixRotationZ(angle);

  if (angle >= 360) {
    angle = 0;
  }

  angle += 1;

  AnimateTranslate(front, front_translated, RECT_SIZE);
  glBegin(GL_LINE_LOOP);
    for (std::size_t i = 0; i < RECT_SIZE; i++) {
      auto result = front[i].Dot(M_rot);
      glVertex3f(result.X(), result.Y(), result.Z());
    }
  glEnd();

  AnimateTranslate(back, back_translated, RECT_SIZE);
  glBegin(GL_LINE_LOOP);
    for (std::size_t i = 0; i < RECT_SIZE; i++) {
      auto result = back[i].Dot(M_rot);
      glVertex3f(result.X(), result.Y(), result.Z());
    }
  glEnd();

  AnimateTranslate(top_left_line, top_left_line_translated, LINE_SIZE);
  glBegin(GL_LINES);
    for (std::size_t i = 0; i < LINE_SIZE; i++) {
      auto result = top_left_line[i].Dot(M_rot);
      glVertex3f(result.X(), result.Y(), result.Z());
    }
  glEnd();

  AnimateTranslate(top_right_line, top_right_line_translated, LINE_SIZE);
  glBegin(GL_LINES);
    for (std::size_t i = 0; i < LINE_SIZE; i++) {
      auto result = top_right_line[i].Dot(M_rot);
      glVertex3f(result.X(), result.Y(), result.Z());
    }
  glEnd();

  AnimateTranslate(bottom_left_line, bottom_left_line_translated, LINE_SIZE);
  glBegin(GL_LINES);
    for (std::size_t i = 0; i < LINE_SIZE; i++) {
      auto result = bottom_left_line[i].Dot(M_rot);
      glVertex3f(result.X(), result.Y(), result.Z());
    }    
  glEnd();

  AnimateTranslate(bottom_right_line, bottom_right_line_translated, LINE_SIZE);
  glBegin(GL_LINES);
    for (std::size_t i = 0; i < LINE_SIZE; i++) {
      auto result = bottom_right_line[i].Dot(M_rot);
      glVertex3f(result.X(), result.Y(), result.Z());
    }
  glEnd();
  
  glutSwapBuffers();
}

void Timer(int) {
  glutPostRedisplay();
  glutTimerFunc(1000/60, Timer, 0);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutCreateWindow("Transform 3D");

  glutDisplayFunc(Display);
  glutTimerFunc(0, Timer, 0);
  InitGL();

  glutMainLoop();

  return 0;
}
