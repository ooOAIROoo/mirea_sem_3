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
