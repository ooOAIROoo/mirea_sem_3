package generics;

import java.util.Locale;

/**
 * Часть 1. Задание №1 (пункт 3).
 * Класс круга с полем radius, реализующий интерфейс Comparable<Circle>.
 */
public class Circle implements Comparable<Circle> {
    private double radius;

    public Circle(double radius) {
        this.radius = radius;
    }

    public double getRadius() {
        return radius;
    }

    public void setRadius(double radius) {
        this.radius = radius;
    }

    @Override
    public int compareTo(Circle other) {
        return Double.compare(this.radius, other.radius);
    }

    @Override
    public String toString() {
        return String.format(Locale.US, "Circle(radius=%.2f)", radius);
    }
}
