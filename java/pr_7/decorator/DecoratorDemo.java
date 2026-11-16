package decorator;

/**
 * Структурные паттерны. Вариант №4 (Decorator).
 * Демонстрация применения декораторов форматирования текста в любом порядке.
 */
public class DecoratorDemo {
    private static void printLine(String label, Text text) {
        System.out.print(label);
        text.display();
        System.out.println();
    }

    public static void main(String[] args) {
        Text base = new PlainText("Привет, Java!");

        printLine("Обычный текст:                  ", base);
        printLine("Жирный текст:                   ", new BoldTextDecorator(base));
        printLine("Курсивный текст:                ", new ItalicTextDecorator(base));
        printLine("Подчёркнутый текст:             ", new UnderlineTextDecorator(base));

        // Комбинация декораторов в одном порядке: жирный -> курсив -> подчёркнутый
        Text combo1 = new UnderlineTextDecorator(
                new ItalicTextDecorator(
                        new BoldTextDecorator(base)));
        printLine("Порядок (Underline(Italic(Bold))): ", combo1);

        // Комбинация тех же декораторов в другом порядке: курсив -> подчёркнутый -> жирный
        Text combo2 = new BoldTextDecorator(
                new UnderlineTextDecorator(
                        new ItalicTextDecorator(base)));
        printLine("Порядок (Bold(Underline(Italic))): ", combo2);
    }
}
