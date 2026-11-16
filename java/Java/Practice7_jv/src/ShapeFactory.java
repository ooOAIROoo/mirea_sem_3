import java.util.HashMap;

class ShapeFactory
{
    private static final HashMap<String, Shape> circleMap = new HashMap<>();

    public static Shape getCircle(String color)
    {
        Circle circle = (Circle) circleMap.get(color);
        if (circle == null)
        {
            circle = new Circle(color);
            circleMap.put(color, circle);
            System.out.println("Создание круга с цветом: " + color);
        }
        return circle;
    }
}
