# Практическая работа №5 — Java

**Тема:** Дженерики и обобщённое программирование. Структуры данных. Стек.

Простая реализация всех заданий практической работы №5 (Часть 1: Задания №1–2 по дженерикам и `GenericStack`; Часть 2: Задание №1 по `MyStack`, наследованию/композиции и глубокому клонированию), UML-диаграммы в `images/` и готовый отчёт по ГОСТ [`report.md`](./report.md).

---

## Что делал?

1. **Часть 1, Задание №1** — реализовал 4 обобщённых метода в `GenericUtils`: удаление дубликатов из `ArrayList<E>` через `contains()`, линейный поиск в массиве, поиск максимума в одномерном массиве (проверил на массиве объектов `Circle` с полем `radius`) и поиск максимума в двумерном массиве через `Comparable.compareTo()`.
2. **Часть 1, Задание №2** — переписал `GenericStack<E>` с `ArrayList` на обычный массив с автоматическим удвоением размера при заполнении; отдельно сделал `GenericStackInheritance<E>`, наследующийся от `ArrayList<E>`, подготовил UML-диаграмму и тестовую программу, выводящую 5 введённых строк в обратном порядке.
3. **Часть 2, Задание №1** — реализовал класс `MyStack` через композицию с `ArrayList<Object>` и альтернативный `MyStackInheritance` через наследование от `ArrayList<Object>`, подготовил UML-диаграмму, написал клиент для разворота 5 строк и добавил в `MyStack` глубокое копирование (`Cloneable` + `clone()`).

---

## Где что лежит?

```text
java/pr_5/
├── README.md                              # Краткое описание работы и запуска
├── report.md                              # Отчёт по ГОСТ 7.32-2017 (под Pandoc)
├── images/                                # UML-диаграммы для отчёта (подтягиваются Pandoc)
│   ├── uml_generic_stack.png              # UML для GenericStack<E> и GenericStackInheritance<E>
│   └── uml_my_stack.png                   # UML для MyStack и MyStackInheritance
├── generics/                              # Пакет Части 1 (дженерики и обобщённый стек)
│   ├── Circle.java                        # Класс круга с полем radius (Comparable<Circle>)
│   ├── GenericUtils.java                  # 4 обобщённых метода Задания №1
│   ├── GenericStack.java                  # Стек GenericStack<E> на массиве с удвоением ёмкости
│   └── GenericStackInheritance.java       # Стек-наследник ArrayList<E>
├── mystack/                               # Пакет Части 2 (стек объектов и клонирование)
│   ├── MyStack.java                       # Стек на композиции + глубокое копирование clone()
│   └── MyStackInheritance.java            # Стек-наследник ArrayList<Object>
└── app/                                   # Пакет с точками входа main
    ├── TestGenericsTask1.java             # Запуск Части 1, Задания №1
    ├── TestGenericStackTask2.java         # Запуск Части 1, Задания №2
    └── TestMyStack.java                   # Запуск Части 2, Задания №1
```

---

## Как работают ключевые методы?

- **`GenericUtils.removeDuplicates(ArrayList<E> list)`** — создаёт новый список `result`, проходит по исходному и добавляет элемент только если `!result.contains(element)`. Исходный список не портится.
- **`GenericUtils.linearSearch(E[] array, E key)`** — последовательно сравнивает элементы массива с `key` через `Objects.equals()`. Нашёл — сразу вернул индекс `i`; дошёл до конца — вернул `-1`.
- **`GenericUtils.max(E[] array)` и `max(E[][] matrix)`** — ограничение типа `<E extends Comparable<E>>` гарантирует, что у элементов есть метод `compareTo()`. Метод проходит по всем элементам и обновляет `maxElement`, если `current.compareTo(maxElement) > 0`.
- **`GenericStack.push(E o)` и `pop()`** — стек хранит элементы в массиве `elements` и счётчик `size`. Перед вставкой проверяется `size == elements.length`: если места нет, создаётся новый массив длиной `elements.length * 2`, старые данные копируются через `System.arraycopy`, затем элемент кладётся в `elements[size++]`. При `pop()` берётся `elements[--size]`, а освободившаяся ячейка зануляется (`null`), чтобы не держать лишнюю ссылку в памяти.
- **`GenericStackInheritance<E>` и `MyStackInheritance`** — наследуются от `ArrayList`, поэтому `push()` вызывает `super.add(o)`, `peek()` — `super.get(getSize() - 1)`, а `pop()` — `super.remove(getSize() - 1)`. За счёт принципа LIFO («последним пришёл — первым вышел») 5 введённых строк при последовательном `pop()` выводятся в обратном порядке.
- **`MyStack.clone()`** — сначала делает поверхностную копию самого стека через `(MyStack) super.clone()`, а затем вызывает `copyList()`, который создаёт **новый** `ArrayList<Object>` и переносит элементы (для изменяемых объектов `Date` и вложенных `MyStack` также вызывается их `clone()`). В результате изменения в клоне никак не затрагивают оригинал.

