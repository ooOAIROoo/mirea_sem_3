package generics;

import java.util.ArrayList;
import java.util.Objects;

/**
 * Часть 1. Задание №1 (пункты 1-4).
 * Набор обобщённых методов для работы с коллекциями и массивами.
 */
public class GenericUtils {
    /**
     * 1. Возвращает новый список ArrayList без дубликатов с использованием List.contains().
     */
    public static <E> ArrayList<E> removeDuplicates(ArrayList<E> list) {
        ArrayList<E> result = new ArrayList<>();
        for (E element : list) {
            if (!result.contains(element)) {
                result.add(element);
            }
        }
        return result;
    }

    /**
     * 2. Линейный поиск элемента в массиве. Возвращает индекс или -1, если элемент не найден.
     */
    public static <E> int linearSearch(E[] array, E key) {
        for (int i = 0; i < array.length; i++) {
            if (Objects.equals(array[i], key)) {
                return i;
            }
        }
        return -1;
    }

    /**
     * 3. Поиск наибольшего элемента в одномерном массиве с помощью Comparable.compareTo().
     */
    public static <E extends Comparable<E>> E max(E[] array) {
        if (array == null || array.length == 0) {
            throw new IllegalArgumentException("Массив не должен быть пустым");
        }
        E maxElement = array[0];
        for (int i = 1; i < array.length; i++) {
            if (array[i].compareTo(maxElement) > 0) {
                maxElement = array[i];
            }
        }
        return maxElement;
    }

    /**
     * 4. Поиск наибольшего элемента в двумерном массиве с помощью Comparable.compareTo().
     */
    public static <E extends Comparable<E>> E max(E[][] matrix) {
        if (matrix == null || matrix.length == 0) {
            throw new IllegalArgumentException("Двумерный массив не должен быть пустым");
        }
        E maxElement = null;
        for (E[] row : matrix) {
            if (row == null) {
                continue;
            }
            for (E element : row) {
                if (maxElement == null || element.compareTo(maxElement) > 0) {
                    maxElement = element;
                }
            }
        }
        if (maxElement == null) {
            throw new IllegalArgumentException("Двумерный массив не содержит элементов");
        }
        return maxElement;
    }
}
