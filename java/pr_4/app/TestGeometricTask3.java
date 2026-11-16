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
