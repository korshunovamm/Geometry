#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

class Vector;
class IShape;
class Point;
class Segment;
class Ray;
class Line;
class Circle;
class Polygon;

class IShape {
 public:
  virtual void Move(const Vector&) = 0;
  virtual bool ContainsPoint(const Point&) const = 0;
  virtual bool CrossSegment(const Segment&) const = 0;
  virtual IShape* Clone() = 0;
  virtual void ToString() = 0;
};

class Point : public IShape {
 public:
  Point() = default;
  Point(const int& x, const int& y) : x_(x), y_(y) {}
  Point(const Point& point) : x_(point.x_), y_(point.y_) {}
  ~Point() = default;

  void Move(const Vector& vect) override;
  bool ContainsPoint(const Point&) const override;
  bool CrossSegment(const Segment&) const override;
  IShape* Clone() override;
  void ToString() override;

  Point operator-(const Point&) const;
  inline std::string String() const {
    return "Point(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
  }
  inline int GetX() const { return x_; };
  inline int GetY() const { return y_; };

 private:
  int x_ = 0;
  int y_ = 0;
};

class Segment : public IShape {
 public:
  Segment() = default;
  Segment(const Point& begin, const Point& end) : begin_({begin.GetX(),
                                                          begin.GetY()}),
                                                  end_({end.GetX(),
                                                        end.GetY()}) {}
  Segment(const Segment& seg) : begin_(seg.begin_), end_(seg.end_) {}
  ~Segment() = default;

  void Move(const Vector& vect) override;
  bool ContainsPoint(const Point&) const override;
  bool CrossSegment(const Segment&) const override;
  IShape* Clone() override;
  void ToString() override;

  inline Point GetBegin() const { return begin_; }
  inline Point GetEnd() const { return end_; }
  std::pair<int, int> GetCoordBegin() const;
  std::pair<int, int> GetCoordEnd() const;

 private:
  Point begin_ = {0, 0};
  Point end_ = {0, 0};
};

class Vector {
 public:
  Vector() = default;
  Vector(const int x, const int y) : x_(x), y_(y) {}
  explicit Vector(const Point& point) : x_(point.GetX()), y_(point.GetY()) {}
  Vector(const Point& begin, const Point& end) : x_(end.GetX() - begin.GetX()),
                                                 y_(end.GetY()
                                                        - begin.GetY()) {}
  Vector(const Vector& vect) = default;
  ~Vector() = default;

  int operator*(const Vector&) const;
  int operator^(const Vector&) const;
  Vector operator+(const Vector&) const;
  Vector operator-(const Vector&) const;
  Vector& operator+=(const Vector&);
  Vector& operator-=(const Vector&);
  Vector& operator*=(const int&);
  Vector& operator-();
  Vector operator*(int constant) const;

  double LengthVector() const;
  inline int GetX() const { return x_; }
  inline int GetY() const { return y_; }

 private:
  int x_ = 0;
  int y_ = 0;
};

class Line : public IShape {
 public:
  Line() = default;
  Line(int a, int b, int c) : a_(a), b_(b), c_(c) {}
  Line(const Point& begin, const Point& end) {
    int x1 = begin.GetX();
    int y1 = begin.GetY();
    int x2 = end.GetX();
    int y2 = end.GetY();
    a_ = y1 - y2;
    b_ = x2 - x1;
    c_ = x1 * y2 - x2 * y1;
  }
  Line(const Line& line) : a_(line.a_), b_(line.b_), c_(line.c_) {}
  ~Line() = default;

  void Move(const Vector&) override;
  bool ContainsPoint(const Point&) const override;
  bool CrossSegment(const Segment&) const override;
  IShape* Clone() override;
  void ToString() override;

  inline long long PutPointIntoEquation(const Point& point) const {
    return a_ * point.GetX() + b_ * point.GetY() + c_;
  }
  inline std::vector<int> GetABC() const { return {a_, b_, c_}; }

 private:
  int a_ = 0;
  int b_ = 0;
  int c_ = 0;
};

