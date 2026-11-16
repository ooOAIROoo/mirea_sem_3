package decorator;

/**
 * Декоратор для жирного начертания текста.
 */
public class BoldTextDecorator extends TextDecorator {
    public BoldTextDecorator(Text decoratedText) {
        super(decoratedText);
    }

    @Override
    public void display() {
        System.out.print("<b>");
        super.display();
        System.out.print("</b>");
    }
}
