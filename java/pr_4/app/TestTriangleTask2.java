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