int Vector::operator*(const Vector& vect) const {
  return x_ * vect.x_ + y_ * vect.y_;
}
int Vector::operator^(const Vector& vect) const {
  return x_ * vect.y_ - y_ * vect.x_;
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
Vector& Vector::operator*=(const int& constant) {
  x_ *= constant;
  y_ *= constant;
  return *this;
}
Vector& Vector::operator-() {
  x_ = -x_;
  y_ = -y_;
  return *this;
}
Vector Vector::operator*(const int constant) const {
  return {constant * x_, constant * y_};
}
Vector operator*(const int constant, Vector& vect) {
  return vect * constant;
}
double Vector::LengthVector() const {
  return std::sqrt(static_cast<double>(x_ * x_ + y_ * y_));
}
double TriangleSquare(const Vector& v1, const Vector& v2) {
  return 0.5 * std::abs(static_cast<double>(v1 ^ v2));
}

void Point::Move(const Vector& vect) {
  x_ += vect.GetX();
  y_ += vect.GetY();
}
bool Point::ContainsPoint(const Point& point) const {
  if (x_ == point.GetX() && y_ == point.GetY()) {
    return true;
  }
  return false;
}
bool Point::CrossSegment(const Segment& segment) const {
  return segment.ContainsPoint(*this);
}
IShape* Point::Clone() {
  return new Point(*this);
}
void Point::ToString() {
  std::cout << String();
}
Point Point::operator-(const Point& deduction) const {
  return {x_ - deduction.x_, y_ - deduction.y_};
}

void Segment::Move(const Vector& vect) {
  begin_.Move(vect);
  end_.Move(vect);
}
bool Segment::ContainsPoint(const Point& point) const {
  Vector v1(point - begin_);
  Vector v2(end_ - point);
  return (v1 ^ v2) == 0;
}
bool Segment::CrossSegment(const Segment& cd_seg) const {
  Line ab_line(this->GetBegin(), this->GetEnd());
  Line cd_line(cd_seg.GetBegin(), cd_seg.GetEnd());
  if (ab_line.CrossSegment(cd_seg) && (cd_line.CrossSegment(*this))) {
    return true;
  }
  return false;
}
IShape* Segment::Clone() {
  return new Segment(*this);
}
void Segment::ToString() {
  Point begin = GetBegin();
  Point end = GetEnd();
  std::cout << "Segment(" << begin.String() << ", " << end.String() << ")\n";
}
std::pair<int, int> Segment::GetCoordBegin() const {
  return {begin_.GetX(), begin_.GetY()};
}
std::pair<int, int> Segment::GetCoordEnd() const {
  return {end_.GetX(), end_.GetY()};
}

void Line::Move(const Vector& vect) {
  c_ -= (a_ * vect.GetX() + b_ * vect.GetY());
}
bool Line::ContainsPoint(const Point& point) const {
  return a_ * point.GetX() + b_ * point.GetY() + c_ == 0;
}
bool Line::CrossSegment(const Segment& seg) const {
  return
      PutPointIntoEquation(seg.GetBegin()) * PutPointIntoEquation(seg.GetEnd())
          <= 0;
}
IShape* Line::Clone() {
  return new Line(*this);
}
void Line::ToString() {
  std::cout << "Line(" + std::to_string(a_) + ", " + std::to_string(b_) + ", "
      + std::to_string(c_) + ")";
}

int Calculate(const Line& line1, const Line& line2, const int& letter1, const int& letter2) {
  Vector v1(line1.GetABC()[letter1], line1.GetABC()[letter2]);
  Vector v2(line2.GetABC()[letter1], line2.GetABC()[letter2]);
  return v1 ^ v2;
}
std::pair<double, double> IntersectionPoint(const Line& line1,
                                            const Line& line2) {
  int x_numerator = Calculate(line1, line2, 2, 1);
  int y_numerator = Calculate(line1, line2, 0, 2);
  double denominator = -static_cast<double>(Calculate(line1, line2, 0, 1));
  return {x_numerator / denominator, y_numerator / denominator};
}
double ParalellDistance(const Line& line1, const Line& line2) {
  return abs(line2.GetABC()[1] - line1.GetABC()[1]) / (sqrt(
      line2.GetABC()[0] * line2.GetABC()[0]
          + line2.GetABC()[1] * line2.GetABC()[1]));
}
bool IsParallel(const Line& line1, const Line& line2) {
  Vector v1(line1.GetABC()[0], line1.GetABC()[1]);
  Vector v2(line2.GetABC()[0], line2.GetABC()[1]);
  return (v1 ^ v2) == 0;
}

int main() {
  int a = 0;
  int b = 0;
  int c = 0;
  double b_double = 0;
  double  a_double = 0;
  std::cin >> a >> b >> c;
  Line line1(a, b, c);
  b_double = static_cast<double>(b);
  a_double = a == 0 ? static_cast<double>(a) : -static_cast<double>(a);
  std::cout << std::fixed << std::setprecision(9) << b_double << " " << a_double << "\n";

  std::cin >> a >> b >> c;
  Line line2(a, b, c);
  b_double = static_cast<double>(b);
  a_double = a == 0 ? static_cast<double>(a) : -static_cast<double>(a);
  std::cout << b_double << " " << a_double << "\n";


  if (IsParallel(line1, line2)) {
    std::cout << ParalellDistance(line1, line2) << "\n";
  } else {
    std::pair<double, double> point = IntersectionPoint(line1, line2);
    std::cout << point.first << " " << point.second << "\n";
  }
  return 0;
}

