package mystack;

import java.util.ArrayList;
import java.util.NoSuchElementException;

/**
 * Часть 2. Задание №1 (пункт 1).
 * Класс MyStack, реализованный через наследование от ArrayList<Object>.
 */
public class MyStackInheritance extends ArrayList<Object> {
    private static final long serialVersionUID = 1L;

    @Override
    public boolean isEmpty() {
        return super.isEmpty();
    }

    public int getSize() {
        return super.size();
    }

    public Object peek() {
        if (isEmpty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        return super.get(getSize() - 1);
    }

    public Object pop() {
        if (isEmpty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        return super.remove(getSize() - 1);
    }

    public void push(Object o) {
        super.add(o);
    }

    @Override
    public String toString() {
        return "стек: " + super.toString();
    }
}
