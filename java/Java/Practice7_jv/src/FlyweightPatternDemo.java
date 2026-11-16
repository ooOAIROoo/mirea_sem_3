import java.util.HashMap;

public class FlyweightPatternDemo
{
    private static final String[] colors = {"красный", "зелёный", "синий", "белый", "чёрный", "жёлтый", "оранжевый",
            "фиолетовый", "розовый", "серый", "коричневый", "голубой", "пурпурный", "лавандовый", "бордовый"};

    public static void main(String[] args)
    {
        for (int i = 0; i < 10; i++)
        {
            Circle circle = (Circle) ShapeFactory.getCircle(getRandomColor());
            circle.setPosition(getRandomX(), getRandomY(), 100);
            circle.draw();
        }
    }

    private static String getRandomColor()
    { return colors[(int) (Math.random() * colors.length)]; }

    private static int getRandomX()
    { return (int) (Math.random() * 100); }

    private static int getRandomY()
    { return (int) (Math.random() * 100); }
}