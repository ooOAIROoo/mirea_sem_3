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
