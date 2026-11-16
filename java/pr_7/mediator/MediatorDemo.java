package mediator;

/**
 * Поведенческие паттерны. Вариант №4 (Mediator).
 * Демонстрация обмена сообщениями между пользователями через посредника ChatRoom.
 */
public class MediatorDemo {
    public static void main(String[] args) {
        Mediator chat = new ChatRoom();

        User alexey = new User(chat, "Алексей");
        User maria = new User(chat, "Мария");
        User dmitry = new User(chat, "Дмитрий");

        chat.addUser(alexey);
        chat.addUser(maria);
        chat.addUser(dmitry);

        alexey.send("Всем привет! Кто сделал практику по Java?");
        System.out.println();
        maria.send("Привет, Алексей! Я уже закончила четвёртый вариант.");
        System.out.println();
        dmitry.send("Отлично, сейчас тоже допишу отчёт.");
    }
}
