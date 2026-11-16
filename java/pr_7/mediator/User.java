package mediator;

/**
 * Поведенческие паттерны. Вариант №4 (Mediator).
 * Класс пользователя чата, отправляющего и получающего сообщения через посредника.
 */
public class User {
    private final Mediator mediator;
    private final String name;

    public User(Mediator mediator, String name) {
        this.mediator = mediator;
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void send(String message) {
        System.out.println("[" + name + " отправляет]: " + message);
        mediator.sendMessage(message, this);
    }

    public void receive(String message, String senderName) {
        System.out.println("  -> [" + name + " <- " + senderName + "]: " + message);
    }
}
