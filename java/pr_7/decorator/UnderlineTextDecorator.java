package decorator;

/**
 * Декоратор для подчёркнутого текста.
 */
public class UnderlineTextDecorator extends TextDecorator {
    public UnderlineTextDecorator(Text decoratedText) {
        super(decoratedText);
    }

    @Override
    public void display() {
        System.out.print("<u>");
        super.display();
        System.out.print("</u>");
    }
}
