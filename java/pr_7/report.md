---
lang: ru-RU
toc-title: СОДЕРЖАНИЕ
---

::: {custom-style="StructuralHeading"}
ВВЕДЕНИЕ
:::

Настоящий отчёт посвящён выполнению практической работы № 7 по дисциплине «Программирование на языке Java» на тему «Паттерны проектирования: структурные и поведенческие паттерны».

Целью работы является изучение принципов построения структурных и поведенческих паттернов проектирования программного обеспечения и их практическая реализация на языке Java по варианту № 4: структурный паттерн `Decorator` («Декоратор») и поведенческий паттерн `Mediator` («Посредник»).

Для достижения поставленной цели решаются следующие задачи:

- разработать систему форматирования текста на основе паттерна `Decorator` с интерфейсом `Text`, базовым классом `PlainText` и декораторами жирного, курсивного и подчёркнутого начертания, применимыми в произвольном порядке;
- разработать систему чата на основе паттерна `Mediator` с интерфейсом `Mediator`, конкретным посредником `ChatRoom` и классом пользователя `User`;
- продемонстрировать работу обеих подсистем с помощью тестовых программ.

# 1 Теоретические сведения

## 1.1 Структурные паттерны проектирования и паттерн Decorator

Структурные паттерны (`Structural Patterns`) определяют способы объединения классов и объектов в более крупные и гибкие структуры при сохранении низкой связности и удобства сопровождения кода. К основным структурным паттернам относятся `Adapter`, `Bridge`, `Composite`, `Decorator`, `Facade`, `Flyweight` и `Proxy`.

Паттерн `Decorator` («Декоратор») позволяет динамически добавлять объекту новые обязанности без изменения исходного кода базового класса и без порождения множества подклассов под каждую комбинацию функций. Абстрактный декоратор одновременно реализует общий интерфейс компонента и содержит ссылку на вложенный объект того же интерфейса, делегируя ему основной вызов и дополняя поведение до или после него.

## 1.2 Поведенческие паттерны проектирования и паттерн Mediator

Поведенческие паттерны (`Behavioral Patterns`) описывают схемы взаимодействия объектов и распределения обязанностей между ними. К этой группе относятся `Chain of Responsibility`, `Command`, `Iterator`, `Mediator`, `Memento`, `Observer`, `State`, `Strategy`, `Template Method` и `Visitor`.

Паттерн `Mediator` («Посредник») обеспечивает централизованное взаимодействие множества объектов: участники системы не ссылаются друг на друга напрямую, а обмениваются сообщениями через единый объект-посредник. Сравнение применённых в работе паттернов представлено в таблице 1.

Table: Таблица 1 — Характеристика реализованных паттернов проектирования (вариант № 4)

| Паттерн | Группа | Основная идея | Роль в практической работе |
|---|---|---|---|
| `Decorator` | Структурный | Послойное оборачивание объекта для динамического добавления функций | Комбинирование жирного, курсивного и подчёркнутого форматирования текста |
| `Mediator` | Поведенческий | Централизация обмена сообщениями между объектами | Координация переписки пользователей через комнату чата `ChatRoom` |

# 2 Постановка задачи и структура проекта

В соответствии с вариантом № 4 выполняются два практических задания:

- структурный паттерн `Decorator` — создать интерфейс `Text` с методом `display()`, базовый класс простого текста и декораторы жирного, курсивного и подчёркнутого форматирования, применимые в любом порядке;
- поведенческий паттерн `Mediator` — создать интерфейс `Mediator`, конкретный класс посредника и класс `User`, обеспечивающие отправку и получение сообщений между несколькими пользователями чата через посредника.

Исходный код разделён на два пакета — `decorator` и `mediator`. Состав файлов проекта представлен в таблице 2.

Table: Таблица 2 — Структура проекта практической работы № 7

| Файл | Пакет | Назначение |
|---|---|---|
| `decorator/Text.java` | `decorator` | Базовый интерфейс текста с методом `display()` |
| `decorator/PlainText.java` | `decorator` | Базовый класс обычного текста без форматирования |
| `decorator/TextDecorator.java` | `decorator` | Абстрактный декоратор, оборачивающий объект `Text` |
| `decorator/BoldTextDecorator.java` | `decorator` | Декоратор жирного начертания (`<b>...</b>`) |
| `decorator/ItalicTextDecorator.java` | `decorator` | Декоратор курсивного начертания (`<i>...</i>`) |
| `decorator/UnderlineTextDecorator.java` | `decorator` | Декоратор подчёркнутого текста (`<u>...</u>`) |
| `decorator/DecoratorDemo.java` | `decorator` | Демонстрация применения декораторов в разном порядке |
| `mediator/Mediator.java` | `mediator` | Интерфейс посредника чата |
| `mediator/ChatRoom.java` | `mediator` | Конкретный посредник, рассылающий сообщения участникам |
| `mediator/User.java` | `mediator` | Класс пользователя чата (отправка и приём сообщений) |
| `mediator/MediatorDemo.java` | `mediator` | Демонстрация обмена сообщениями между пользователями |