---

## Как это связано?

```text
Часть 1 (пакет generics):
  TestGenericsTask1     ──> GenericUtils + Circle (implements Comparable<Circle>)
  TestGenericStackTask2 ──> GenericStack<E> (внутри массив E[])
                        └── GenericStackInheritance<E> ──extends──> ArrayList<E>

Часть 2 (пакет mystack):
  TestMyStack           ──> MyStack (implements Cloneable, внутри поле ArrayList<Object>)
                        └── MyStackInheritance ──extends──> ArrayList<Object>
```

Обе части показывают разницу между двумя подходами к проектированию стека: **композицией** (`has-a`, список/массив спрятан внутри поля) и **наследованием** (`is-a`, стек расширяет `ArrayList`).

---

## Компиляция, запуск и сборка отчёта

Компиляция всех заданий из папки `java/pr_5`:

```sh
cd java/pr_5
javac -encoding UTF-8 -d /tmp/pr5-classes generics/*.java mystack/*.java app/*.java
```

Запуск каждого задания:

```sh
java -cp /tmp/pr5-classes app.TestGenericsTask1
java -cp /tmp/pr5-classes app.TestGenericStackTask2
java -cp /tmp/pr5-classes app.TestMyStack
```

Конвертация отчёта в Word по ГОСТ через Pandoc (запускать из `java/pr_5`, чтобы подтянулись картинки из `images/`):

```sh
cd java/pr_5
pandoc report.md --toc --reference-doc=../gost-reference.docx -o report.docx
```

---

## Контрольные вопросы (для защиты)

1. **Зачем нужны дженерики (generics) в Java?**  
   Они позволяют параметризовать классы и методы типом (`<E>`, `<T>`) и ловить ошибки несовместимости типов ещё на этапе компиляции, а не во время работы программы (`ClassCastException`), и избавляют от ручного приведения типов.

2. **Почему в `GenericStack<E>` массив создаётся как `(E[]) new Object[...]`?**  
   Из-за стирания типов (type erasure) во время выполнения JVM не знает конкретный тип `E`, поэтому конструкция `new E[n]` запрещена в Java. Стандартный приём — создать `Object[]` и привести его к `E[]` внутри класса.

3. **Что лучше для реализации стека — композиция или наследование от `ArrayList`?**  
   Композиция лучше: стек открывает наружу только свои операции (`push`, `pop`, `peek`, `isEmpty`, `getSize`). При наследовании от `ArrayList` клиент получает доступ к методам `add(0, x)` или `remove(0)`, которыми можно нарушить принцип LIFO.

4. **Чем поверхностное копирование отличается от глубокого?**  
   При поверхностном (`super.clone()`) копируются значения примитивов и **ссылки** на объекты — оригинал и копия делят один и тот же внутренний список `list`. При глубоком копировании для ссылочных полей создаются отдельные независимые копии объектов.

5. **Зачем классу реализовывать интерфейс `Cloneable`, если в нём нет методов?**  
   `Cloneable` — маркерный интерфейс. Нативный метод `Object.clone()` проверяет его наличие: если класс не реализует `Cloneable`, при вызове `super.clone()` будет выброшено исключение `CloneNotSupportedException`.
