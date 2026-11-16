package mediator;

import java.util.ArrayList;
import java.util.List;

/**
 * Поведенческие паттерны. Вариант №4 (Mediator).
 * Конкретный посредник — комната чата, пересылающая сообщения участникам.
 */
public class ChatRoom implements Mediator {
    private final List<User> users = new ArrayList<>();

    @Override
    public void addUser(User user) {
        if (user != null && !users.contains(user)) {
            users.add(user);
        }
    }

    @Override
    public void sendMessage(String message, User sender) {
        for (User user : users) {
            if (user != sender) {
                user.receive(message, sender.getName());
            }
        }
    }
}
