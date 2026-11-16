package mystack;

import java.util.ArrayList;
import java.util.Date;
import java.util.NoSuchElementException;

/**
 * Часть 2. Задание №1.
 * Класс MyStack на основе композиции с поддержкой глубокого копирования поля list.
 */
public class MyStack implements Cloneable {
    private ArrayList<Object> list = new ArrayList<>();

    public boolean isEmpty() {
        return list.isEmpty();
    }

    public int getSize() {
        return list.size();
    }

    public Object peek() {
        if (isEmpty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        return list.get(getSize() - 1);
    }

    public Object pop() {
        if (isEmpty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        return list.remove(getSize() - 1);
    }

    public void push(Object o) {
        list.add(o);
    }

    @Override
    public Object clone() {
        try {
            // Поверхностная копия объекта MyStack
            MyStack stackClone = (MyStack) super.clone();
            // Глубокая копия поля списка list
            stackClone.list = copyList();
            return stackClone;
        } catch (CloneNotSupportedException ex) {
            return null;
        }
    }

    private ArrayList<Object> copyList() {
        ArrayList<Object> newList = new ArrayList<>(list.size());
        for (Object item : list) {
            if (item instanceof Date) {
                newList.add(((Date) item).clone());
            } else if (item instanceof MyStack) {
                newList.add(((MyStack) item).clone());
            } else {
                newList.add(item);
            }
        }
        return newList;
    }

    @Override
    public String toString() {
        return "стек: " + list.toString();
    }
}
