package decorator;

/**
 * Декоратор для курсивного начертания текста.
 */
public class ItalicTextDecorator extends TextDecorator {
    public ItalicTextDecorator(Text decoratedText) {
        super(decoratedText);
    }

    @Override
    public void display() {
        System.out.print("<i>");
        super.display();
        System.out.print("</i>");
    }
}
