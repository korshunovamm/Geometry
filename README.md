## Geometry

Реализован набор классов для решения геометрических задач на плоскости. Все координаты предполагаются целочисленными.

1. Класс _Vector_ для вектора на плоскости, реализованы следующие методы:
* Конструктор по умолчанию (создает нулевой вектор)
* Конструктор от двух целочисленных переменных (создает вектор с соответствующими координатами)
* Оператор * для скалярного произведения
* Оператор  для векторного произведения
* Операторы сложения/разности с другим вектором, соответственно реализовать операторы += и -=
* Оператор умножения на число (допустимы как умножение вектора на число, так и числа на вектор), реализован оператор *=
* Унарный оператор минус для получения вектора в противоположном направлении
2. Создан набор классов — фигур, которые наследуются от абстрактного класса _IShape_ для работы с двумерными примитивами:
* _Point_ или точка
* _Segment_ или отрезок
* _Line_ или прямая
* _Ray_ или луч
* _Polygon_ или многоугольник
* *Circl*e или окружность
  В базовом классе предусмотрены следующие методы:

* void Move(const Vector&) — сдвиг фигуры на соответствующий вектор
* bool ContainsPoint(const Point&) — проверка, содержит ли фигура точку
* bool CrossSegment(const Segment&) — проверка, пересекает ли отрезок фигуру
* IShape* Clone() — вернуть указатель на копию фигуры
* void ToString() — строковое представление фигуры (формат в примерах)