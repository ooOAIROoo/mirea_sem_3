# Введение

Целью работы является изучение механизмов наследования классов, проектирования абстрактных классов и интерфейсов (`Comparable`, `Colorable`), а также освоение встроенных и пользовательских исключений в языке программирования Java.

Для достижения поставленной цели решаются следующие задачи:

- разработать класс треугольника `Triangle`, наследующий базовый класс `GeometricObject`, и реализовать вычисление площади и периметра;
- создать пользовательское проверяемое исключение `IllegalTriangleException` для контроля правила существования треугольника;
- спроектировать абстрактный класс `GeometricObject`, реализующий интерфейс `Comparable`, со статическим методом `max()`, а также подклассы `Triangle`, `Circle`, `Rectangle` и `ComparableCircle`;
- разработать интерфейс `Colorable` и класс `Square`, продемонстрировать полиморфную обработку массива геометрических фигур;
- реализовать консольные программы определения названия месяца и числа дней по номеру с обработкой исключений `ArrayIndexOutOfBoundsException` и `InputMismatchException`, включая проверку високосного года для февраля.

# 1 Теоретические сведения

## 1.1 Наследование, абстрактные классы и интерфейсы

Наследование в языке Java задаётся ключевым словом `extends` и позволяет формировать подклассы на основе общего суперкласса, повторно используя его поля и публичные методы. Общие характеристики геометрических фигур (цвет `color`, признак заливки `filled`, дата создания `dateCreated`) выносятся в базовый класс `GeometricObject`, а специфические свойства и формулы расчёта определяются в классах-наследниках (`Circle`, `Rectangle`, `Triangle`, `Square`).

Абстрактный класс (`abstract class`) может содержать абстрактные методы без реализации (например, `getArea()` и `getPerimeter()`), которые обязаны переопределить все конкретные подклассы. Создать экземпляр абстрактного класса напрямую нельзя, однако ссылки его типа используются для полиморфной работы с объектами подклассов.

Интерфейс (`interface`) задаёт контракт поведения класса и подключается ключевым словом `implements`. Стандартный интерфейс `java.lang.Comparable<T>` определяет метод `compareTo(T other)` для установления естественного порядка объектов, а пользовательский интерфейс `Colorable` выделяет подмножество фигур, поддерживающих операцию раскрашивания `howToColor()`.

## 1.2 Обработка исключений в Java

Исключение — это объект, описывающий нештатную ситуацию, возникающую во время выполнения программы и нарушающую её нормальный ход. Корневым классом иерархии исключений является `java.lang.Throwable`, от которого наследуются системные ошибки `Error` (выбрасываются виртуальной машиной JVM) и исключения `Exception`. Подклассы `RuntimeException` и `Error` относятся к непроверяемым (`unchecked`), а все прочие наследники `Exception` — к проверяемым (`checked`), обработка или объявление которых контролируется компилятором.

Основные ключевые слова механизма обработки исключений приведены в таблице 1.

Table: Таблица 1 — Ключевые слова обработки исключений в языке Java

| Ключевое слово | Назначение |
|---|---|
| `try` | Выделяет блок кода, в котором может возникнуть исключение |
| `catch` | Перехватывает и обрабатывает исключение указанного типа |
| `finally` | Выполняет завершающий код независимо от возникновения исключения |
| `throw` | Явно создаёт и выбрасывает объект исключения в теле метода |
| `throws` | Указывает в сигнатуре метода список возможных исключений |

# 2 Постановка задачи и структура проекта

Работа состоит из двух частей. В первой части выполняются четыре задания по объектно-ориентированному программированию и пользовательским исключениям, во второй части — два задания по перехвату стандартных исключений при работе с массивами и пользовательским вводом.

Для исключения конфликта имён между начальной (задания 1 и 2) и абстрактной (задания 3 и 4) версиями классов `GeometricObject` и `Triangle` исходный код разделён на три пакета. Соответствие файлов и заданий представлено в таблице 2.

Table: Таблица 2 — Структура проекта практической работы № 4

