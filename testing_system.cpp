#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Geometry {

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
  virtual IShape& Move(const Geometry::Vector& vect) = 0;
  virtual bool ContainsPoint(const Geometry::Point& point) const = 0;
  virtual bool CrossesSegment(const Geometry::Segment& segment) const = 0;
  virtual IShape* Clone() const = 0;
  virtual std::string ToString() = 0;
  virtual ~IShape() = default;
};

class Point : public IShape {
 public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {}
  Point(const Point& point) : x_(point.x_), y_(point.y_) {}

  IShape& Move(const Vector& vect) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossesSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  std::string ToString() override;

  Point operator-(const Point& point) const;
  Point operator+(const Point& point) const;
  inline int GetX() const { return x_; };
  inline int GetY() const { return y_; };
  void SetPoint(const Point& point) {
    x_ = point.GetX();
    y_ = point.GetY();
  }

 private:
  int x_ = 0;
  int y_ = 0;
};

class Segment : public IShape {
 public:
  Segment() = default;
  Segment(const Point& begin, const Point& end)
      : begin_({begin.GetX(), begin.GetY()}), end_({end.GetX(), end.GetY()}) {}
  Segment(const Segment& seg) : begin_(seg.begin_), end_(seg.end_) {}

  IShape& Move(const Vector& vect) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossesSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  std::string ToString() override;

  inline Point GetBegin() const { return begin_; }
  inline Point GetEnd() const { return end_; }
  void SetSegment(const Point& begin, const Point& end) {
    begin_ = {begin.GetX(), begin.GetY()};
    end_ = {end.GetX(), end.GetY()};
  }

 private:
  Point begin_ = {0, 0};
  Point end_ = {0, 0};
};

class Vector {
 public:
  Vector() = default;
  Vector(int x, int y) : x_(x), y_(y) {}
  Vector(const Point& point) : x_(point.GetX()), y_(point.GetY()) {}
  Vector(const Point& begin, const Point& end)
      : x_(end.GetX() - begin.GetX()), y_(end.GetY() - begin.GetY()) {}
  Vector(const Vector& vect) = default;

  int operator*(const Vector& vect) const;
  int operator^(const Vector& vect) const;
  Vector operator+(const Vector& vect) const;
  Vector operator-(const Vector& vect) const;
  Vector& operator+=(const Vector& vect);
  Vector& operator-=(const Vector& vect);
  Vector& operator*=(int num);
  Vector& operator-();
  Vector operator*(int num) const;

  std::string ToString() const {
    return "Vector(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
  }
  inline int GetX() const { return x_; }
  inline int GetY() const { return y_; }

 private:
  int x_ = 0;
  int y_ = 0;
};

class Ray : public IShape {
 public:
  Ray() = default;
  Ray(const Point& begin, const Point& end)
      : begin_({begin.GetX(), begin.GetY()}), direction_(end - begin) {}
  Ray(const Point& begin, const Vector& direction)
      : begin_(begin), direction_(direction) {}
  Ray(const Ray& ray) : begin_(ray.begin_), direction_(ray.direction_) {}

  IShape& Move(const Vector& vect) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossesSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  std::string ToString() override;

  inline Point GetBegin() const { return begin_; }
  inline Vector GetVector() const { return direction_; }

 private:
  Point begin_ = {0, 0};
  Vector direction_ = {0, 0};
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
    a_ = y2 - y1;
    b_ = x1 - x2;
    c_ = x2 * y1 - x1 * y2;
  }
  explicit Line(const Ray& ray) {
    a_ = ray.GetVector().GetY();
    b_ = -ray.GetVector().GetX();
    c_ = -a_ * ray.GetBegin().GetX() - b_ * ray.GetBegin().GetY();
  }
  Line(const Line& line) : a_(line.a_), b_(line.b_), c_(line.c_) {}

  IShape& Move(const Vector& vect) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossesSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  std::string ToString() override;

  inline long long PutPointIntoEquation(const Point& point) const {
    return a_ * point.GetX() + b_ * point.GetY() + c_;
  }
  bool IsParalel(const Line& line) const {
    return a_ * line.b_ == b_ * line.a_;
  }
  bool IsSame(const Line& line) const {
    return IsParalel(line) && (c_ * line.b_ == b_ * line.c_);
  }
  bool IsDistanceEqualRadius(const Line& line, int radius) const {
    if (radius == 0) {
      return false;
    }
    return (c_ - line.c_) * (c_ - line.c_) <=
           radius * radius * (a_ * a_ + b_ * b_);
  }
  std::vector<int> GetABC() const { return {a_, b_, c_}; }

 private:
  int a_ = 0;
  int b_ = 0;
  int c_ = 0;
};

