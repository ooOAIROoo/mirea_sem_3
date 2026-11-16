package mediator;

/**
 * Поведенческие паттерны. Вариант №4 (Mediator).
 * Интерфейс посредника для координации обмена сообщениями.
 */
public interface Mediator {
    void addUser(User user);

    void sendMessage(String message, User sender);
}
