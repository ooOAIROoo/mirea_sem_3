# Практическая работа №4 — Java

**Тема:** ООП в Java. Исключения.

Простая реализация всех заданий практической работы №4 (Часть 1: Задания №1–4 по наследованию, абстрактным классам и интерфейсам; Часть 2: Задания №1–2 по обработке исключений) и готовый отчёт по ГОСТ [`report.md`](./report.md).

---

## Что делал?

1. **Часть 1, Задание №1** — создал класс треугольника `Triangle_t1`, наследующий базовый класс `GeometricObject`, с расчётом периметра и площади по формуле Герона, и клиентскую программу с вводом сторон, цвета и заливки с клавиатуры.
2. **Часть 1, Задание №2** — добавил своё проверяемое исключение `IllegalTriangleException` и обновил конструктор `Triangle`, чтобы нельзя было создать треугольник, у которого сумма любых двух сторон меньше или равна третьей.
3. **Часть 1, Задание №3** — переделал `GeometricObject` в абстрактный класс с интерфейсом `Comparable<GeometricObject>` и статическим методом `max()`. Подключил к нему `Triangle`, `Circle`, `Rectangle` и `ComparableCircle`, проверил сравнение кругов и прямоугольников по площади.
4. **Часть 1, Задание №4** — создал интерфейс `Colorable` с методом `howToColor()` и класс `Square`, собрал массив из 5 разных фигур и вызвал `howToColor()` только для тех, которые можно раскрасить (`instanceof Colorable`).
5. **Часть 2, Задания №1 и №2** — написал программу вывода названия месяца и числа дней по номеру от 1 до 12 с перехватом `ArrayIndexOutOfBoundsException` и `InputMismatchException`, а затем доработал её проверкой високосного года для февраля.

---

## Где что лежит?

Как и в `pr_2` и `pr_3`, код разложен по пакетам, чтобы начальная и абстрактная версии `GeometricObject` и `Triangle` не конфликтовали по именам, а каждое задание запускалось отдельно:

```text
java/pr_4/
├── README.md                           # Краткое описание работы и запуска
├── report.md                           # Отчёт по ГОСТ 7.32-2017 (под Pandoc)
├── geom_basic/                         # Пакет для Заданий №1 и №2 (обычный GeometricObject)
│   ├── GeometricObject.java            # Базовый класс фигуры (color, filled, dateCreated)
│   ├── Triangle_t1.java                # Треугольник без исключений (Задание №1)
│   ├── IllegalTriangleException.java   # Своё исключение для неверных сторон (Задание №2)
│   └── Triangle.java                   # Треугольник с проверкой сторон (Задание №2)
├── geom/                               # Пакет для Заданий №3 и №4 (абстрактный GeometricObject)
│   ├── GeometricObject.java            # Абстрактный класс + Comparable + статический max()
│   ├── Triangle.java                   # Треугольник-наследник абстрактного GeometricObject
│   ├── Circle.java                     # Круг (radius, getArea, getPerimeter, getDiameter)
│   ├── Rectangle.java                  # Прямоугольник (width, height, getArea, getPerimeter)
│   ├── ComparableCircle.java           # Сравнимый круг (наследует Circle, реализует Comparable)
│   ├── Colorable.java                  # Интерфейс с методом howToColor()
│   └── Square.java                     # Квадрат (наследует GeometricObject, реализует Colorable)
└── app/                                # Пакет с точками входа main для каждого задания
    ├── TestTriangleTask1.java          # Запуск Части 1, Задания №1
    ├── TestTriangleTask2.java          # Запуск Части 1, Задания №2
    ├── TestGeometricTask3.java         # Запуск Части 1, Задания №3
    ├── TestColorableTask4.java         # Запуск Части 1, Задания №4
    ├── MonthDaysTask1.java             # Запуск Части 2, Задания 1
    └── MonthDaysLeapTask2.java         # Запуск Части 2, Задания 2
```

---

## Как работают ключевые методы?

- **`Triangle.getPerimeter()` и `Triangle.getArea()`** — периметр считается как `side1 + side2 + side3`, а площадь вычисляется по формуле Герона через полупериметр `s = getPerimeter() / 2.0`: `Math.sqrt(s * (s - side1) * (s - side2) * (s - side3))`.
- **`Triangle(double side1, double side2, double side3) throws IllegalTriangleException`** — проверяет, что все стороны положительны и для любой пары сторон их сумма строго больше третьей. Если правило нарушено, оператором `throw` выбрасывается `IllegalTriangleException`.
- **`GeometricObject.compareTo(GeometricObject other)`** — сравнивает текущую фигуру с другой по площади через `Double.compare(this.getArea(), other.getArea())`. Возвращает отрицательное число, `0` или положительное число.
- **`GeometricObject.max(GeometricObject o1, GeometricObject o2)`** — статический метод, который вызывает `o1.compareTo(o2)` и возвращает фигуру с большей площадью. Благодаря полиморфизму работает сразу и для кругов, и для прямоугольников, и для любых других наследников.
- **`Square.howToColor()`** — реализация метода интерфейса `Colorable`, выводит строку `"Раскрасьте все четыре стороны."`. В `TestColorableTask4` перед вызовом проверяется `obj instanceof Colorable` и делается приведение типа `((Colorable) obj).howToColor()`.
- **`MonthDaysTask1.main()` и `MonthDaysLeapTask2.isLeapYear(int year)`** — пользователь вводит номер месяца `1..12`, программа обращается к массивам `months[monthNumber - 1]` и `dom[monthNumber - 1]`. При выходе за границы `0..11` срабатывает `catch (ArrayIndexOutOfBoundsException)` и печатается `"Недопустимое число"`; при вводе букв или дробного числа срабатывает `catch (InputMismatchException)`. Метод `isLeapYear(year)` возвращает `true`, если `(year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)`, и для февраля ставит `29` дней вместо `28`.

