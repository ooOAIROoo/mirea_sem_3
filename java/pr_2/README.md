# Практическая работа №2 — Java

**Тема:** Объекты и классы: модификаторы доступа, инкапсуляция полей. Принципы ООП в Java и наследование классов, передача метода объектов.

В данной папке находится полное решение практической работы №2 (Часть 1 и Часть 2) и готовый отчёт [`report.md`](./report.md).

---

## Структура проекта и организация файлов

По условию работы проект разделён на два пакета:
- **`vehicles`** — классы предметной области (транспортные средства).
- **`app`** — запускаемые классы с точкой входа `public static void main(String[] args)`.

По аналогии с первой практической работой (`java/pr_1`, где промежуточные версии класса `Car` для разных заданий выделялись в `Car_t3.java` и `Car_t4.java`, а запуск каждого задания — в `task_1.java`…`task_4.java`), в `java/pr_2` сохранены **все этапы работы**, чтобы каждое задание можно было скомпилировать, запустить и заскриншотить отдельно без конфликта имён классов:

```text
java/pr_2/
├── README.md                # Описание структуры проекта, файлов и запуска заданий
├── report.md                # Полный отчёт о проделанной работе с кодом и метками для скриншотов
├── vehicles/                # Пакет vehicles (модели транспортных средств)
│   ├── Car_t1.java          # Класс Car для Части 1 (Задачи #1 и #2)
│   ├── ElectricCar_t1.java  # Класс ElectricCar для Части 1 (Задача #2)
│   ├── Vehicle.java         # Абстрактный базовый класс Vehicle для Части 2 (Задача #1)
│   ├── Car.java             # Итоговый класс Car (наследует Vehicle) для Части 2 (Задача #1)
│   └── ElectricCar.java     # Итоговый класс ElectricCar (наследует Car) для Части 2 (Задача #1)
└── app/                     # Пакет app (классы запуска для каждого задания)
    ├── task_1.java          # Запуск Части 1, Задачи #1
    ├── task_2.java          # Запуск Части 1, Задачи #2
    └── TestCar.java         # Запуск Части 2, Задачи #1 (итоговый тестовый класс)
```

---

## Соответствие файлов и заданий

| Задание | Запускаемый файл (`main`) | Классы пакета `vehicles` | Что демонстрируется |
|---|---|---|---|
| **Часть 1. Задача #1** | `app/task_1.java` | `vehicles/Car_t1.java` | Создание пакетов `vehicles` и `app`. Инкапсуляция приватных полей `ownerName` и `insuranceNumber` (`private`), защищённое поле `engineType` (`protected`), чтение и изменение полей через геттеры и сеттеры. |
| **Часть 1. Задача #2** | `app/task_2.java` | `vehicles/Car_t1.java`, `vehicles/ElectricCar_t1.java` | Наследование `ElectricCar_t1 extends Car_t1`. Добавление поля `batteryCapacity`. Установка `this.engineType = "Electric"` через `protected`-поле родителя. Проверка доступа к полям с разными модификаторами. |
| **Часть 2. Задача #1** | `app/TestCar.java` | `vehicles/Vehicle.java`, `vehicles/Car.java`, `vehicles/ElectricCar.java` | Абстрактный класс `Vehicle` с общими полями (`model`, `license`, `color`, `year`, `ownerName`, `insuranceNumber`, `protected engineType`) и абстрактным методом `vehicleType()`. Иерархия `Vehicle -> Car -> ElectricCar`. Полиморфизм через ссылки на `Vehicle`, изменение свойств через сеттеры и вывод через `toString()`. |

---

## Подробное описание классов

### 1. Часть 1 (Задачи #1 и #2)