class Circle : public IShape {
 public:
  Circle() = default;
  Circle(const Point& center, int radius) : center_(center), radius_(radius) {}
  Circle(const Circle& circle)
      : center_(circle.center_), radius_(circle.radius_) {}

  IShape& Move(const Vector& vect) override;
  bool ContainsPoint(const Point& point) const override;
  bool ContainsPointInPerimetr(const Point& point) const;
  bool CrossesSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  std::string ToString() override;

 private:
  Point center_ = {0, 0};
  int radius_ = 0;
};

class Polygon : public IShape {
 public:
  Polygon() = default;
  Polygon(const size_t& size, std::vector<Point> points)
      : size_(size), points_(std::move(points)) {}
  explicit Polygon(std::vector<Point> points)
      : size_(points.size()), points_(std::move(points)) {}

  IShape& Move(const Vector& vect) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossesSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  std::string ToString() override;

  inline std::vector<Point> GetVector() const { return points_; }
  bool ContainsPointWithDirection(const Point& point,
                                  const Vector& direction) const;
  inline void SetPoint(const Point& point, const size_t& index) {
    points_[index].SetPoint(point);
  }

 private:
  size_t size_ = 0;
  std::vector<Point> points_ = {};
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
Vector& Vector::operator*=(int num) {
  x_ *= num;
  y_ *= num;
  return *this;
}
Vector& Vector::operator-() {
  x_ = -x_;
  y_ = -y_;
  return *this;
}
Vector Vector::operator*(int num) const { return {num * x_, num * y_}; }
Vector operator*(int num, Vector& vect) { return vect * num; }

IShape& Point::Move(const Vector& vect) {
  x_ += vect.GetX();
  y_ += vect.GetY();
  return *this;
}
bool Point::ContainsPoint(const Point& point) const {
  return x_ == point.GetX() && y_ == point.GetY();
}
bool Point::CrossesSegment(const Segment& segment) const {
  return segment.ContainsPoint(*this);
}
IShape* Point::Clone() const { return new Point(*this); }
std::string Point::ToString() {
  return "Point(" + std::to_string(x_) + ", " + std::to_string(y_) + ")";
}
Point Point::operator-(const Point& point) const {
  return {x_ - point.x_, y_ - point.y_};
}
Point Point::operator+(const Point& point) const {
  return {x_ + point.x_, y_ + point.y_};
}

IShape& Segment::Move(const Vector& vect) {
  begin_.Move(vect);
  end_.Move(vect);
  return *this;
}
bool Segment::ContainsPoint(const Point& point) const {
  Vector v1(point - begin_);
  Vector v2(point - end_);
  return (v1 ^ v2) == 0 && (v1 * v2) <= 0;
}
bool Segment::CrossesSegment(const Segment& segment) const {
  Line ab_line(this->GetBegin(), this->GetEnd());
  Line cd_line(segment.GetBegin(), segment.GetEnd());
  if (ab_line.CrossesSegment(segment) && (cd_line.CrossesSegment(*this))) {
    if (!ContainsPoint(segment.GetBegin()) &&
        !ContainsPoint(segment.GetBegin())) {
      Vector v1(GetBegin(), segment.GetBegin());
      Vector v2(GetEnd(), segment.GetEnd());
      return v1 * v2 <= 0;
    }
    return true;
  }
  return false;
}
IShape* Segment::Clone() const { return new Segment(*this); }
std::string Segment::ToString() {
  Point begin = GetBegin();
  Point end = GetEnd();
  return "Segment(" + begin.ToString() + ", " + end.ToString() + ")\n";
}

IShape& Line::Move(const Vector& vect) {
  c_ -= (a_ * vect.GetX() + b_ * vect.GetY());
  return *this;
}
bool Line::ContainsPoint(const Point& point) const {
  return a_ * point.GetX() + b_ * point.GetY() + c_ == 0;
}
bool Line::CrossesSegment(const Segment& segment) const {
  return PutPointIntoEquation(segment.GetBegin()) *
             PutPointIntoEquation(segment.GetEnd()) <=
         0;
}
IShape* Line::Clone() const { return new Line(*this); }
std::string Line::ToString() {
  return "Line(" + std::to_string(a_) + ", " + std::to_string(b_) + ", " +
         std::to_string(c_) + ")";
}

IShape& Ray::Move(const Vector& vect) {
  begin_ = {vect.GetX() + begin_.GetX(), vect.GetY() + begin_.GetY()};
  return *this;
}

bool Ray::ContainsPoint(const Point& point) const {
  Line line(*this);
  if (line.ContainsPoint(point)) {
    Vector r0(point - begin_);
    if (r0 * direction_ >= 0) {
      return true;
    }
  }
  return false;
}
long long CalculateDeterminant(const Point& point1, const Point& point2) {
  Vector v1(point1);
  Vector v2(point2);
  return (v1 ^ v2);
}
bool Ray::CrossesSegment(const Segment& segment) const {
  Line line1(*this);
  Line line2(segment.GetBegin(), segment.GetEnd());
  if (line1.CrossesSegment(segment)) {
    if (line1.IsSame(line2)) {
      return ContainsPoint(segment.GetBegin()) ||
             ContainsPoint(segment.GetEnd());
    }
    long long x_numerator =
        CalculateDeterminant({line1.GetABC()[2], line1.GetABC()[1]},
                             {line2.GetABC()[2], line2.GetABC()[1]});
    long long y_numerator =
        CalculateDeterminant({line1.GetABC()[0], line1.GetABC()[2]},
                             {line2.GetABC()[0], line2.GetABC()[2]});
    long long denominator =
        -CalculateDeterminant({line1.GetABC()[0], line1.GetABC()[1]},
                              {line2.GetABC()[0], line2.GetABC()[1]});
    int x_intersect =
        denominator != 0 ? static_cast<int>(x_numerator / denominator) : 0;
    int y_intersect =
        denominator != 0 ? static_cast<int>(y_numerator / denominator) : 0;
    Vector intersect_point(x_intersect - begin_.GetX(),
                           y_intersect - begin_.GetY());
    if (intersect_point * direction_ >= 0) {
      return true;
    }
  }
  return false;
}
IShape* Ray::Clone() const { return new Ray(*this); }
std::string Ray::ToString() {
  return "Ray(" + begin_.ToString() + ", " + direction_.ToString() + ")";
}

IShape& Circle::Move(const Vector& vect) {
  center_ = {vect.GetX() + center_.GetX(), vect.GetY() + center_.GetY()};
  return *this;
}
bool Circle::ContainsPoint(const Point& point) const {
  Point len(point - center_);
  return len.GetX() * len.GetX() + len.GetY() * len.GetY() <= radius_ * radius_;
}
bool Circle::ContainsPointInPerimetr(const Point& point) const {
  Point len(point - center_);
  return len.GetX() * len.GetX() + len.GetY() * len.GetY() == radius_ * radius_;
}
bool Circle::CrossesSegment(const Segment& segment) const {
  if (ContainsPointInPerimetr(segment.GetBegin()) ||
      ContainsPointInPerimetr(segment.GetEnd())) {
    return true;
  }
  if ((ContainsPoint(segment.GetBegin()) && !ContainsPoint(segment.GetEnd())) ||
      (ContainsPoint(segment.GetEnd()) && !ContainsPoint(segment.GetBegin()))) {
    return true;
  }
  if ((!ContainsPoint(segment.GetBegin())) &&
      (!ContainsPoint(segment.GetEnd()))) {
    Line circle_line(center_,
                     center_ + (segment.GetEnd() - segment.GetBegin()));
    Line seg_line(segment.GetBegin(), segment.GetEnd());
    if (circle_line.IsSame(seg_line)) {
      Vector v1(segment.GetBegin() - center_);
      Vector v2(segment.GetEnd() - center_);
      return v1 * v2 < 0;
    }
    if (circle_line.IsDistanceEqualRadius(seg_line, radius_)) {
      return true;
    }
  }
  return false;
}
IShape* Circle::Clone() const { return new Circle(*this); }
std::string Circle::ToString() {
  return "Circle(" + center_.ToString() + ", " + std::to_string(radius_) + ")";
}

IShape& Polygon::Move(const Vector& vect) {
  for (size_t i = 0; i < size_; ++i) {
    SetPoint({vect.GetX() + points_[i].GetX(), vect.GetY() + points_[i].GetY()},
             i);
  }
  return *this;
}
bool Polygon::ContainsPoint(const Point& point) const {
  Vector direction1(13, 2);
  Vector direction2(1, 0);
  return ContainsPointWithDirection(point, direction2) ||
         ContainsPointWithDirection(point, direction1);
}
bool Polygon::ContainsPointWithDirection(const Point& point,
                                         const Vector& direction) const {
  Ray ray(point, direction);
  Segment seg;
  int n_intersections = 0;
  for (size_t i = 0; i < size_; ++i) {
    if (i != size_ - 1) {
      seg.SetSegment(points_[i], points_[i + 1]);
    } else {
      seg.SetSegment(points_[i], points_[0]);
    }
    if (seg.ContainsPoint(ray.GetBegin())) {
      return true;
    }
    n_intersections += ray.CrossesSegment(seg) ? 1 : 0;
  }
  return n_intersections % 2 == 1;
}
bool Polygon::CrossesSegment(const Segment& segment) const {
  Segment side;
  for (size_t i = 0; i < size_; ++i) {
    if (i != size_ - 1) {
      side.SetSegment(points_[i], points_[i + 1]);
    } else {
      side.SetSegment(points_[i], points_[0]);
    }
    if (side.CrossesSegment(segment)) {
      return true;
    }
  }
  return false;
}
IShape* Polygon::Clone() const { return new Polygon(*this); }
std::string Polygon::ToString() {
  std::string polygon;
  for (size_t i = 0; i < size_; ++i) {
    polygon += GetVector()[i].ToString();
    polygon += (i != (size_ - 1)) ? ", " : "";
  }
  return "Polygon(" + polygon + ")";
}
}  // namespace Geometry