| Файл | Пакет | Назначение |
|---|---|---|
| `geom_basic/GeometricObject.java` | `geom_basic` | Базовый класс геометрической фигуры (задания 1 и 2) |
| `geom_basic/Triangle_t1.java` | `geom_basic` | Класс треугольника без генерации исключений (задание 1) |
| `geom_basic/IllegalTriangleException.java` | `geom_basic` | Проверяемое исключение нарушения правила треугольника (задание 2) |
| `geom_basic/Triangle.java` | `geom_basic` | Класс треугольника с проверкой сторон в конструкторе (задание 2) |
| `geom/GeometricObject.java` | `geom` | Абстрактный класс фигуры с `Comparable` и методом `max()` (задания 3 и 4) |
| `geom/Triangle.java` | `geom` | Класс треугольника, наследующий абстрактный `GeometricObject` (задание 3) |
| `geom/Circle.java` | `geom` | Класс круга (задания 3 и 4) |
| `geom/Rectangle.java` | `geom` | Класс прямоугольника (задания 3 и 4) |
| `geom/ComparableCircle.java` | `geom` | Сравнимый круг, наследующий `Circle` (задание 3) |
| `geom/Colorable.java` | `geom` | Интерфейс раскрашиваемого объекта (задание 4) |
| `geom/Square.java` | `geom` | Класс квадрата, реализующий `Colorable` (задание 4) |
| `app/TestTriangleTask1.java` | `app` | Клиентская программа для задания 1 части 1 |
| `app/TestTriangleTask2.java` | `app` | Тестовая программа для задания 2 части 1 |
| `app/TestGeometricTask3.java` | `app` | Тестовая программа для задания 3 части 1 |
| `app/TestColorableTask4.java` | `app` | Тестовая программа для задания 4 части 1 |
| `app/MonthDaysTask1.java` | `app` | Программа вывода месяца и числа дней (часть 2, задание 1) |
| `app/MonthDaysLeapTask2.java` | `app` | Программа вывода месяца с учётом високосного года (часть 2, задание 2) |

Компиляция всего проекта выполняется из каталога `java/pr_4` командой, приведённой в листинге 1.

::: {custom-style="ListingCaption"}
Листинг 1 — Команда компиляции проекта практической работы № 4
:::

```text
javac -encoding UTF-8 -d /tmp/pr4-classes geom_basic/*.java geom/*.java app/*.java
```

# 3 Ход выполнения работы

## 3.1 Часть 1. Задание 1 — Класс Triangle и наследование от GeometricObject

В пакете `geom_basic` реализован базовый класс `GeometricObject` с приватными полями `color`, `filled` и `dateCreated`, двумя конструкторами, методами доступа и переопределённым методом `toString()`. Его исходный код представлен в листинге 2.

::: {custom-style="ListingCaption"}
Листинг 2 — Исходный код файла geom_basic/GeometricObject.java
:::

```java
package geom_basic;

import java.util.Date;

/**
 * Базовый класс геометрической фигуры (Задания №1 и №2).
 */
public class GeometricObject {
    private String color = "белый";
    private boolean filled;
    private Date dateCreated;

    public GeometricObject() {
        this.dateCreated = new Date();
    }

    public GeometricObject(String color, boolean filled) {
        this.dateCreated = new Date();
        this.color = color;
        this.filled = filled;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public boolean isFilled() {
        return filled;
    }

    public void setFilled(boolean filled) {
        this.filled = filled;
    }

    public Date getDateCreated() {
        return dateCreated;
    }

    @Override
    public String toString() {
        return "создан " + dateCreated + ", цвет: " + color + ", закрашен: " + filled;
    }
}
```

Подкласс `Triangle_t1` содержит три поля `side1`, `side2`, `side3` типа `double` со значениями по умолчанию `1.0`, безаргументный конструктор, конструктор с тремя сторонами, геттеры для всех сторон, а также методы `getPerimeter()`, `getArea()` и `toString()`. Вычисление площади выполняется по формуле Герона через полупериметр:

$$s = \frac{side_1 + side_2 + side_3}{2}, \quad area = \sqrt{s(s - side_1)(s - side_2)(s - side_3)},$$

где $s$ — полупериметр треугольника; $side_1$, $side_2$, $side_3$ — длины сторон треугольника; $area$ — площадь треугольника.

Исходный код класса `Triangle_t1` и клиентской программы `TestTriangleTask1` приведён в листингах 3 и 4.

::: {custom-style="ListingCaption"}
Листинг 3 — Исходный код файла geom_basic/Triangle_t1.java
:::

```java
package geom_basic;

/**
 * Часть 1. Задание №1.
 * Класс треугольника, наследующий базовый класс GeometricObject.
 */
public class Triangle_t1 extends GeometricObject {
    private double side1 = 1.0;
    private double side2 = 1.0;
    private double side3 = 1.0;

    public Triangle_t1() {
        super();
    }

    public Triangle_t1(double side1, double side2, double side3) {
        super();
        this.side1 = side1;
        this.side2 = side2;
        this.side3 = side3;
    }

    public double getSide1() {
        return side1;
    }

    public double getSide2() {
        return side2;
    }

    public double getSide3() {
        return side3;
    }

    public double getPerimeter() {
        return side1 + side2 + side3;
    }

    public double getArea() {
        double s = getPerimeter() / 2.0;
        return Math.sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }

    @Override
    public String toString() {
        return "Треугольник: сторона1 = " + side1 + " сторона2 = " + side2
                + " сторона3 = " + side3;
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 4 — Исходный код файла app/TestTriangleTask1.java
:::

```java
package app;

