class Circle implements Shape
{
    private String color;
    private int x, y, radius;

    public Circle(String color)
    { this.color = color; }

    public void setPosition(int x, int y, int radius)
    { this.x = x; this.y = y; this.radius = radius; }

    public void draw()
    { System.out.printf
            ("""
            Круг:
               цвет    :  %s;
               x       :  %d;
               y       :  %d;
               радиус  :  %d.
            """,
                    color, x, y, radius); }


}