package generics;

import java.util.ArrayList;
import java.util.NoSuchElementException;

/**
 * Часть 1. Задание №2 (пункт 2).
 * Обобщённый стек, реализованный через наследование от java.util.ArrayList<E>.
 */
public class GenericStackInheritance<E> extends ArrayList<E> {
    private static final long serialVersionUID = 1L;

    public int getSize() {
        return super.size();
    }

    public E peek() {
        if (isEmpty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        return super.get(getSize() - 1);
    }

    public void push(E o) {
        super.add(o);
    }

    public E pop() {
        if (isEmpty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        return super.remove(getSize() - 1);
    }

    @Override
    public boolean isEmpty() {
        return super.isEmpty();
    }

    @Override
    public String toString() {
        return "стек: " + super.toString();
    }
}
