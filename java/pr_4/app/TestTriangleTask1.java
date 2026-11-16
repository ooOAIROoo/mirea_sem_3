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
