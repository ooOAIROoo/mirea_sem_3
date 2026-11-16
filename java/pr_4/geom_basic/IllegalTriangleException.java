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
