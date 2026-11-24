---
lang: ru-RU
toc-title: СОДЕРЖАНИЕ
---

::: {custom-style="StructuralHeading"}
ВВЕДЕНИЕ
:::

Настоящий отчёт посвящён выполнению практической работы № 6 по дисциплине «Программирование на языке Java» на тему «Абстрактные типы данных: очередь. Паттерны проектирования, порождающие паттерны».

Целью работы является изучение абстрактного типа данных «очередь» (интерфейс `java.util.Queue`) и принципов применения порождающих паттернов проектирования программного обеспечения на примере паттерна `Builder` («Строитель», вариант № 4).

Для достижения поставленной цели решаются следующие задачи:

- реализовать класс стека `StackOnQueue` (принцип LIFO) на основе композиции не более чем с двумя очередями (принцип FIFO), используя исключительно стандартные методы очереди (`offer`, `peek`, `poll`, `size`, `isEmpty`);
- разработать тестовый класс `StackOnQueueTest` для проверки операций добавления, чтения и удаления вершины, проверки пустоты и строкового вывода элементов стека;
- реализовать систему формирования заказа в ресторане с применением порождающего паттерна `Builder` (вариант № 4): класс заказа `Order`, строитель `OrderBuilder` и демонстрационную программу сборки заказов с различными конфигурациями.

# 1 Теоретические сведения

## 1.1 Абстрактный тип данных очередь

Очередь (`java.util.Queue<E>`) — коллекция, обычно упорядочивающая элементы по принципу FIFO (`First In, First Out` — «первым пришёл — первым ушёл»): новые элементы добавляются в конец очереди, а чтение и удаление производятся из её начала. Основные стандартные операции интерфейса `Queue` приведены в таблице 1.

Table: Таблица 1 — Стандартные операции интерфейса java.util.Queue

| Метод | Назначение |
|---|---|
| `add(e)` / `offer(e)` | Добавляет элемент в конец очереди |
| `peek()` | Возвращает первый элемент очереди без удаления (или `null`, если очередь пуста) |
| `poll()` | Удаляет и возвращает первый элемент очереди (или `null`, если очередь пуста) |
| `size()` | Возвращает текущее количество элементов в очереди |
| `isEmpty()` | Возвращает `true`, если очередь не содержит элементов |

Для моделирования стека (LIFO) с помощью двух очередей FIFO при каждом вызове `push(x)` новый элемент помещается в пустую вспомогательную очередь, после чего за ним последовательно перекладываются все элементы основной очереди, и ссылки на очереди меняются местами. Таким образом, вершина стека постоянно располагается в голове основной очереди, а операции `top()` и `pop()` выполняются за время $O(1)$.

## 1.2 Порождающие паттерны проектирования и паттерн Builder

Порождающие паттерны (`Creational Patterns`) инкапсулируют логику создания объектов, отделяя процесс конструирования от использования и повышая гибкость программы. К основным порождающим паттернам относятся `Singleton` («Одиночка»), `Factory Method` («Фабричный метод»), `Abstract Factory` («Абстрактная фабрика»), `Builder` («Строитель») и `Prototype` («Прототип»).

Паттерн `Builder` отделяет пошаговое построение сложного объекта от его представления. Вместо набора перегруженных конструкторов со множеством необязательных параметров используется отдельный класс-строитель, методы которого задают нужные поля объекта и возвращают ссылку на самого строителя (`this`), а завершающий метод `build()` возвращает готовый экземпляр продукта.

# 2 Постановка задачи и структура проекта

В первой части работы выполняются общие задачи 1 и 2 по реализации и тестированию класса `StackOnQueue`. Во второй части выполняется практическое задание варианта № 4 (паттерн `Builder`): разработать систему создания заказа в ресторане `Order` с параметрами «основное блюдо», «гарнир», «напиток» и «десерт», класс `OrderBuilder` для пошаговой сборки и демонстрационную программу сборки заказов с разными конфигурациями.

Исходный код разделён на два пакета — `queue` и `builder`. Состав файлов проекта представлен в таблице 2.

Table: Таблица 2 — Структура проекта практической работы № 6

| Файл | Пакет | Назначение |
|---|---|---|
| `queue/StackOnQueue.java` | `queue` | Стек LIFO на основе композиции с двумя очередями FIFO (задача 1) |
| `queue/StackOnQueueTest.java` | `queue` | Тестовый класс для проверки всех операций стека (задача 2) |
| `builder/Order.java` | `builder` | Класс заказа в ресторане с четырьмя параметрами (вариант 4) |
| `builder/OrderBuilder.java` | `builder` | Класс строителя для пошагового формирования заказа (вариант 4) |
| `builder/RestaurantDemo.java` | `builder` | Демонстрация сборки заказов разных конфигураций (вариант 4) |