template <class T>
void Delete(T* ptr) {
  delete ptr;
}

void CheckFunctions(const Geometry::IShape* shape_ptr,
                    const Geometry::Point& point_a,
                    const Geometry::Point& point_b) {
  std::cout << "Given shape "
            << (shape_ptr->ContainsPoint(point_a) ? "contains"
                                                  : "does not contain")
            << " point A\n";

  const auto kSegmentAb = Geometry::Segment(point_a, point_b);
  std::cout << "Given shape "
            << (shape_ptr->CrossesSegment(kSegmentAb) ? "crosses"
                                                      : "does not cross")
            << " segment AB\n";

  const auto kVectorAb = point_b - point_a;
  const auto kClonedShapePtr =
      shape_ptr->Clone();  // may return either raw or smart pointer
  std::cout << kClonedShapePtr->Move(kVectorAb).ToString();

  Delete(kClonedShapePtr);  // raw pointer compatibility
}

int main() {
  std::unique_ptr<Geometry::IShape> shape_ptr;

  std::string command;
  std::cin >> command;

  int x = 0;
  int y = 0;
  int a = 0;
  int b = 0;

  if (command == "point") {
    std::cin >> x >> y;
    shape_ptr = std::make_unique<Geometry::Point>(x, y);
  } else if (command == "segment") {
    std::cin >> x >> y >> a >> b;
    shape_ptr = std::make_unique<Geometry::Segment>(Geometry::Point(x, y),
                                                    Geometry::Point(a, b));
  } else if (command == "ray") {
    std::cin >> x >> y >> a >> b;
    shape_ptr = std::make_unique<Geometry::Ray>(Geometry::Point(x, y),
                                                Geometry::Point(a, b));
  } else if (command == "line") {
    std::cin >> x >> y >> a >> b;
    shape_ptr = std::make_unique<Geometry::Line>(Geometry::Point(x, y),
                                                 Geometry::Point(a, b));
  } else if (command == "polygon") {
    size_t n_points = 0;
    std::cin >> n_points;
    std::vector<Geometry::Point> points;
    points.reserve(n_points);
    for (size_t i = 0; i < n_points; ++i) {
      std::cin >> x >> y;
      points.emplace_back(x, y);
    }
    shape_ptr = std::make_unique<Geometry::Polygon>(std::move(points));
  } else if (command == "circle") {
    std::cin >> x >> y;
    const auto kCenter = Geometry::Point(x, y);
    uint64_t radius = 0;
    std::cin >> radius;
    shape_ptr = std::make_unique<Geometry::Circle>(kCenter, radius);
  } else {
    std::cerr << "Undefined command" << std::endl;
    return 1;
  }

  std::cin >> x >> y;
  const auto kPointA = Geometry::Point(x, y);
  std::cin >> x >> y;
  const auto kPointB = Geometry::Point(x, y);

  CheckFunctions(shape_ptr.get(), kPointA, kPointB);
  return 0;
}