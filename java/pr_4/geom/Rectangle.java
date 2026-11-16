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