import geom_basic.Triangle_t1;
import java.util.Locale;
import java.util.Scanner;

/**
 * Часть 1. Задание №1.
 * Ввод сторон треугольника, цвета и заливки; вывод площади, периметра, цвета и заливки.
 */
public class TestTriangleTask1 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        scanner.useLocale(Locale.US);

        System.out.print("Введите три стороны треугольника (через пробел): ");
        double side1 = scanner.nextDouble();
        double side2 = scanner.nextDouble();
        double side3 = scanner.nextDouble();

        System.out.print("Введите цвет треугольника: ");
        String color = scanner.next();

        System.out.print("Закрашен ли треугольник (true/false): ");
        boolean isFilled = scanner.nextBoolean();

        Triangle_t1 triangle = new Triangle_t1(side1, side2, side3);
        triangle.setColor(color);
        triangle.setFilled(isFilled);

        System.out.println();
        System.out.println(triangle.toString());
        System.out.printf(Locale.US, "Площадь: %.2f%n", triangle.getArea());
        System.out.printf(Locale.US, "Периметр: %.2f%n", triangle.getPerimeter());
        System.out.println("Цвет: " + triangle.getColor());
        System.out.println("Закрашен: " + triangle.isFilled());
        scanner.close();
    }
}
```

Консольный вывод при вводе сторон `3`, `4`, `5`, цвета `красный` и признака заливки `true` приведён в листинге 5, место для скриншота обозначено на рисунке 1.

::: {custom-style="ListingCaption"}
Листинг 5 — Вывод программы app.TestTriangleTask1
:::

```text
Введите три стороны треугольника (через пробел): 3 4 5
Введите цвет треугольника: красный
Закрашен ли треугольник (true/false): true

Треугольник: сторона1 = 3.0 сторона2 = 4.0 сторона3 = 5.0
Площадь: 6.00
Периметр: 12.00
Цвет: красный
Закрашен: true
```

::: {custom-style="Captioned Figure"}
[Место для вставки скриншота запуска программы app.TestTriangleTask1]
:::

::: {custom-style="Image Caption"}
Рисунок 1 — Результат работы программы app.TestTriangleTask1
:::

## 3.2 Часть 1. Задание 2 — Пользовательское исключение IllegalTriangleException

Для проверки неравенства треугольника создан класс `IllegalTriangleException`, наследующий `java.lang.Exception` (листинг 6). В конструкторе класса `Triangle` (листинг 7) проверяется положительность сторон и условие того, что сумма длин любых двух сторон строго больше третьей; при нарушении правила оператором `throw` выбрасывается экземпляр `IllegalTriangleException`. Тестирование корректного и некорректного наборов сторон выполняется в классе `TestTriangleTask2` (листинг 8).

::: {custom-style="ListingCaption"}
Листинг 6 — Исходный код файла geom_basic/IllegalTriangleException.java
:::

```java
package geom_basic;

/**
 * Часть 1. Задание №2.
 * Пользовательское проверяемое исключение для недопустимых сторон треугольника.
 */
public class IllegalTriangleException extends Exception {
    private static final long serialVersionUID = 1L;

    private final double side1;
    private final double side2;
    private final double side3;

    public IllegalTriangleException(double side1, double side2, double side3) {
        super("Недопустимые стороны треугольника: " + side1 + ", " + side2 + ", " + side3
                + " (сумма любых двух сторон должна быть больше третьей)");
        this.side1 = side1;
        this.side2 = side2;
        this.side3 = side3;
    }

    public double getSide1() {
        return side1;
    }

    public double getSide2() {
        return side2;
    }