---

## Как это связано?

```text
Часть 1 (Задания 1–2, пакет geom_basic):
  GeometricObject <── Triangle_t1
  GeometricObject <── Triangle ──throws──> IllegalTriangleException (<── Exception)

Часть 1 (Задания 3–4, пакет geom):
  Comparable<GeometricObject> <|.. abstract GeometricObject
                                      ├── Triangle (использует IllegalTriangleException)
                                      ├── Circle <── ComparableCircle
                                      ├── Rectangle
                                      └── Square ..|> Colorable

Часть 2 (Задания 1–2, пакет app):
  MonthDaysTask1     ──> массивы months[] и dom[] + try / catch / finally
  MonthDaysLeapTask2 ──> то же + проверка isLeapYear(year) для февраля
```

Все демонстрационные классы в пакете `app` обращаются к классам из `geom_basic` и `geom`, не смешивая этапы работы между собой.

---

## Компиляция, запуск и сборка отчёта

Компиляция всех заданий из папки `java/pr_4`:

```sh
cd java/pr_4
javac -encoding UTF-8 -d /tmp/pr4-classes geom_basic/*.java geom/*.java app/*.java
```

Запуск каждого задания по очереди:

```sh
java -cp /tmp/pr4-classes app.TestTriangleTask1
java -cp /tmp/pr4-classes app.TestTriangleTask2
java -cp /tmp/pr4-classes app.TestGeometricTask3
java -cp /tmp/pr4-classes app.TestColorableTask4
java -cp /tmp/pr4-classes app.MonthDaysTask1
java -cp /tmp/pr4-classes app.MonthDaysLeapTask2
```

> В Windows перед запуском можно выполнить `chcp 65001` для корректного отображения кириллицы.

Конвертация отчёта в Word по ГОСТ через Pandoc:

```sh
cd java/pr_4
pandoc report.md --toc --reference-doc=../gost-reference.docx -o report.docx
```

---

## Контрольные вопросы (для защиты)

1. **Чем отличаются проверяемые (`checked`) и непроверяемые (`unchecked`) исключения?**  
   Непроверяемые наследуются от `RuntimeException` или `Error` (например, `ArrayIndexOutOfBoundsException`, `ArithmeticException`) — компилятор не требует обязательно ловить их. Все остальные наследники `Exception` (в том числе наш `IllegalTriangleException`) являются проверяемыми: их нужно либо обработать в `try-catch`, либо указать в заголовке метода через `throws`.

2. **В чём разница между ключевыми словами `throw` и `throws`?**  
   `throw` используется внутри тела метода для фактического создания и выброса объекта исключения (`throw new IllegalTriangleException(...)`). `throws` пишется в сигнатуре метода и предупреждает вызывающий код, что метод может выбросить указанное исключение.

3. **Зачем нужен блок `finally`?**  
   Код в `finally` выполняется в любом случае — было исключение или нет. Его используют для гарантированного освобождения ресурсов (например, закрытия `Scanner` или файла).

4. **Почему в Задании №3 `GeometricObject` сделан абстрактным?**  
   У «просто геометрической фигуры» нет формулы площади и периметра — они зависят от конкретного вида фигуры (круг, прямоугольник, треугольник, квадрат). Объявление `getArea()` и `getPerimeter()` абстрактными обязывает каждый подкласс реализовать их и позволяет сравнивать любые фигуры в `compareTo()` и `max()`.

5. **Зачем перед вызовом `howToColor()` используется оператор `instanceof`?**  
   В массиве `GeometricObject[]` лежат разные фигуры, и не все они реализуют интерфейс `Colorable` (например, `Circle` и `Rectangle` — нет, а `Square` — да). Проверка `obj instanceof Colorable` защищает от ошибки приведения типов `ClassCastException`.

## Скриншоты для отчёта

Положите скриншоты запуска в папку `screenshots/` в порядке появления в отчёте:

1. `image.png`
2. `image copy.png`
3. `image copy 2.png`
4. `image copy 3.png`
5. `image copy 4.png`

Ссылки на эти файлы уже добавлены в `report.md`; подписи рисунков сохранены. В каждой работе последовательность имён начинается заново. Перед конвертацией через Pandoc добавьте все перечисленные изображения. Запускайте Pandoc из папки соответствующей практики, чтобы пути к картинкам разрешались корректно. Файл `.gitkeep` сохраняет пустую папку в Git.
