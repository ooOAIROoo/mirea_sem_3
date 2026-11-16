import java.util.ArrayList;
import java.util.List;

public class ObserverPatternExample
{
    public static void main(String[] args)
    {
        Subject subject = new Subject();
        User user1 = new User("Братиш");
        User user2 = new User("Кимпентяу");
        User user3 = new User("СубоБратик");
        subject.addObserver(user1);
        subject.addObserver(user2);
        subject.addObserver(user3);
        System.out.println("Событие: Обновление состояния.");
        subject.notifyObservers("Состояние объекта изменено!");
        subject.removeObserver(user2);
        System.out.println("Событие: Другое обновление.");
        subject.notifyObservers("Новое событие произошло!");
    }
}