package decorator;

/**
 * Структурные паттерны. Вариант №4 (Decorator).
 * Абстрактный базовый декоратор текста.
 */
public abstract class TextDecorator implements Text {
    protected final Text decoratedText;

    protected TextDecorator(Text decoratedText) {
        this.decoratedText = decoratedText;
    }

    @Override
    public void display() {
        decoratedText.display();
    }
}
