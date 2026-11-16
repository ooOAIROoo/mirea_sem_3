package app;

import generics.Circle;
import generics.GenericUtils;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * Часть 1. Задание №1.
 * Демонстрация удаления дубликатов, линейного поиска и поиска максимума в 1D и 2D массивах.
 */
public class TestGenericsTask1 {
    public static void main(String[] args) {
        // 1. Удаление дубликатов из ArrayList
        ArrayList<String> words = new ArrayList<>(
                Arrays.asList("Java", "Стек", "Java", "Дженерики", "Стек", "Массив"));
        ArrayList<String> uniqueWords = GenericUtils.removeDuplicates(words);
        System.out.println("1. Удаление дубликатов:");
        System.out.println("   Исходный список: " + words);
        System.out.println("   Без дубликатов:  " + uniqueWords);

        // 2. Линейный поиск в массиве
        Integer[] numbers = {14, 7, 25, 3, 42, 18};
        System.out.println("\n2. Линейный поиск в массиве " + Arrays.toString(numbers) + ":");
        System.out.println("   Позиция элемента 25: " + GenericUtils.linearSearch(numbers, 25));
        System.out.println("   Позиция элемента 99: " + GenericUtils.linearSearch(numbers, 99));

        // 3. Наибольший элемент в массиве экземпляров Circle
        Circle[] circles = {
            new Circle(2.5),
            new Circle(7.2),
            new Circle(1.0),
            new Circle(5.4)
        };
        System.out.println("\n3. Поиск наибольшего круга в массиве " + Arrays.toString(circles) + ":");
        System.out.println("   Наибольший элемент: " + GenericUtils.max(circles));

        // 4. Наибольший элемент в двумерном массиве
        Integer[][] matrix = {
            {4, 12, 9},
            {27, 5, 16},
            {8, 31, 2}
        };
        System.out.println("\n4. Поиск наибольшего элемента в двумерном массиве:");
        for (Integer[] row : matrix) {
            System.out.println("   " + Arrays.toString(row));
        }
        System.out.println("   Наибольший элемент: " + GenericUtils.max(matrix));
    }
}
