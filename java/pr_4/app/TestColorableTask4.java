package app;

import geom.Circle;
import geom.Colorable;
import geom.GeometricObject;
import geom.Rectangle;
import geom.Square;
import geom.Triangle;
import geom_basic.IllegalTriangleException;
import java.util.Locale;

/**
 * Часть 1. Задание №4.
 * Массив из пяти объектов GeometricObject: вывод площади и вызов howToColor() для Colorable.
 */
public class TestColorableTask4 {
    public static void main(String[] args) throws IllegalTriangleException {
        GeometricObject[] objects = new GeometricObject[5];
        objects[0] = new Square(4.0);
        objects[1] = new Circle(2.5);
        objects[2] = new Square();
        objects[3] = new Rectangle(3.0, 5.0);
        objects[4] = new Triangle(3.0, 4.0, 5.0);

        for (int i = 0; i < objects.length; i++) {
            GeometricObject obj = objects[i];
            System.out.printf(Locale.US, "Объект %d: %s | Площадь = %.2f%n",
                    i + 1, obj.toString(), obj.getArea());
            if (obj instanceof Colorable) {
                ((Colorable) obj).howToColor();
            }
        }
    }
}
