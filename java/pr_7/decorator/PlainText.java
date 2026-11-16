package decorator;

/**
 * Структурные паттерны. Вариант №4 (Decorator).
 * Базовый класс для обычного текста без форматирования.
 */
public class PlainText implements Text {
    private final String content;

    public PlainText(String content) {
        this.content = content;
    }

    @Override
    public void display() {
        System.out.print(content);
    }
}