Команда компиляции проекта из каталога `java/pr_7` приведена в листинге 1.

::: {custom-style="ListingCaption"}
Листинг 1 — Команда компиляции проекта практической работы № 7
:::

```text
javac -encoding UTF-8 -d /tmp/pr7-classes decorator/*.java mediator/*.java
```

# 3 Ход выполнения работы

## 3.1 Структурный паттерн Decorator (вариант 4) — Форматирование текста

Определён интерфейс `Text` с методом `void display()` (листинг 2) и класс `PlainText` (листинг 3), выводящий исходную строку. Абстрактный класс `TextDecorator` (листинг 4) хранит защищённую ссылку `decoratedText` типа `Text` и делегирует ей вызов `display()`. Конкретные декораторы `BoldTextDecorator` (листинг 5), `ItalicTextDecorator` (листинг 6) и `UnderlineTextDecorator` (листинг 7) обрамляют вывод вложенного объекта соответствующими тегами разметки `<b>...</b>`, `<i>...</i>` и `<u>...</u>`. Поскольку каждый декоратор принимает и реализует тип `Text`, декораторы свободно вкладываются друг в друга в произвольной последовательности, что продемонстрировано в классе `DecoratorDemo` (листинг 8).

::: {custom-style="ListingCaption"}
Листинг 2 — Исходный код файла decorator/Text.java
:::

```java
package decorator;

/**
 * Структурные паттерны. Вариант №4 (Decorator).
 * Базовый интерфейс текстового компонента.
 */
public interface Text {
    void display();
}
```

::: {custom-style="ListingCaption"}
Листинг 3 — Исходный код файла decorator/PlainText.java
:::

```java
package decorator;

/**
 * Структурные паттерны. Вариант №4 (Decorator).
 * Базовый класс для обычного текста без форматирования.
 */
public class PlainText implements Text {
    private final String content;

    public PlainText(String content) {
        this.content = content;
    }

    @Override
    public void display() {
        System.out.print(content);
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 4 — Исходный код файла decorator/TextDecorator.java
:::

```java
package decorator;

/**
 * Структурные паттерны. Вариант №4 (Decorator).
 * Абстрактный базовый декоратор текста.
 */
public abstract class TextDecorator implements Text {
    protected final Text decoratedText;

    protected TextDecorator(Text decoratedText) {
        this.decoratedText = decoratedText;
    }

