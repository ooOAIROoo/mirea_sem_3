package geom_basic;

import java.util.Date;

/**
 * Базовый класс геометрической фигуры (Задания №1 и №2).
 */
public class GeometricObject {
    private String color = "белый";
    private boolean filled;
    private Date dateCreated;

    public GeometricObject() {
        this.dateCreated = new Date();
    }

    public GeometricObject(String color, boolean filled) {
        this.dateCreated = new Date();
        this.color = color;
        this.filled = filled;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public boolean isFilled() {
        return filled;
    }

    public void setFilled(boolean filled) {
        this.filled = filled;
    }

    public Date getDateCreated() {
        return dateCreated;
    }

    @Override
    public String toString() {
        return "создан " + dateCreated + ", цвет: " + color + ", закрашен: " + filled;
    }
}