- **`vehicles/Car_t1.java`**  
  Реализует начальную версию класса автомобиля из Части 1:
  - `private String ownerName` — имя владельца (доступно только внутри `Car_t1`, снаружи — через `getOwnerName()` / `setOwnerName()`).
  - `private String insuranceNumber` — страховой номер (доступен через `getInsuranceNumber()` / `setInsuranceNumber()`).
  - `protected String engineType` — тип двигателя (доступен напрямую в подклассах и внутри пакета `vehicles`, а из пакета `app` — через `getEngineType()` / `setEngineType()`).
  - `printInfo()` — вспомогательный метод для наглядного вывода значений полей в консоль.

- **`vehicles/ElectricCar_t1.java`**  
  Наследует `Car_t1` (`extends Car_t1`):
  - `private int batteryCapacity` — ёмкость батареи (кВт·ч) с методами `getBatteryCapacity()` и `setBatteryCapacity()`.
  - В конструкторе напрямую обращается к унаследованному `protected`-полю `this.engineType = "Electric"`.
  - Переопределяет `printInfo()` для вывода всех характеристик электромобиля.

- **`app/task_1.java`**  
  Точка входа для **Части 1. Задачи #1**: создаёт объекты `Car_t1`, изменяет поля `ownerName`, `insuranceNumber`, `engineType` через сеттеры и выводит их значения через геттеры.

- **`app/task_2.java`**  
  Точка входа для **Части 1. Задачи #2**: создаёт объекты `Car_t1` и `ElectricCar_t1`, демонстрирует автоматическую установку `engineType = "Electric"`, работу с `batteryCapacity` и унаследованными методами доступа.

---

### 2. Часть 2 (Задача #1)

- **`vehicles/Vehicle.java`**  
  Абстрактный базовый класс всех транспортных средств:
  - Инкапсулированные поля (`private`): `model`, `license`, `color`, `year`, `ownerName`, `insuranceNumber`.
  - Защищённое поле (`protected`): `engineType`.
  - Абстрактный метод `public abstract String vehicleType()`.
  - Геттеры и сеттеры для всех семи полей.
  - Переопределённый метод `toString()`, формирующий строковое представление объекта с вызовом `vehicleType()` и хука `additionalDetails()`.

- **`vehicles/Car.java`**  
  Класс обычного автомобиля, наследующий `Vehicle` (`extends Vehicle`):
  - В конструкторах использует конструктор (`super(...)`), сеттеры (`setModel()`, `setLicense()` и др.) и защищённое поле `engineType` родительского класса `Vehicle`.
  - Реализует метод `vehicleType()`, возвращая `"Car"`.

- **`vehicles/ElectricCar.java`**  
  Класс электромобиля, наследующий `Car` (`extends Car`):
  - Содержит приватное поле `private int batteryCapacity` с геттером и сеттером.
  - В конструкторе задаёт `this.engineType = "Electric"` через `protected`-поле.
  - Переопределяет `vehicleType()`, возвращая `"Electric Car"`, и дополняет строковое представление `toString()` ёмкостью аккумулятора.

- **`app/TestCar.java`**  
  Тестовый класс для **Части 2. Задачи #1**:
  - Создаёт объекты `Car` и `ElectricCar`.
  - Работает с ними полиморфно через ссылки типа `Vehicle` (`Vehicle carAsVehicle`, `Vehicle electricCarAsVehicle`).
  - Модифицирует свойства объектов исключительно через сеттеры.
  - В цикле по массиву `Vehicle[]` демонстрирует полиморфный вызов `vehicleType()` и `toString()`.

---

## Компиляция и запуск

Перейдите в каталог `java/pr_2` и скомпилируйте все классы:

```sh
cd java/pr_2
javac -d /tmp/pr2-classes vehicles/*.java app/*.java
```

После компиляции можно по очереди запустить каждое задание для проверки и создания скриншотов в отчёт:

### Запуск Части 1, Задачи #1
```sh
java -cp /tmp/pr2-classes app.task_1
```

### Запуск Части 1, Задачи #2
```sh
java -cp /tmp/pr2-classes app.task_2
```

### Запуск Части 2, Задачи #1 (основной тестовый класс `TestCar`)
```sh
java -cp /tmp/pr2-classes app.TestCar
```