    @Override
    public void display() {
        decoratedText.display();
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 5 — Исходный код файла decorator/BoldTextDecorator.java
:::

```java
package decorator;

/**
 * Декоратор для жирного начертания текста.
 */
public class BoldTextDecorator extends TextDecorator {
    public BoldTextDecorator(Text decoratedText) {
        super(decoratedText);
    }

    @Override
    public void display() {
        System.out.print("<b>");
        super.display();
        System.out.print("</b>");
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 6 — Исходный код файла decorator/ItalicTextDecorator.java
:::

```java
package decorator;

/**
 * Декоратор для курсивного начертания текста.
 */
public class ItalicTextDecorator extends TextDecorator {
    public ItalicTextDecorator(Text decoratedText) {
        super(decoratedText);
    }

    @Override
    public void display() {
        System.out.print("<i>");
        super.display();
        System.out.print("</i>");
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 7 — Исходный код файла decorator/UnderlineTextDecorator.java
:::

```java
package decorator;

/**
 * Декоратор для подчёркнутого текста.
 */
public class UnderlineTextDecorator extends TextDecorator {
    public UnderlineTextDecorator(Text decoratedText) {
        super(decoratedText);
    }

    @Override
    public void display() {
        System.out.print("<u>");
        super.display();
        System.out.print("</u>");
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 8 — Исходный код файла decorator/DecoratorDemo.java
:::

```java
package decorator;

/**
 * Структурные паттерны. Вариант №4 (Decorator).
 * Демонстрация применения декораторов форматирования текста в любом порядке.
 */
public class DecoratorDemo {
    private static void printLine(String label, Text text) {
        System.out.print(label);
        text.display();
        System.out.println();
    }

    public static void main(String[] args) {
        Text base = new PlainText("Привет, Java!");

        printLine("Обычный текст:                  ", base);
        printLine("Жирный текст:                   ", new BoldTextDecorator(base));
        printLine("Курсивный текст:                ", new ItalicTextDecorator(base));
        printLine("Подчёркнутый текст:             ", new UnderlineTextDecorator(base));

        // Комбинация декораторов в одном порядке: жирный -> курсив -> подчёркнутый
        Text combo1 = new UnderlineTextDecorator(
                new ItalicTextDecorator(
                        new BoldTextDecorator(base)));
        printLine("Порядок (Underline(Italic(Bold))): ", combo1);

        // Комбинация тех же декораторов в другом порядке: курсив -> подчёркнутый -> жирный
        Text combo2 = new BoldTextDecorator(
                new UnderlineTextDecorator(
                        new ItalicTextDecorator(base)));
        printLine("Порядок (Bold(Underline(Italic))): ", combo2);
    }
}
```

Консольный вывод программы `DecoratorDemo` приведён в листинге 9, место для скриншота — на рисунке 1.

::: {custom-style="ListingCaption"}
Листинг 9 — Вывод программы decorator.DecoratorDemo
:::

```text
Обычный текст:                  Привет, Java!
Жирный текст:                   <b>Привет, Java!</b>
Курсивный текст:                <i>Привет, Java!</i>
Подчёркнутый текст:             <u>Привет, Java!</u>
Порядок (Underline(Italic(Bold))): <u><i><b>Привет, Java!</b></i></u>
Порядок (Bold(Underline(Italic))): <b><u><i>Привет, Java!</i></u></b>
```

::: {custom-style="Captioned Figure"}
![](<screenshots/20261124_16h43m39s-th.png>){width=15.5cm}
:::

::: {custom-style="Image Caption"}
Рисунок 1 — Результат работы программы decorator.DecoratorDemo (паттерн Decorator, вариант 4)
:::

## 3.2 Поведенческий паттерн Mediator (вариант 4) — Система чата

В пакете `mediator` объявлен интерфейс `Mediator` с методами регистрации пользователя `addUser()` и отправки сообщения `sendMessage()` (листинг 10). Класс `ChatRoom` (листинг 11) хранит список подключённых участников `users` и при получении сообщения пересылает его всем пользователям, кроме отправителя (`user != sender`). Класс `User` (листинг 12) взаимодействует исключительно с посредником через метод `send()` и принимает входящие сообщения в методе `receive()`. Демонстрация переписки трёх пользователей реализована в классе `MediatorDemo` (листинг 13).

::: {custom-style="ListingCaption"}
Листинг 10 — Исходный код файла mediator/Mediator.java
:::

```java
package mediator;

/**
 * Поведенческие паттерны. Вариант №4 (Mediator).
 * Интерфейс посредника для координации обмена сообщениями.
 */
public interface Mediator {
    void addUser(User user);

    void sendMessage(String message, User sender);
}
```

::: {custom-style="ListingCaption"}
Листинг 11 — Исходный код файла mediator/ChatRoom.java
:::

```java
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
```

::: {custom-style="ListingCaption"}
Листинг 12 — Исходный код файла mediator/User.java
:::

```java
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
```

::: {custom-style="ListingCaption"}
Листинг 13 — Исходный код файла mediator/MediatorDemo.java
:::

```java
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
```

Консольный вывод программы `MediatorDemo` приведён в листинге 14, место для скриншота — на рисунке 2.

::: {custom-style="ListingCaption"}
Листинг 14 — Вывод программы mediator.MediatorDemo
:::

```text
[Алексей отправляет]: Всем привет! Кто сделал практику по Java?
  -> [Мария <- Алексей]: Всем привет! Кто сделал практику по Java?
  -> [Дмитрий <- Алексей]: Всем привет! Кто сделал практику по Java?

[Мария отправляет]: Привет, Алексей! Я уже закончила четвёртый вариант.
  -> [Алексей <- Мария]: Привет, Алексей! Я уже закончила четвёртый вариант.
  -> [Дмитрий <- Мария]: Привет, Алексей! Я уже закончила четвёртый вариант.

[Дмитрий отправляет]: Отлично, сейчас тоже допишу отчёт.
  -> [Алексей <- Дмитрий]: Отлично, сейчас тоже допишу отчёт.
  -> [Мария <- Дмитрий]: Отлично, сейчас тоже допишу отчёт.
```

::: {custom-style="Captioned Figure"}
![](<screenshots/20261124_16h45m21s-th.png>){width=15.5cm}
:::

::: {custom-style="Image Caption"}
Рисунок 2 — Результат работы программы mediator.MediatorDemo (паттерн Mediator, вариант 4)
:::

::: {custom-style="StructuralHeading"}
ЗАКЛЮЧЕНИЕ
:::

В ходе выполнения практической работы № 7 изучены и практически реализованы структурный и поведенческий паттерны проектирования варианта № 4 в языке Java.

С применением структурного паттерна `Decorator` разработана расширяемая система форматирования текста на базе интерфейса `Text`, класса `PlainText` и трёх конкретных декораторов (`BoldTextDecorator`, `ItalicTextDecorator`, `UnderlineTextDecorator`), допускающих произвольный порядок вложения без модификации исходных классов. С применением поведенческого паттерна `Mediator` реализована система чата (`Mediator`, `ChatRoom`, `User`), обеспечивающая централизованную рассылку сообщений между участниками и устраняющая прямые зависимости между объектами пользователей.
