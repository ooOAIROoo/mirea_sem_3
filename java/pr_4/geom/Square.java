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
