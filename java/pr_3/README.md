# Практическая работа №3 — Java

**Тема:** Объекты и классы: абстрактные классы и интерфейсы, пакеты.

В данной папке находится полное решение практической работы №3 (Часть 1, Задача #1) и готовый отчёт [`report.md`](./report.md).

---

## Структура проекта и организация файлов

По условию работа организована по пакетам:
- **`vehicles`** — абстрактный класс `Vehicle`, классы `Car` и `ElectricCar`, интерфейс `ElectricVehicle`.
- **`app`** — тестовый класс `TestCar` с точкой входа `public static void main(String[] args)`.

Оформление повторяет предыдущие работы (`java/pr_1`, `java/pr_2`): исходные файлы сгруппированы по пакетам, а в корне практики находятся `README.md` и `report.md`.

```text
java/pr_3/
├── README.md                     # Описание структуры проекта, файлов и запуска
├── report.md                     # Полный отчёт о проделанной работе с кодом и меткой для скриншота
├── vehicles/                     # Пакет vehicles (модели транспортных средств)
│   ├── Vehicle.java              # Абстрактный базовый класс транспортного средства
│   ├── Car.java                  # Класс обычного автомобиля (наследует Vehicle)
│   ├── ElectricVehicle.java      # Интерфейс электромобиля
│   └── ElectricCar.java          # Класс электромобиля (наследует Car, реализует ElectricVehicle)
└── app/                          # Пакет app (тестовый класс)
    └── TestCar.java              # Запуск Части 1, Задачи #1
```

---

## Соответствие требований задания и файлов

| Требование задания | Где реализовано |
|---|---|
| Пакет `vehicles` для классов `Vehicle`, `Car`, `ElectricCar` и интерфейса `ElectricVehicle` | `vehicles/Vehicle.java`, `vehicles/Car.java`, `vehicles/ElectricCar.java`, `vehicles/ElectricVehicle.java` |
| Пакет `app` для тестового класса `TestCar` | `app/TestCar.java` |
| Абстрактный класс `Vehicle` с полями `model`, `license`, `color`, `year`, `ownerName`, `insuranceNumber`, `engineType` | `vehicles/Vehicle.java` |
| Геттеры и сеттеры для каждого поля, метод `toString()` | `vehicles/Vehicle.java` (в `ElectricCar.java` метод `toString()` переопределён и дополнен ёмкостью батареи) |
| Абстрактный метод `vehicleType()` | `vehicles/Vehicle.java`; реализован в `Car.java` (`"Car"`) и `ElectricCar.java` (`"Electric Car"`) |
| Класс `Car` наследует `Vehicle`, в конструкторе задаётся `engineType = "Combustion"` | `vehicles/Car.java` |
| Интерфейс `ElectricVehicle` с методами `getBatteryCapacity()` и `setBatteryCapacity()` | `vehicles/ElectricVehicle.java` |
| Класс `ElectricCar` наследует `Car`, реализует `ElectricVehicle`, содержит поле `batteryCapacity`, в конструкторе задаётся `engineType = "Electric"` | `vehicles/ElectricCar.java` |
| Созданы экземпляры `Car` и `ElectricCar` | `app/TestCar.java`, шаг 1 |
| Изменение года выпуска и имени владельца | `app/TestCar.java`, шаг 2 |
| Изменение страхового номера | `app/TestCar.java`, шаг 3 |
| Получение информации о ёмкости батареи электромобиля | `app/TestCar.java`, шаг 4 (через ссылку типа `ElectricVehicle`) |
| Вывод информации о транспортных средствах методом `toString()` | `app/TestCar.java`, шаг 5 |

---

## Подробное описание классов

- **`vehicles/Vehicle.java`**
  Абстрактный базовый класс всех транспортных средств:
  - приватные поля `model`, `license`, `color`, `year`, `ownerName`, `insuranceNumber`;
  - защищённое поле `protected String engineType` — доступно классам-наследникам (`Car`, `ElectricCar`);
  - абстрактный метод `public abstract String vehicleType()`;
  - публичные геттеры и сеттеры для всех семи полей;
  - переопределённый `toString()`, возвращающий строку с описанием транспортного средства; общая часть описания вынесена в `protected String commonDescription()`, чтобы подклассы могли её дополнять.

- **`vehicles/ElectricVehicle.java`**
  Интерфейс, описывающий специфические методы электромобиля: `int getBatteryCapacity()` и `void setBatteryCapacity(int batteryCapacity)`. Методы интерфейса неявно являются `public abstract`.

- **`vehicles/Car.java`**
  Наследует `Vehicle` (`extends Vehicle`):
  - в обоих конструкторах тип двигателя задаётся как `"Combustion"`;
  - реализует `vehicleType()`, возвращая `"Car"`.

- **`vehicles/ElectricCar.java`**
  Наследует `Car` и реализует `ElectricVehicle` (`extends Car implements ElectricVehicle`):
  - приватное поле `batteryCapacity` — ёмкость батареи в кВт·ч;
  - реализация методов интерфейса `getBatteryCapacity()` и `setBatteryCapacity()`;
  - в конструкторах после вызова `super(...)` тип двигателя переопределяется как `"Electric"`;
  - переопределяет `vehicleType()` (`"Electric Car"`) и `toString()`, добавляя к общему описанию `batteryCapacity`.

- **`app/TestCar.java`**
  Тестовый класс:
  - создаёт объекты `Car` и `ElectricCar`;
  - изменяет год выпуска и имя владельца, а затем страховой номер через сеттеры;
  - получает ёмкость батареи через ссылку типа `ElectricVehicle` и изменяет её;
  - выводит описания транспортных средств методом `toString()` (в том числе полиморфно — через массив ссылок `Vehicle[]`).

---

## Компиляция и запуск

Перейдите в каталог `java/pr_3` и скомпилируйте все классы:

```sh
cd java/pr_3
javac -encoding UTF-8 -d /tmp/pr3-classes vehicles/*.java app/*.java
```

Запуск тестового класса:

```sh
java -cp /tmp/pr3-classes app.TestCar
```

> В Windows для корректного отображения кириллицы в консоли может потребоваться команда `chcp 65001` перед запуском.
