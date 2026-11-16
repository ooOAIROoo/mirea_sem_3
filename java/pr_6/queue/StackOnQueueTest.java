package queue;

/**
 * Задача #2.
 * Тестовый класс для проверки работы стека StackOnQueue.
 */
public class StackOnQueueTest {
    public static void main(String[] args) {
        StackOnQueue stack = new StackOnQueue();

        // 1. Добавление двух значений в стек
        stack.push(10);
        stack.push(25);
        System.out.println("Добавлены элементы: 10, затем 25.");
        System.out.println("Текущее состояние: " + stack);

        // 2. Вывод элемента на вершине стека без удаления
        System.out.println("Вершина стека без удаления (top): " + stack.top());

        // 3. Вывод элемента на вершине стека с удалением
        System.out.println("Вершина стека с удалением (pop):  " + stack.pop());

        // 4. Проверка стека на пустоту
        System.out.println("Стек пуст (empty)? " + stack.empty());

        // 5. Вывод информации обо всех оставшихся элементах стека
        System.out.println("Все элементы стека: " + stack.toString());
    }
}
