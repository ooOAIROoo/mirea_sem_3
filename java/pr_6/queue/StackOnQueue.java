package queue;

import java.util.LinkedList;
import java.util.NoSuchElementException;
import java.util.Queue;

/**
 * Задача #1.
 * Реализация стека (LIFO) на основе двух очередей (FIFO) с использованием композиции
 * и только стандартных методов очереди: offer, peek, poll, size, isEmpty.
 */
public class StackOnQueue {
    private Queue<Integer> mainQueue;
    private Queue<Integer> helperQueue;

    public StackOnQueue() {
        this.mainQueue = new LinkedList<>();
        this.helperQueue = new LinkedList<>();
    }

    /** Помещает элемент x на вершину стека. */
    public void push(int x) {
        helperQueue.offer(x);
        while (!mainQueue.isEmpty()) {
            helperQueue.offer(mainQueue.poll());
        }
        Queue<Integer> temp = mainQueue;
        mainQueue = helperQueue;
        helperQueue = temp;
    }

    /** Удаляет элемент на вершине стека и возвращает его. */
    public int pop() {
        if (empty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        return mainQueue.poll();
    }

    /** Возвращает элемент на вершине стека без удаления. */
    public int top() {
        if (empty()) {
            throw new NoSuchElementException("Стек пуст");
        }
        return mainQueue.peek();
    }

    /** Возвращает true, если стек пуст, иначе false. */
    public boolean empty() {
        return mainQueue.isEmpty();
    }

    /** Возвращает строковое представление всех элементов стека (от вершины к дну). */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("StackOnQueue (вершина -> дно): [");
        int count = mainQueue.size();
        for (int i = 0; i < count; i++) {
            int value = mainQueue.poll();
            if (i > 0) {
                sb.append(", ");
            }
            sb.append(value);
            mainQueue.offer(value);
        }
        sb.append("]");
        return sb.toString();
    }
}
