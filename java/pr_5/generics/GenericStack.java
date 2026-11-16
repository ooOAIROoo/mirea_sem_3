package generics;

import java.util.NoSuchElementException;

/**
 * Часть 1. Задание №2 (пункт 1).
 * Обобщённый стек GenericStack<E>, реализованный на основе массива
 * с автоматическим удвоением ёмкости при заполнении.
 */
public class GenericStack<E> {
    private static final int DEFAULT_CAPACITY = 4;

    private E[] elements;
    private int size;

    @SuppressWarnings("unchecked")
    public GenericStack() {
        this.elements = (E[]) new Object[DEFAULT_CAPACITY];
        this.size = 0;
    }

    @SuppressWarnings("unchecked")
    public GenericStack(int initialCapacity) {
        if (initialCapacity <= 0) {
            throw new IllegalArgumentException("Начальная ёмкость должна быть положительной");
        }
        this.elements = (E[]) new Object[initialCapacity];
        this.size = 0;
    }

    public int getSize() {
        return size;
    }

    public int getCapacity() {
        return elements.length;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public E peek() {
        if (isEmpty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        return elements[size - 1];
    }

    @SuppressWarnings("unchecked")
    public void push(E o) {
        if (size == elements.length) {
            E[] newArray = (E[]) new Object[elements.length * 2];
            System.arraycopy(elements, 0, newArray, 0, elements.length);
            elements = newArray;
        }
        elements[size++] = o;
    }

    public E pop() {
        if (isEmpty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        E top = elements[--size];
        elements[size] = null;
        return top;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("стек: [");
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                sb.append(", ");
            }
            sb.append(elements[i]);
        }
        sb.append("]");
        return sb.toString();
    }
}
