#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

class Vector {
 public:
  Vector() = default;
  Vector(double x, double y) : x_(x), y_(y) {}
  double operator*(const Vector&) const;
  double operator^(const Vector&) const;
  Vector operator+(const Vector&) const;
  Vector operator-(const Vector&) const;
  Vector& operator+=(const Vector&);
  Vector& operator-=(const Vector&);
  Vector& operator*=(const int64_t&);
  Vector& operator-();

  double LengthVector() const;

  double GetX() const { return x_; }
  double GetY() const { return y_; }

 private:
  double x_ = 0;
  double y_ = 0;
};

double Vector::operator*(const Vector& vect) const {
  return x_ * vect.x_ + y_ * vect.y_;
}

double Vector::operator^(const Vector& vect) const {
  return static_cast<double>(x_ * vect.y_ - y_ * vect.x_);
}

Vector Vector::operator+(const Vector& vect) const {
  return {x_ + vect.x_, y_ + vect.y_};
}

Vector Vector::operator-(const Vector& vect) const {
  return {x_ - vect.x_, y_ - vect.y_};
}

Vector& Vector::operator+=(const Vector& vect) {
  x_ += vect.x_;
  y_ += vect.y_;
  return *this;
}

Vector& Vector::operator-=(const Vector& vect) {
  x_ -= vect.x_;
  y_ -= vect.y_;
  return *this;
}

Vector& Vector::operator-() {
  x_ = -x_;
  y_ = -y_;
  return *this;
}

double TriangleSquare(const Vector& v1, const Vector& v2) {
  return 0.5 * std::abs(static_cast<double>(v1 ^ v2));
}

double Vector::LengthVector() const {
  return std::sqrt(static_cast<double>(x_ * x_ + y_ * y_));
}

int main() {
  std::pair<double, double> begin;
  std::pair<double, double> end;
  std::cin >> begin.first >> begin.second >> end.first >> end.second;
  Vector v1(end.first - begin.first, end.second - begin.second);
  std::cin >> begin.first >> begin.second >> end.first >> end.second;
  Vector v2(end.first - begin.first, end.second - begin.second);

  std::cout << std::setprecision(9);
  std::cout << v1.LengthVector() << " " << v2.LengthVector() << "\n";
  std::cout << std::fixed;
  Vector sum = v1 + v2;
  std::cout << std::setprecision(9) << sum.GetX() << " "
            << sum.GetY() << "\n";
  std::cout << std::fixed;
  std::cout << std::setprecision(9) << v1 * v2 << " "
            << (v1 ^ v2) << "\n" << TriangleSquare(v1, v2);
  std::cout << std::fixed;

  return 0;
}





