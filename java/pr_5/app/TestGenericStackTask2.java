package app;

import generics.GenericStack;
import generics.GenericStackInheritance;
import java.util.Scanner;

/**
 * Часть 1. Задание №2.
 * Проверка GenericStack на массиве (с удвоением ёмкости) и GenericStackInheritance (ввод 5 строк).
 */
public class TestGenericStackTask2 {
    public static void main(String[] args) {
        // Пункт 1: проверка стека на массиве и автоматического удвоения размера
        System.out.println("=== Пункт 1. GenericStack<Integer> на массиве ===");
        GenericStack<Integer> arrayStack = new GenericStack<>(2);
        System.out.println("Начальная ёмкость массива: " + arrayStack.getCapacity());
        for (int i = 1; i <= 5; i++) {
            arrayStack.push(i * 10);
            System.out.println("push(" + (i * 10) + ") -> размер = " + arrayStack.getSize()
                    + ", ёмкость = " + arrayStack.getCapacity());
        }
        System.out.println("Содержимое: " + arrayStack);
        System.out.println("Вершина (peek): " + arrayStack.peek());
        System.out.println("Удалён (pop):   " + arrayStack.pop());
        System.out.println("После pop:      " + arrayStack);

        // Пункт 2: стек-наследник ArrayList — ввод 5 строк и вывод в обратном порядке
        System.out.println("\n=== Пункт 2. GenericStackInheritance<String> (5 строк) ===");
        GenericStackInheritance<String> stringStack = new GenericStackInheritance<>();
        Scanner scanner = new Scanner(System.in);
        System.out.println("Введите 5 строк (каждую с новой строки):");
        for (int i = 1; i <= 5; i++) {
            System.out.print("Строка " + i + ": ");
            stringStack.push(scanner.nextLine());
        }
        scanner.close();

        System.out.println("\nСтроки в обратном порядке:");
        while (!stringStack.isEmpty()) {
            System.out.println(stringStack.pop());
        }
    }
}