    public double getSide3() {
        return side3;
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 7 — Исходный код файла geom_basic/Triangle.java
:::

```java
package geom_basic;

/**
 * Часть 1. Задание №2.
 * Класс треугольника с проверкой неравенства треугольника в конструкторе.
 */
public class Triangle extends GeometricObject {
    private double side1 = 1.0;
    private double side2 = 1.0;
    private double side3 = 1.0;

    public Triangle() {
        super();
    }

    /** Создает треугольник с указанными сторонами */
    public Triangle(double side1, double side2, double side3)
            throws IllegalTriangleException {
        super();
        if (side1 <= 0 || side2 <= 0 || side3 <= 0
                || side1 + side2 <= side3
                || side1 + side3 <= side2
                || side2 + side3 <= side1) {
            throw new IllegalTriangleException(side1, side2, side3);
        }
        this.side1 = side1;
        this.side2 = side2;
        this.side3 = side3;
    }

    public double getSide1() {
        return side1;
    }

    public double getSide2() {
        return side2;
    }

    public double getSide3() {
        return side3;
    }

    public double getPerimeter() {
        return side1 + side2 + side3;
    }

    public double getArea() {
        double s = getPerimeter() / 2.0;
        return Math.sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }

    @Override
    public String toString() {
        return "Треугольник: сторона1 = " + side1 + " сторона2 = " + side2
                + " сторона3 = " + side3;
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 8 — Исходный код файла app/TestTriangleTask2.java
:::

```java
package app;

import geom_basic.IllegalTriangleException;
import geom_basic.Triangle;
import java.util.Locale;

/**
 * Часть 1. Задание №2.
 * Проверка класса Triangle и пользовательского исключения IllegalTriangleException.
 */
public class TestTriangleTask2 {
    public static void main(String[] args) {
        // 1. Корректный треугольник
        try {
            Triangle valid = new Triangle(3.0, 4.0, 5.0);
            System.out.println("Создан корректный объект -> " + valid);
            System.out.printf(Locale.US, "Площадь: %.2f, периметр: %.2f%n",
                    valid.getArea(), valid.getPerimeter());
        } catch (IllegalTriangleException ex) {
            System.out.println("Ошибка: " + ex.getMessage());
        }

        System.out.println();

        // 2. Треугольник с нарушением правила суммы двух сторон (1 + 2 <= 3)
        try {
            Triangle invalid = new Triangle(1.0, 2.0, 3.0);
            System.out.println("Создан объект -> " + invalid);
        } catch (IllegalTriangleException ex) {
            System.out.println("Перехвачено исключение: " + ex.getMessage());
        }
    }
}
```

Вывод программы `TestTriangleTask2` приведён в листинге 9, место для скриншота показано на рисунке 2.

::: {custom-style="ListingCaption"}
Листинг 9 — Вывод программы app.TestTriangleTask2
:::

```text
Создан корректный объект -> Треугольник: сторона1 = 3.0 сторона2 = 4.0 сторона3 = 5.0
Площадь: 6.00, периметр: 12.00

Перехвачено исключение: Недопустимые стороны треугольника: 1.0, 2.0, 3.0 (сумма любых двух сторон должна быть больше третьей)
```

::: {custom-style="Captioned Figure"}
[Место для вставки скриншота запуска программы app.TestTriangleTask2]
:::

::: {custom-style="Image Caption"}
Рисунок 2 — Результат работы программы app.TestTriangleTask2
:::

## 3.3 Часть 1. Задание 3 — Абстрактный GeometricObject, Comparable и ComparableCircle

В пакете `geom` спроектирован абстрактный класс `GeometricObject`, реализующий интерфейс `Comparable<GeometricObject>` (листинг 10). В нём объявлены абстрактные методы `getArea()` и `getPerimeter()`, реализовано сравнение по площади в `compareTo()` и определён статический метод `max()` для выбора наибольшей из двух фигур. От него унаследованы классы `Triangle` (листинг 11), `Circle` (листинг 12), `Rectangle` (листинг 13) и `ComparableCircle` (листинг 14). Проверка всех пунктов задания выполнена в программе `TestGeometricTask3` (листинг 15).

::: {custom-style="ListingCaption"}
Листинг 10 — Исходный код файла geom/GeometricObject.java
:::

```java
package geom;

import java.util.Date;

/**
 * Часть 1. Задания №3 и №4.
 * Абстрактный класс геометрической фигуры, реализующий Comparable.
 */
public abstract class GeometricObject implements Comparable<GeometricObject> {
    private String color = "белый";
    private boolean filled;
    private Date dateCreated;

    protected GeometricObject() {
        this.dateCreated = new Date();
    }

    protected GeometricObject(String color, boolean filled) {
        this.dateCreated = new Date();
        this.color = color;
        this.filled = filled;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public boolean isFilled() {
        return filled;
    }

    public void setFilled(boolean filled) {
        this.filled = filled;
    }

    public Date getDateCreated() {
        return dateCreated;
    }

    public abstract double getArea();

    public abstract double getPerimeter();

    @Override
    public int compareTo(GeometricObject other) {
        return Double.compare(this.getArea(), other.getArea());
    }

    public static GeometricObject max(GeometricObject o1, GeometricObject o2) {
        return (o1.compareTo(o2) >= 0) ? o1 : o2;
    }

    @Override
    public String toString() {
        return "создан " + dateCreated + ", цвет: " + color + ", закрашен: " + filled;
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 11 — Исходный код файла geom/Triangle.java
:::

```java
package geom;

import geom_basic.IllegalTriangleException;

/**
 * Часть 1. Задание №3.
 * Класс треугольника, наследующий абстрактный класс GeometricObject.
 */
public class Triangle extends GeometricObject {
    private double side1 = 1.0;
    private double side2 = 1.0;
    private double side3 = 1.0;

    public Triangle() {
        super();
    }

    public Triangle(double side1, double side2, double side3)
            throws IllegalTriangleException {
        super();
        if (side1 <= 0 || side2 <= 0 || side3 <= 0
                || side1 + side2 <= side3
                || side1 + side3 <= side2
                || side2 + side3 <= side1) {
            throw new IllegalTriangleException(side1, side2, side3);
        }
        this.side1 = side1;
        this.side2 = side2;
        this.side3 = side3;
    }

    public Triangle(double side1, double side2, double side3, String color, boolean filled)
            throws IllegalTriangleException {
        this(side1, side2, side3);
        setColor(color);
        setFilled(filled);
    }

    public double getSide1() {
        return side1;
    }

    public double getSide2() {
        return side2;
    }

    public double getSide3() {
        return side3;
    }

    @Override
    public double getPerimeter() {
        return side1 + side2 + side3;
    }

    @Override
    public double getArea() {
        double s = getPerimeter() / 2.0;
        return Math.sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }

    @Override
    public String toString() {
        return "Треугольник: сторона1 = " + side1 + " сторона2 = " + side2
                + " сторона3 = " + side3;
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 12 — Исходный код файла geom/Circle.java
:::

```java
package geom;

import java.util.Locale;

/**
 * Часть 1. Задания №3 и №4.
 * Класс круга, наследующий абстрактный класс GeometricObject.
 */
public class Circle extends GeometricObject {
    private double radius;

    public Circle() {
        this(1.0);
    }

    public Circle(double radius) {
        super();
        this.radius = radius;
    }

    public Circle(double radius, String color, boolean filled) {
        super(color, filled);
        this.radius = radius;
    }

    public double getRadius() {
        return radius;
    }

    public void setRadius(double radius) {
        this.radius = radius;
    }

    public double getDiameter() {
        return 2.0 * radius;
    }

    @Override
    public double getArea() {
        return Math.PI * radius * radius;
    }

    @Override
    public double getPerimeter() {
        return 2.0 * Math.PI * radius;
    }

    public void printCircle() {
        System.out.println("Круг создан " + getDateCreated() + ", радиус = " + radius);
    }

    @Override
    public String toString() {
        return String.format(Locale.US, "Круг (radius = %.2f, площадь = %.2f)", radius, getArea());
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 13 — Исходный код файла geom/Rectangle.java
:::

```java
package geom;

import java.util.Locale;

/**
 * Часть 1. Задания №3 и №4.
 * Класс прямоугольника, наследующий абстрактный класс GeometricObject.
 */
public class Rectangle extends GeometricObject {
    private double width;
    private double height;

    public Rectangle() {
        this(1.0, 1.0);
    }

    public Rectangle(double width, double height) {
        super();
        this.width = width;
        this.height = height;
    }

    public Rectangle(double width, double height, String color, boolean filled) {
        super(color, filled);
        this.width = width;
        this.height = height;
    }

    public double getWidth() {
        return width;
    }

    public void setWidth(double width) {
        this.width = width;
    }

    public double getHeight() {
        return height;
    }

    public void setHeight(double height) {
        this.height = height;
    }

    @Override
    public double getArea() {
        return width * height;
    }

    @Override
    public double getPerimeter() {
        return 2.0 * (width + height);
    }

    @Override
    public String toString() {
        return String.format(Locale.US, "Прямоугольник (width = %.2f, height = %.2f, площадь = %.2f)",
                width, height, getArea());
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 14 — Исходный код файла geom/ComparableCircle.java
:::

```java
package geom;

import java.util.Locale;

/**
 * Часть 1. Задание №3 (пункт 6).
 * Класс сравнимого круга, наследующий Circle и реализующий Comparable.
 */
public class ComparableCircle extends Circle implements Comparable<GeometricObject> {
    public ComparableCircle() {
        super();
    }

    public ComparableCircle(double radius) {
        super(radius);
    }

    public ComparableCircle(double radius, String color, boolean filled) {
        super(radius, color, filled);
    }

    @Override
    public int compareTo(GeometricObject other) {
        return Double.compare(this.getArea(), other.getArea());
    }

    @Override
    public String toString() {
        return String.format(Locale.US, "ComparableCircle (radius = %.2f, площадь = %.2f)",
                getRadius(), getArea());
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 15 — Исходный код файла app/TestGeometricTask3.java
:::

```java
package app;

import geom.Circle;
import geom.ComparableCircle;
import geom.GeometricObject;
import geom.Rectangle;
import geom.Triangle;
import geom_basic.IllegalTriangleException;
import java.util.Locale;
import java.util.Scanner;

/**
 * Часть 1. Задание №3.
 * Тестирование абстрактного класса GeometricObject, Triangle, метода max() и ComparableCircle.
 */
public class TestGeometricTask3 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        scanner.useLocale(Locale.US);

        // Пункты 1-3: ввод данных и создание Triangle
        System.out.print("Введите три стороны треугольника (через пробел): ");
        double side1 = scanner.nextDouble();
        double side2 = scanner.nextDouble();
        double side3 = scanner.nextDouble();

        System.out.print("Введите цвет треугольника: ");
        String color = scanner.next();

        System.out.print("Закрашен ли треугольник (true/false): ");
        boolean filled = scanner.nextBoolean();

        System.out.println();
        try {
            Triangle triangle = new Triangle(side1, side2, side3);
            triangle.setColor(color);
            triangle.setFilled(filled);

            System.out.println(triangle);
            System.out.printf(Locale.US, "Площадь: %.2f%n", triangle.getArea());
            System.out.printf(Locale.US, "Периметр: %.2f%n", triangle.getPerimeter());
            System.out.println("Цвет: " + triangle.getColor());
            System.out.println("Закрашен: " + triangle.isFilled());
        } catch (IllegalTriangleException ex) {
            System.out.println("Ошибка создания треугольника: " + ex.getMessage());
        }

        // Пункт 5: поиск наибольшего из двух кругов и двух прямоугольников через max()
        System.out.println("\n--- Проверка GeometricObject.max() ---");
        Circle circle1 = new Circle(3.0);
        Circle circle2 = new Circle(5.0);
        System.out.println("Наибольший из двух кругов: "
                + GeometricObject.max(circle1, circle2));

        Rectangle rect1 = new Rectangle(4.0, 6.0);
        Rectangle rect2 = new Rectangle(3.0, 9.0);
        System.out.println("Наибольший из двух прямоугольников: "
                + GeometricObject.max(rect1, rect2));

        // Пункт 6: сравнение двух ComparableCircle и круга с прямоугольником через compareTo()
        System.out.println("\n--- Проверка ComparableCircle и compareTo() ---");
        ComparableCircle compCircle1 = new ComparableCircle(4.0);
        ComparableCircle compCircle2 = new ComparableCircle(6.0);
        GeometricObject maxCompCircle = (compCircle1.compareTo(compCircle2) >= 0)
                ? compCircle1 : compCircle2;
        System.out.println("Наибольший из двух ComparableCircle: " + maxCompCircle);

        Rectangle rectForCompare = new Rectangle(7.0, 8.0);
        GeometricObject maxCircleOrRect = (compCircle1.compareTo(rectForCompare) >= 0)
                ? compCircle1 : rectForCompare;
        System.out.println("Наибольший между " + compCircle1 + " и " + rectForCompare
                + " -> " + maxCircleOrRect);
        scanner.close();
    }
}
```

Вывод программы `TestGeometricTask3` представлен в листинге 16, место для скриншота указано на рисунке 3.

::: {custom-style="ListingCaption"}
Листинг 16 — Вывод программы app.TestGeometricTask3
:::

```text
Введите три стороны треугольника (через пробел): 5 6 7
Введите цвет треугольника: синий
Закрашен ли треугольник (true/false): false

Треугольник: сторона1 = 5.0 сторона2 = 6.0 сторона3 = 7.0
Площадь: 14.70
Периметр: 18.00
Цвет: синий
Закрашен: false

--- Проверка GeometricObject.max() ---
Наибольший из двух кругов: Круг (radius = 5.00, площадь = 78.54)
Наибольший из двух прямоугольников: Прямоугольник (width = 3.00, height = 9.00, площадь = 27.00)

--- Проверка ComparableCircle и compareTo() ---
Наибольший из двух ComparableCircle: ComparableCircle (radius = 6.00, площадь = 113.10)
Наибольший между ComparableCircle (radius = 4.00, площадь = 50.27) и Прямоугольник (width = 7.00, height = 8.00, площадь = 56.00) -> Прямоугольник (width = 7.00, height = 8.00, площадь = 56.00)
```

::: {custom-style="Captioned Figure"}
[Место для вставки скриншота запуска программы app.TestGeometricTask3]
:::

::: {custom-style="Image Caption"}
Рисунок 3 — Результат работы программы app.TestGeometricTask3
:::

## 3.4 Часть 1. Задание 4 — Интерфейс Colorable и класс Square

Создан интерфейс `Colorable` с методом `void howToColor()` (листинг 17) и класс `Square` (листинг 18), наследующий `GeometricObject` и реализующий `Colorable`. Класс содержит приватное поле `side`, безаргументный конструктор (задающий `side = 0`), конструктор с параметром стороны, геттер и сеттер, методы `getArea()`, `getPerimeter()` и `howToColor()`. В классе `TestColorableTask4` (листинг 19) формируется массив из пяти объектов `GeometricObject`; для каждого выводится площадь, а при выполнении условия `obj instanceof Colorable` вызывается метод `howToColor()`.

::: {custom-style="ListingCaption"}
Листинг 17 — Исходный код файла geom/Colorable.java
:::

```java
package geom;

/**
 * Часть 1. Задание №4 (пункт 1).
 * Интерфейс для раскрашиваемых геометрических объектов.
 */
public interface Colorable {
    void howToColor();
}
```

::: {custom-style="ListingCaption"}
Листинг 18 — Исходный код файла geom/Square.java
:::

```java
package geom;

import java.util.Locale;

/**
 * Часть 1. Задание №4 (пункты 2 и 3).
 * Класс квадрата, наследующий GeometricObject и реализующий интерфейс Colorable.
 */
public class Square extends GeometricObject implements Colorable {
    private double side;

    public Square() {
        this(0.0);
    }

    public Square(double side) {
        super();
        this.side = side;
    }

    public Square(double side, String color, boolean filled) {
        super(color, filled);
        this.side = side;
    }

    public double getSide() {
        return side;
    }

    public void setSide(double side) {
        this.side = side;
    }

    @Override
    public double getArea() {
        return side * side;
    }

    @Override
    public double getPerimeter() {
        return 4.0 * side;
    }

    @Override
    public void howToColor() {
        System.out.println("Раскрасьте все четыре стороны.");
    }

    @Override
    public String toString() {
        return String.format(Locale.US, "Квадрат (side = %.2f, площадь = %.2f)",
                side, getArea());
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 19 — Исходный код файла app/TestColorableTask4.java
:::

```java
package app;

import geom.Circle;
import geom.Colorable;
import geom.GeometricObject;
import geom.Rectangle;
import geom.Square;
import geom.Triangle;
import geom_basic.IllegalTriangleException;
import java.util.Locale;

/**
 * Часть 1. Задание №4.
 * Массив из пяти объектов GeometricObject: вывод площади и вызов howToColor() для Colorable.
 */
public class TestColorableTask4 {
    public static void main(String[] args) throws IllegalTriangleException {
        GeometricObject[] objects = new GeometricObject[5];
        objects[0] = new Square(4.0);
        objects[1] = new Circle(2.5);
        objects[2] = new Square();
        objects[3] = new Rectangle(3.0, 5.0);
        objects[4] = new Triangle(3.0, 4.0, 5.0);

        for (int i = 0; i < objects.length; i++) {
            GeometricObject obj = objects[i];
            System.out.printf(Locale.US, "Объект %d: %s | Площадь = %.2f%n",
                    i + 1, obj.toString(), obj.getArea());
            if (obj instanceof Colorable) {
                ((Colorable) obj).howToColor();
            }
        }
    }
}
```

Результат запуска программы `TestColorableTask4` приведён в листинге 20, место для скриншота — на рисунке 4.

::: {custom-style="ListingCaption"}
Листинг 20 — Вывод программы app.TestColorableTask4
:::

```text
Объект 1: Квадрат (side = 4.00, площадь = 16.00) | Площадь = 16.00
Раскрасьте все четыре стороны.
Объект 2: Круг (radius = 2.50, площадь = 19.63) | Площадь = 19.63
Объект 3: Квадрат (side = 0.00, площадь = 0.00) | Площадь = 0.00
Раскрасьте все четыре стороны.
Объект 4: Прямоугольник (width = 3.00, height = 5.00, площадь = 15.00) | Площадь = 15.00
Объект 5: Треугольник: сторона1 = 3.0 сторона2 = 4.0 сторона3 = 5.0 | Площадь = 6.00
```

::: {custom-style="Captioned Figure"}
[Место для вставки скриншота запуска программы app.TestColorableTask4]
:::

::: {custom-style="Image Caption"}
Рисунок 4 — Результат работы программы app.TestColorableTask4
:::

## 3.5 Часть 2. Задания 1 и 2 — Массивы месяцев, исключения и високосный год

В программе `MonthDaysTask1` (листинг 21) используются массивы названий месяцев `months` и количества дней `dom`. Номер месяца считывается методом `Scanner.nextInt()`. При вводе целого числа вне диапазона от 1 до 12 возникает исключение `ArrayIndexOutOfBoundsException`, в обработчике которого выводится сообщение «Недопустимое число». При попытке ввода нецелого числа или текста перехватывается `InputMismatchException`.

В программе `MonthDaysLeapTask2` (листинг 22) код дополнен статическим методом `isLeapYear(int year)`: год считается високосным, если он делится на 4 и не делится на 100, либо делится на 400. При выборе февраля (число `2`) запрашивается год и для високосного года выводится `29` дней вместо `28`.

::: {custom-style="ListingCaption"}
Листинг 21 — Исходный код файла app/MonthDaysTask1.java
:::

```java
package app;

import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * Часть 2. Задание 1.
 * Вывод названия месяца и числа дней по номеру 1..12 с обработкой
 * ArrayIndexOutOfBoundsException и защитой от ввода нецелого числа.
 */
public class MonthDaysTask1 {
    public static void main(String[] args) {
        String[] months = {
            "январь", "февраль", "март", "апрель", "май", "июнь",
            "июль", "август", "сентябрь", "октябрь", "ноябрь", "декабрь"
        };
        int[] dom = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        Scanner scanner = new Scanner(System.in);
        System.out.print("Введите целое число от 1 до 12: ");

        try {
            int monthNumber = scanner.nextInt();
            String monthName = months[monthNumber - 1];
            int days = dom[monthNumber - 1];
            System.out.println("Месяц: " + monthName + ", количество дней: " + days);
        } catch (ArrayIndexOutOfBoundsException ex) {
            System.out.println("Недопустимое число");
        } catch (InputMismatchException ex) {
            System.out.println("Ошибка ввода: необходимо ввести целое число.");
        } finally {
            scanner.close();
        }
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 22 — Исходный код файла app/MonthDaysLeapTask2.java
:::

```java
package app;

import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * Часть 2. Задание 2.
 * Вывод месяца и количества дней с учётом високосного года для февраля.
 */
public class MonthDaysLeapTask2 {
    public static boolean isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    public static void main(String[] args) {
        String[] months = {
            "январь", "февраль", "март", "апрель", "май", "июнь",
            "июль", "август", "сентябрь", "октябрь", "ноябрь", "декабрь"
        };
        int[] dom = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        Scanner scanner = new Scanner(System.in);
        System.out.print("Введите целое число от 1 до 12: ");

        try {
            int monthNumber = scanner.nextInt();
            String monthName = months[monthNumber - 1];
            int days = dom[monthNumber - 1];

            if (monthNumber == 2) {
                System.out.print("Введите год: ");
                int year = scanner.nextInt();
                if (year <= 0) {
                    System.out.println("Недопустимый год");
                    return;
                }
                boolean leap = isLeapYear(year);
                if (leap) {
                    days = 29;
                }
                System.out.println("Год " + year + (leap ? " — високосный." : " — невисокосный."));
            }

            System.out.println("Месяц: " + monthName + ", количество дней: " + days);
        } catch (ArrayIndexOutOfBoundsException ex) {
            System.out.println("Недопустимое число");
        } catch (InputMismatchException ex) {
            System.out.println("Ошибка ввода: необходимо ввести целое число.");
        } finally {
            scanner.close();
        }
    }
}
```

Результаты тестовых запусков `MonthDaysTask1` и `MonthDaysLeapTask2` на корректных и ошибочных входных данных представлены в листинге 23, место для скриншота — на рисунке 5.

::: {custom-style="ListingCaption"}
Листинг 23 — Вывод программ app.MonthDaysTask1 и app.MonthDaysLeapTask2
:::

```text
--- MonthDaysTask1 ---
Введите целое число от 1 до 12: 8
Месяц: август, количество дней: 31

Введите целое число от 1 до 12: 15
Недопустимое число

Введите целое число от 1 до 12: 3.5
Ошибка ввода: необходимо ввести целое число.

--- MonthDaysLeapTask2 ---
Введите целое число от 1 до 12: 2
Введите год: 2024
Год 2024 — високосный.
Месяц: февраль, количество дней: 29

Введите целое число от 1 до 12: 2
Введите год: 2026
Год 2026 — невисокосный.
Месяц: февраль, количество дней: 28
```

::: {custom-style="Captioned Figure"}
[Место для вставки скриншота запуска программ app.MonthDaysTask1 и app.MonthDaysLeapTask2]
:::

::: {custom-style="Image Caption"}
Рисунок 5 — Результат работы программ части 2 (обработка исключений и високосный год)
:::

::: {custom-style="StructuralHeading"}
ЗАКЛЮЧЕНИЕ
:::

В ходе выполнения практической работы № 4 изучены и практически применены средства объектно-ориентированного программирования и обработки исключений языка Java.

Построена иерархия геометрических фигур на основе абстрактного класса `GeometricObject`, реализованы интерфейсы `Comparable<GeometricObject>` и `Colorable`, статический метод `max()`, а также подклассы `Triangle`, `Circle`, `Rectangle`, `ComparableCircle` и `Square`. Разработано пользовательское проверяемое исключение `IllegalTriangleException`, предотвращающее создание треугольника с недопустимыми сторонами. Во второй части реализована безопасная обработка пользовательского ввода и обращения к массивам месяцев с перехватом `ArrayIndexOutOfBoundsException` и `InputMismatchException`, а также расчётом числа дней в феврале для високосного и невисокосного годов. Все программы успешно скомпилированы и протестированы.
