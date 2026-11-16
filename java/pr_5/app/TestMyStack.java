package app;

import java.util.Date;
import java.util.Scanner;
import mystack.MyStack;
import mystack.MyStackInheritance;

/**
 * Часть 2. Задание №1.
 * Клиент классов MyStack и MyStackInheritance (5 строк в обратном порядке)
 * и проверка глубокого клонирования MyStack.
 */
public class TestMyStack {
    public static void main(String[] args) {
        // 1. Ввод 5 строк и вывод в обратном порядке (MyStack и MyStackInheritance)
        MyStack compositionStack = new MyStack();
        MyStackInheritance inheritanceStack = new MyStackInheritance();

        Scanner scanner = new Scanner(System.in);
        System.out.println("=== Пункт 1. Ввод 5 строк для MyStack ===");
        for (int i = 1; i <= 5; i++) {
            System.out.print("Строка " + i + ": ");
            String line = scanner.nextLine();
            compositionStack.push(line);
            inheritanceStack.push(line);
        }
        scanner.close();

        System.out.println("\nВывод из MyStack (композиция) в обратном порядке:");
        while (!compositionStack.isEmpty()) {
            System.out.println(compositionStack.pop());
        }

        System.out.println("\nВывод из MyStackInheritance (наследование) в обратном порядке:");
        while (!inheritanceStack.isEmpty()) {
            System.out.println(inheritanceStack.pop());
        }

        // 2. Проверка глубокой копии MyStack
        System.out.println("\n=== Пункт 2. Проверка глубокой копии MyStack.clone() ===");
        MyStack original = new MyStack();
        Date sampleDate = new Date(1_700_000_000_000L);
        original.push("Первый");
        original.push("Второй");
        original.push(sampleDate);

        MyStack copy = (MyStack) original.clone();
        System.out.println("original == copy: " + (original == copy));
        System.out.println("Дата на вершине один и тот же объект? "
                + (original.peek() == copy.peek()));

        // Модифицируем копию и проверяем, что оригинал не изменился
        copy.pop();
        copy.push("Изменённый элемент");
        copy.push("Дополнительный элемент");

        System.out.println("Оригинальный стек (размер = " + original.getSize() + "): " + original);
        System.out.println("Клонированный стек (размер = " + copy.getSize() + "): " + copy);
    }
}