Команда компиляции проекта из каталога `java/pr_6` приведена в листинге 1.

::: {custom-style="ListingCaption"}
Листинг 1 — Команда компиляции проекта практической работы № 6
:::

```text
javac -encoding UTF-8 -d /tmp/pr6-classes queue/*.java builder/*.java
```

# 3 Ход выполнения работы

## 3.1 Задачи 1 и 2 — Реализация и тестирование класса StackOnQueue

Класс `StackOnQueue` (листинг 2) хранит две очереди `mainQueue` и `helperQueue` типа `Queue<Integer>` (реализация `LinkedList<>`), что соответствует отношению композиции. Во всех методах, включая формирование строкового представления в `toString()`, задействованы исключительно стандартные методы `offer()`, `peek()`, `poll()`, `size()` и `isEmpty()`. В методе `toString()` выполняется полный циклический обход очереди из `size()` шагов с возвратом каждого элемента в конец, благодаря чему порядок элементов в стеке полностью сохраняется.

В тестовом классе `StackOnQueueTest` (листинг 3) создаётся экземпляр `StackOnQueue`, последовательно добавляются значения `10` и `25`, выводится элемент на вершине без удаления (`top()`), затем извлекается и выводится вершина (`pop()`), проверяется пустота (`empty()`) и выводится строковое представление оставшихся элементов.

::: {custom-style="ListingCaption"}
Листинг 2 — Исходный код файла queue/StackOnQueue.java
:::

```java
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
```

::: {custom-style="ListingCaption"}
Листинг 3 — Исходный код файла queue/StackOnQueueTest.java
:::

```java
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
```

Консольный вывод программы `StackOnQueueTest` приведён в листинге 4, место для скриншота — на рисунке 1.

::: {custom-style="ListingCaption"}
Листинг 4 — Вывод программы queue.StackOnQueueTest
:::

```text
Добавлены элементы: 10, затем 25.
Текущее состояние: StackOnQueue (вершина -> дно): [25, 10]
Вершина стека без удаления (top): 25
Вершина стека с удалением (pop):  25
Стек пуст (empty)? false
Все элементы стека: StackOnQueue (вершина -> дно): [10]
```

::: {custom-style="Captioned Figure"}
![](<screenshots/20261124_16h33m28s-th.png>){width=15.5cm}
:::

::: {custom-style="Image Caption"}
Рисунок 1 — Результат работы тестового класса queue.StackOnQueueTest
:::

## 3.2 Порождающий паттерн Builder (вариант 4) — Заказ в ресторане

В пакете `builder` создан класс продукта `Order` (листинг 5) с четырьмя приватными строковыми полями: `mainDish` (основное блюдо), `sideDish` (гарнир), `drink` (напиток) и `dessert` (десерт), методами доступа и переопределённым методом `toString()`, подставляющим обозначение «не выбрано» для незаполненных позиций.

Класс `OrderBuilder` (листинг 6) инкапсулирует создаваемый экземпляр `Order` и предоставляет методы пошаговой настройки `setMainDish()`, `setSideDish()`, `setDrink()` и `setDessert()`, возвращающие `this` для цепочки вызовов. Метод `build()` возвращает готовый объект заказа и пересоздаёт внутреннюю заготовку, что позволяет многократно использовать один экземпляр строителя для независимых заказов.

В демонстрационном классе `RestaurantDemo` (листинг 7) собираются три заказа с разными наборами позиций: полный обед, бизнес-ланч без десерта и лёгкий заказ из напитка и десерта.

::: {custom-style="ListingCaption"}
Листинг 5 — Исходный код файла builder/Order.java
:::

```java
package builder;

/**
 * Порождающие паттерны. Вариант №4 (Builder).
 * Класс заказа в ресторане с четырьмя параметрами: основное блюдо, гарнир, напиток, десерт.
 */
public class Order {
    private String mainDish;
    private String sideDish;
    private String drink;
    private String dessert;

    public String getMainDish() {
        return mainDish;
    }

    public void setMainDish(String mainDish) {
        this.mainDish = mainDish;
    }

    public String getSideDish() {
        return sideDish;
    }

    public void setSideDish(String sideDish) {
        this.sideDish = sideDish;
    }

    public String getDrink() {
        return drink;
    }

    public void setDrink(String drink) {
        this.drink = drink;
    }

    public String getDessert() {
        return dessert;
    }

    public void setDessert(String dessert) {
        this.dessert = dessert;
    }

    private String formatItem(String value) {
        return (value == null || value.isEmpty()) ? "не выбрано" : value;
    }

    @Override
    public String toString() {
        return "Заказ {"
                + "основное блюдо='" + formatItem(mainDish) + "', "
                + "гарнир='" + formatItem(sideDish) + "', "
                + "напиток='" + formatItem(drink) + "', "
                + "десерт='" + formatItem(dessert) + "'}";
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 6 — Исходный код файла builder/OrderBuilder.java
:::

```java
package builder;

