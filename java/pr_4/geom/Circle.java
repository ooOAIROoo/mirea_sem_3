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