/**
 * Порождающие паттерны. Вариант №4 (Builder).
 * Строитель для пошагового создания объекта Order.
 */
public class OrderBuilder {
    private Order order;

    public OrderBuilder() {
        this.order = new Order();
    }

    public OrderBuilder setMainDish(String mainDish) {
        order.setMainDish(mainDish);
        return this;
    }

    public OrderBuilder setSideDish(String sideDish) {
        order.setSideDish(sideDish);
        return this;
    }

    public OrderBuilder setDrink(String drink) {
        order.setDrink(drink);
        return this;
    }

    public OrderBuilder setDessert(String dessert) {
        order.setDessert(dessert);
        return this;
    }

    public Order build() {
        Order readyOrder = order;
        order = new Order();
        return readyOrder;
    }
}
```

::: {custom-style="ListingCaption"}
Листинг 7 — Исходный код файла builder/RestaurantDemo.java
:::

```java
package builder;

/**
 * Порождающие паттерны. Вариант №4 (Builder).
 * Демонстрация сборки заказов в ресторане с разными конфигурациями.
 */
public class RestaurantDemo {
    public static void main(String[] args) {
        OrderBuilder builder = new OrderBuilder();

        // 1. Полный заказ из четырёх позиций
        Order fullOrder = builder
                .setMainDish("Стейк из говядины")
                .setSideDish("Запечённый картофель")
                .setDrink("Клюквенный морс")
                .setDessert("Чизкейк")
                .build();

        // 2. Бизнес-ланч без десерта
        Order lunchOrder = builder
                .setMainDish("Куриное филе на гриле")
                .setSideDish("Рис с овощами")
                .setDrink("Зелёный чай")
                .build();

        // 3. Лёгкий заказ: только напиток и десерт
        Order coffeeBreakOrder = builder
                .setDrink("Капучино")
                .setDessert("Яблочный штрудель")
                .build();

        System.out.println("Заказ №1 (полный):      " + fullOrder);
        System.out.println("Заказ №2 (бизнес-ланч): " + lunchOrder);
        System.out.println("Заказ №3 (кофе-брейк):  " + coffeeBreakOrder);
    }
}
```

Результат выполнения программы `RestaurantDemo` приведён в листинге 8, место для скриншота — на рисунке 2.

::: {custom-style="ListingCaption"}
Листинг 8 — Вывод программы builder.RestaurantDemo
:::

```text
Заказ №1 (полный):      Заказ {основное блюдо='Стейк из говядины', гарнир='Запечённый картофель', напиток='Клюквенный морс', десерт='Чизкейк'}
Заказ №2 (бизнес-ланч): Заказ {основное блюдо='Куриное филе на гриле', гарнир='Рис с овощами', напиток='Зелёный чай', десерт='не выбрано'}
Заказ №3 (кофе-брейк):  Заказ {основное блюдо='не выбрано', гарнир='не выбрано', напиток='Капучино', десерт='Яблочный штрудель'}
```

::: {custom-style="Captioned Figure"}
![](<screenshots/20261124_16h35m10s-th.png>){width=15.5cm}
:::

::: {custom-style="Image Caption"}
Рисунок 2 — Результат работы программы builder.RestaurantDemo (паттерн Builder, вариант 4)
:::

::: {custom-style="StructuralHeading"}
ЗАКЛЮЧЕНИЕ
:::

В ходе выполнения практической работы № 6 изучены особенности интерфейса очереди `java.util.Queue` и порождающих паттернов проектирования в языке Java.

С помощью отношения композиции и двух очередей FIFO реализован класс стека `StackOnQueue`, поддерживающий операции `push()`, `pop()`, `top()`, `empty()` и `toString()` исключительно на базе стандартных методов очереди; корректность его работы подтверждена тестовым классом `StackOnQueueTest`. В рамках варианта № 4 разработан модуль формирования ресторанных заказов на основе паттерна `Builder`, позволяющий пошагово и наглядно конструировать независимые объекты `Order` с произвольным набором блюд.
