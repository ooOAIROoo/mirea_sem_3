# Отчёт по практической работе №2

**Дисциплина:** Программирование на языке Java  
**Тема:** Объекты и классы: модификаторы доступа, инкапсуляция полей. Принципы ООП в Java и наследование классов, передача метода объектов.

---

## Содержание

1. [Цель работы и теоретические сведения](#1-цель-работы-и-теоретические-сведения)
2. [Структура проекта и распределение кода по заданиям](#2-структура-проекта-и-распределение-кода-по-заданиям)
3. [Часть 1. Задача #1 — Пакеты, модификаторы доступа и инкапсуляция в классе Car](#3-часть-1-задача-1--пакеты-модификаторы-доступа-и-инкапсуляция-в-классе-car)
4. [Часть 1. Задача #2 — Наследование класса ElectricCar и доступ к protected-полю](#4-часть-1-задача-2--наследование-класса-electriccar-и-доступ-к-protected-полю)
5. [Часть 2. Задача #1 — Абстрактный класс Vehicle, наследование, инкапсуляция и полиморфизм](#5-часть-2-задача-1--абстрактный-класс-vehicle-наследование-инкапсуляция-и-полиморфизм)
6. [Заключение](#6-заключение)

---

## 1. Цель работы и теоретические сведения

### 1.1 Цель работы

Изучить организацию классов с помощью пакетов (`package`), применение модификаторов доступа (`private`, `default`, `protected`, `public`), реализацию принципа инкапсуляции через методы доступа (геттеры и сеттеры), а также механизмы наследования (`extends`), абстрактных классов (`abstract`) и полиморфизма в языке Java.

### 1.2 Краткие теоретические сведения

Доступ к классам, методам и полям данных в Java регулируется модификаторами доступа:

| Модификатор | Внутри класса | Внутри того же пакета | В классе-наследнике (другой пакет) | Из любого класса |
|---|:---:|:---:|:---:|:---:|
| `private` | Да | Нет | Нет | Нет |
| `default` (без модификатора) | Да | Да | Нет | Нет |
| `protected` | Да | Да | Да | Нет |
| `public` | Да | Да | Да | Да |

- **Пакеты (`package`)** используются для группировки логически связанных классов. В соответствии со стандартами разработки один каталог файловой системы соответствует одному пакету.
- **Инкапсуляция** — сокрытие внутренней структуры данных объекта с помощью модификатора `private` и предоставление контролируемого доступа к полям через публичные методы:
  - **геттеры (accessors)** — методы чтения вида `public Тип getИмяПоля()`;
  - **сеттеры (mutators)** — методы модификации вида `public void setИмяПоля(Тип значение)`.
- **Наследование (`extends`)** позволяет создавать новый класс на основе существующего, переиспользуя поля и методы родительского класса.
- **Полиморфизм** позволяет работать с объектами различных подклассов (`Car`, `ElectricCar`) через единый интерфейс родительского класса (`Vehicle`), при этом конкретная реализация переопределённых методов (`vehicleType()`, `toString()`) определяется во время выполнения (динамическое связывание).

---

## 2. Структура проекта и распределение кода по заданиям

По аналогии с первой практической работой (`pr_1`), где для разных этапов модификации класса использовались отдельные файлы (`Car_t3.java`, `Car_t4.java` и запускаемые классы `task_1`–`task_4`), во второй практической работе код разделён по задачам внутри пакетов `vehicles` и `app`:

```text
java/pr_2/
├── README.md
├── report.md
├── vehicles/
│   ├── Car_t1.java          # Часть 1 (Задачи #1 и #2): базовый класс автомобиля
│   ├── ElectricCar_t1.java  # Часть 1 (Задача #2): электромобиль, наследующий Car_t1
│   ├── Vehicle.java         # Часть 2 (Задача #1): абстрактный класс транспортного средства
│   ├── Car.java             # Часть 2 (Задача #1): класс Car, наследующий Vehicle
│   └── ElectricCar.java     # Часть 2 (Задача #1): класс ElectricCar, наследующий Car
└── app/
    ├── task_1.java          # Точка входа (main) для Части 1, Задачи #1
    ├── task_2.java          # Точка входа (main) для Части 1, Задачи #2
    └── TestCar.java         # Точка входа (main) для Части 2, Задачи #1
```

---

## 3. Часть 1. Задача #1 — Пакеты, модификаторы доступа и инкапсуляция в классе Car

### 3.1 Постановка задачи

Создайте пакет `vehicles`, который будет содержать классы `Car` и `ElectricCar`, и пакет `app`, в котором будет находиться основной класс с методом `main`. Добавьте в класс `Car` приватные поля (`private`) `ownerName` и `insuranceNumber`. Создайте методы доступа (геттеры и сеттеры) для полей `ownerName` и `insuranceNumber`. Добавьте поле `engineType` с модификатором доступа `protected` и создайте методы доступа к этому полю.

### 3.2 Описание решения

В пакете `vehicles` создан класс `Car_t1` (суффикс `_t1` использован по аналогии с `pr_1`, чтобы сохранить начальную версию класса до его переделки в Части 2).
- Поля `ownerName` и `insuranceNumber` объявлены с модификатором `private`, поэтому прямой доступ к ним из других классов невозможен.
- Поле `engineType` объявлено с модификатором `protected`, что разрешает обращаться к нему из классов-наследников (например, `ElectricCar_t1`) и внутри пакета `vehicles`, но скрывает его от прямого обращения из пакета `app`.
- Для всех трёх полей реализованы публичные геттеры (`getOwnerName()`, `getInsuranceNumber()`, `getEngineType()`) и сеттеры (`setOwnerName()`, `setInsuranceNumber()`, `setEngineType()`).

### 3.3 Код программы

**Файл `vehicles/Car_t1.java`:**

```java
package vehicles;

/**
 * Часть 1. Задача #1 и #2.
 * Класс автомобиля с инкапсулированными полями ownerName, insuranceNumber
 * и защищённым полем engineType.
 */
public class Car_t1 {
    private String ownerName;
    private String insuranceNumber;
    protected String engineType;

    public Car_t1() {
        this.ownerName = "Неизвестно";
        this.insuranceNumber = "Неизвестно";
        this.engineType = "Бензиновый";
    }

    public Car_t1(String ownerName, String insuranceNumber, String engineType) {
        this.ownerName = ownerName;
        this.insuranceNumber = insuranceNumber;
        this.engineType = engineType;
    }

    public String getOwnerName() {
        return ownerName;
    }

    public void setOwnerName(String ownerName) {
        this.ownerName = ownerName;
    }

    public String getInsuranceNumber() {
        return insuranceNumber;
    }

    public void setInsuranceNumber(String insuranceNumber) {
        this.insuranceNumber = insuranceNumber;
    }

    public String getEngineType() {
        return engineType;
    }

    public void setEngineType(String engineType) {
        this.engineType = engineType;
    }

    public void printInfo() {
        System.out.println("Владелец (private ownerName): " + ownerName);
        System.out.println("Страховой номер (private insuranceNumber): " + insuranceNumber);
        System.out.println("Тип двигателя (protected engineType): " + engineType);
        System.out.println("------------------------------------------");
    }
}
```

**Файл `app/task_1.java`:**

```java
package app;

import vehicles.Car_t1;

/**
 * Часть 1. Задача #1.
 * Демонстрация работы класса Car с приватными полями ownerName, insuranceNumber
 * и защищённым полем engineType через геттеры и сеттеры.
 */
public class task_1 {
    public static void main(String[] args) {
        System.out.println("=== Часть 1. Задача #1 ===");

        // 1. Создание объекта конструктором по умолчанию
        Car_t1 car1 = new Car_t1();
        System.out.println("Автомобиль 1 (значения по умолчанию):");
        car1.printInfo();

        // Изменение значений закрытых полей через сеттеры
        car1.setOwnerName("Иванов Иван");
        car1.setInsuranceNumber("INS-001-77");
        car1.setEngineType("Petrol");

        System.out.println("Автомобиль 1 (после изменения через сеттеры):");
        System.out.println("Владелец: " + car1.getOwnerName());
        System.out.println("Страховой номер: " + car1.getInsuranceNumber());
        System.out.println("Тип двигателя: " + car1.getEngineType());
        System.out.println("------------------------------------------");

        // 2. Создание объекта конструктором с параметрами
        Car_t1 car2 = new Car_t1("Петров Пётр", "INS-002-99", "Diesel");
        System.out.println("Автомобиль 2 (создан через конструктор с параметрами):");
        car2.printInfo();
    }
}
```

### 3.4 Результат работы программы (Часть 1. Задача #1)

Команды для компиляции и запуска из каталога `java/pr_2`:

```sh
javac -d /tmp/pr2-classes vehicles/*.java app/*.java
java -cp /tmp/pr2-classes app.task_1
```

Ожидаемый вывод в терминале:

```text
=== Часть 1. Задача #1 ===
Автомобиль 1 (значения по умолчанию):
Владелец (private ownerName): Неизвестно
Страховой номер (private insuranceNumber): Неизвестно
Тип двигателя (protected engineType): Бензиновый
------------------------------------------
Автомобиль 1 (после изменения через сеттеры):
Владелец: Иванов Иван
Страховой номер: INS-001-77
Тип двигателя: Petrol
------------------------------------------
Автомобиль 2 (создан через конструктор с параметрами):
Владелец (private ownerName): Петров Пётр
Страховой номер (private insuranceNumber): INS-002-99
Тип двигателя (protected engineType): Diesel
------------------------------------------
```

> 📸 **[СКРИНШОТ №1 — Часть 1. Задача #1]**  
> *Вставьте сюда скриншот запуска `app.task_1` в терминале / IDE (вывод создания объектов `Car_t1` и изменения полей `ownerName`, `insuranceNumber`, `engineType` через геттеры и сеттеры).*

---

## 4. Часть 1. Задача #2 — Наследование класса ElectricCar и доступ к protected-полю

### 4.1 Постановка задачи

Создайте новый класс `ElectricCar`, который наследует класс `Car`, и добавьте в него поле `batteryCapacity`. В классе `ElectricCar` используйте поле `engineType`, чтобы задать тип двигателя как `"Electric"`. Проверьте работу инкапсуляции и наследования, создав объекты классов `Car` и `ElectricCar` и продемонстрируйте доступ к полям с разными модификаторами.

### 4.2 Описание решения

В пакете `vehicles` создан класс `ElectricCar_t1`, наследующий `Car_t1` (`extends Car_t1`):
- Добавлено приватное поле `private int batteryCapacity` и публичные методы доступа `getBatteryCapacity()` и `setBatteryCapacity(int batteryCapacity)`.
- В конструкторах класса `ElectricCar_t1` выполняется прямое обращение к унаследованному защищённому полю `this.engineType = "Electric"`, так как модификатор `protected` разрешает доступ из дочернего класса.
- При этом к приватным полям родителя (`ownerName`, `insuranceNumber`) класс `ElectricCar_t1` и внешний класс `app.task_2` обращаются исключительно через публичные геттеры и сеттеры (`getOwnerName()`, `setOwnerName()`, `getInsuranceNumber()`, `setInsuranceNumber()`).

### 4.3 Код программы

**Файл `vehicles/ElectricCar_t1.java`:**

```java
package vehicles;

/**
 * Часть 1. Задача #2.
 * Класс электромобиля, наследующий Car_t1 и использующий protected-поле engineType.
 */
public class ElectricCar_t1 extends Car_t1 {
    private int batteryCapacity;

    public ElectricCar_t1() {
        super();
        this.engineType = "Electric";
        this.batteryCapacity = 0;
    }

    public ElectricCar_t1(String ownerName, String insuranceNumber, int batteryCapacity) {
        super(ownerName, insuranceNumber, "Electric");
        // Прямой доступ к protected-полю родительского класса Car_t1
        this.engineType = "Electric";
        this.batteryCapacity = batteryCapacity;
    }

    public int getBatteryCapacity() {
        return batteryCapacity;
    }

    public void setBatteryCapacity(int batteryCapacity) {
        this.batteryCapacity = batteryCapacity;
    }

    @Override
    public void printInfo() {
        System.out.println("Владелец (через getOwnerName): " + getOwnerName());
        System.out.println("Страховой номер (через getInsuranceNumber): " + getInsuranceNumber());
        System.out.println("Тип двигателя (protected engineType): " + this.engineType);
        System.out.println("Ёмкость батареи (private batteryCapacity): " + batteryCapacity + " кВт·ч");
        System.out.println("------------------------------------------");
    }
}
```

**Файл `app/task_2.java`:**

```java
package app;

import vehicles.Car_t1;
import vehicles.ElectricCar_t1;

/**
 * Часть 1. Задача #2.
 * Проверка инкапсуляции и наследования на примере классов Car_t1 и ElectricCar_t1.
 */
public class task_2 {
    public static void main(String[] args) {
        System.out.println("=== Часть 1. Задача #2 ===");

        // Создание объекта обычного автомобиля
        Car_t1 car = new Car_t1("Алексеев Алексей", "CAR-100-77", "Gasoline");
        System.out.println("Обычный автомобиль (Car):");
        car.printInfo();

        // Создание объекта электромобиля (engineType автоматически "Electric")
        ElectricCar_t1 electricCar = new ElectricCar_t1("Смирнова Мария", "EV-200-77", 75);
        System.out.println("Электромобиль (ElectricCar):");
        electricCar.printInfo();

        // Изменение полей через публичные методы доступа (сеттеры)
        // Прямой доступ вида electricCar.ownerName или electricCar.engineType из пакета app запрещён!
        electricCar.setOwnerName("Смирнова Мария Сергеевна");
        electricCar.setInsuranceNumber("EV-200-99-UPD");
        electricCar.setBatteryCapacity(82);

        System.out.println("Электромобиль после изменения данных через сеттеры:");
        System.out.println("Владелец: " + electricCar.getOwnerName());
        System.out.println("Страховой номер: " + electricCar.getInsuranceNumber());
        System.out.println("Тип двигателя: " + electricCar.getEngineType());
        System.out.println("Ёмкость аккумулятора: " + electricCar.getBatteryCapacity() + " кВт·ч");
        System.out.println("------------------------------------------");
    }
}
```

### 4.4 Результат работы программы (Часть 1. Задача #2)

Команды для компиляции и запуска из каталога `java/pr_2`:

```sh
javac -d /tmp/pr2-classes vehicles/*.java app/*.java
java -cp /tmp/pr2-classes app.task_2
```

Ожидаемый вывод в терминале:

```text
=== Часть 1. Задача #2 ===
Обычный автомобиль (Car):
Владелец (private ownerName): Алексеев Алексей
Страховой номер (private insuranceNumber): CAR-100-77
Тип двигателя (protected engineType): Gasoline
------------------------------------------
Электромобиль (ElectricCar):
Владелец (через getOwnerName): Смирнова Мария
Страховой номер (через getInsuranceNumber): EV-200-77
Тип двигателя (protected engineType): Electric
Ёмкость батареи (private batteryCapacity): 75 кВт·ч
------------------------------------------
Электромобиль после изменения данных через сеттеры:
Владелец: Смирнова Мария Сергеевна
Страховой номер: EV-200-99-UPD
Тип двигателя: Electric
Ёмкость аккумулятора: 82 кВт·ч
------------------------------------------
```

> 📸 **[СКРИНШОТ №2 — Часть 1. Задача #2]**  
> *Вставьте сюда скриншот запуска `app.task_2` в терминале / IDE (демонстрация работы `Car_t1` и `ElectricCar_t1`, наследования поля `engineType = "Electric"` и доступа к полям через геттеры/сеттеры).*

---

## 5. Часть 2. Задача #1 — Абстрактный класс Vehicle, наследование, инкапсуляция и полиморфизм

### 5.1 Постановка задачи

Программа должна быть организована по пакетам:
- Пакет `vehicles` для классов `Vehicle`, `Car`, `ElectricCar`.
- Пакет `app` для тестового класса `TestCar`.

Требуется внести следующие изменения:
1. Добавить абстрактный класс `Vehicle`, представляющий общие свойства всех транспортных средств: `model` (модель), `license` (номерной знак), `color` (цвет), `year` (год выпуска), `ownerName` (имя владельца), `insuranceNumber` (страховой номер), `engineType` (тип двигателя, `protected` для наследования). Определить абстрактный метод `vehicleType()`, возвращающий тип транспортного средства, и добавить геттеры и сеттеры для всех полей.
2. Изменить класс `Car`, чтобы он наследовал `Vehicle`. Реализовать абстрактный метод `vehicleType()`, возвращающий `"Car"`. В конструкторе класса `Car` использовать поля и методы родительского класса.
3. Изменить класс `ElectricCar`, чтобы он наследовал `Car`. Добавить поле `batteryCapacity` и методы для работы с ним. Реализовать метод `vehicleType()`, возвращающий `"Electric Car"`. Использовать `protected`-поле `engineType` для установки значения `"Electric"` в классе `ElectricCar`.
4. Использовать полиморфизм в тестовом классе `TestCar` для работы с объектами `Car` и `ElectricCar` через ссылки на родительские классы. Создать объекты `Car` и `ElectricCar`, изменить их свойства с помощью сеттеров и вывести информацию на экран с помощью метода `toString()`.
5. Включить инкапсуляцию: убедиться, что поля каждого класса имеют доступ через методы (геттеры и сеттеры), а не напрямую.

### 5.2 Описание решения

1. **Класс `vehicles.Vehicle`**:
   - Объявлен как `public abstract class Vehicle`.
   - Инкапсулирует поля `model`, `license`, `color`, `year`, `ownerName`, `insuranceNumber` с модификатором `private` и поле `engineType` с модификатором `protected`.
   - Содержит абстрактный метод `public abstract String vehicleType()`.
   - Предоставляет геттеры и сеттеры для каждого поля, а также метод `toString()`, который полиморфно вызывает `vehicleType()` и `additionalDetails()`.
2. **Класс `vehicles.Car`**:
   - Наследует `Vehicle` (`public class Car extends Vehicle`).
   - В конструкторах использует конструктор `super(...)`, сеттеры родительского класса (`setModel()`, `setLicense()`, `setColor()`, `setYear()`, `setOwnerName()`, `setInsuranceNumber()`) и защищённое поле `this.engineType`.
   - Реализует метод `vehicleType()`, возвращая строку `"Car"`.
3. **Класс `vehicles.ElectricCar`**:
   - Наследует `Car` (`public class ElectricCar extends Car`).
   - Содержит приватное поле `private int batteryCapacity` и методы `getBatteryCapacity()` / `setBatteryCapacity()`.
   - В конструкторе устанавливает `this.engineType = "Electric"` через `protected`-поле родительского класса.
   - Переопределяет метод `vehicleType()`, возвращая `"Electric Car"`, и добавляет вывод ёмкости батареи в `toString()` через `additionalDetails()`.
4. **Класс `app.TestCar`**:
   - Создаёт экземпляры `Car` и `ElectricCar`.
   - Сохраняет ссылки на них в переменные типа `Vehicle` (`Vehicle carAsVehicle = car; Vehicle electricCarAsVehicle = electricCar;`).
   - Изменяет свойства объектов с помощью сеттеров и выводит информацию через полиморфный вызов `vehicleType()` и `toString()`.

### 5.3 Код программы

**Файл `vehicles/Vehicle.java`:**

```java
package vehicles;

/**
 * Базовый абстрактный класс с общими характеристиками транспортного средства.
 */
public abstract class Vehicle {
    private String model;
    private String license;
    private String color;
    private int year;
    private String ownerName;
    private String insuranceNumber;
    protected String engineType;

    protected Vehicle() {
        this("", "", "", 0, "", "", "Unknown");
    }

    protected Vehicle(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber,
            String engineType) {
        this.model = model;
        this.license = license;
        this.color = color;
        this.year = year;
        this.ownerName = ownerName;
        this.insuranceNumber = insuranceNumber;
        this.engineType = engineType;
    }

    /** Возвращает конкретный тип транспортного средства. */
    public abstract String vehicleType();

    public String getModel() {
        return model;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public String getLicense() {
        return license;
    }

    public void setLicense(String license) {
        this.license = license;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }

    public String getOwnerName() {
        return ownerName;
    }

    public void setOwnerName(String ownerName) {
        this.ownerName = ownerName;
    }

    public String getInsuranceNumber() {
        return insuranceNumber;
    }

    public void setInsuranceNumber(String insuranceNumber) {
        this.insuranceNumber = insuranceNumber;
    }

    public String getEngineType() {
        return engineType;
    }

    public void setEngineType(String engineType) {
        this.engineType = engineType;
    }

    /** Дополнительные характеристики, которые добавляют подклассы. */
    protected String additionalDetails() {
        return "";
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder(vehicleType())
                .append("{model='").append(model).append('\'')
                .append(", license='").append(license).append('\'')
                .append(", color='").append(color).append('\'')
                .append(", year=").append(year)
                .append(", ownerName='").append(ownerName).append('\'')
                .append(", insuranceNumber='").append(insuranceNumber).append('\'')
                .append(", engineType='").append(engineType).append('\'');

        String extra = additionalDetails();
        if (!extra.isEmpty()) {
            result.append(", ").append(extra);
        }
        return result.append('}').toString();
    }
}
```

**Файл `vehicles/Car.java`:**

```java
package vehicles;

/** Обычный автомобиль, наследующий абстрактный класс Vehicle. */
public class Car extends Vehicle {
    private static final String DEFAULT_ENGINE_TYPE = "Gasoline";

    public Car() {
        super();
        this.engineType = DEFAULT_ENGINE_TYPE;
    }

    public Car(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber) {
        // Использование методов (сеттеров) и защищённого поля родительского класса Vehicle
        setModel(model);
        setLicense(license);
        setColor(color);
        setYear(year);
        setOwnerName(ownerName);
        setInsuranceNumber(insuranceNumber);
        this.engineType = DEFAULT_ENGINE_TYPE;
    }

    public Car(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber,
            String engineType) {
        super(model, license, color, year, ownerName, insuranceNumber, engineType);
        this.engineType = engineType;
    }

    @Override
    public String vehicleType() {
        return "Car";
    }
}
```

**Файл `vehicles/ElectricCar.java`:**

```java
package vehicles;

/** Электромобиль, наследующий характеристики обычного автомобиля. */
public class ElectricCar extends Car {
    private int batteryCapacity;

    public ElectricCar() {
        this("", "", "", 0, "", "", 0);
    }

    public ElectricCar(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber,
            int batteryCapacity) {
        super(model, license, color, year, ownerName, insuranceNumber, "Electric");
        // engineType защищён в Vehicle и доступен наследникам.
        this.engineType = "Electric";
        this.batteryCapacity = batteryCapacity;
    }

    public int getBatteryCapacity() {
        return batteryCapacity;
    }

    public void setBatteryCapacity(int batteryCapacity) {
        this.batteryCapacity = batteryCapacity;
    }

    @Override
    public String vehicleType() {
        return "Electric Car";
    }

    @Override
    protected String additionalDetails() {
        return "batteryCapacity=" + batteryCapacity + " kWh";
    }
}
```

**Файл `app/TestCar.java`:**

```java
package app;

import vehicles.Car;
import vehicles.ElectricCar;
import vehicles.Vehicle;

/**
 * Часть 2. Задача #1.
 * Тестовый класс для проверки абстрактного класса Vehicle,
 * инкапсуляции, наследования и полиморфизма.
 */
public class TestCar {
    public static void main(String[] args) {
        System.out.println("=== Часть 2. Задача #1 ===");

        // 1. Создание объектов Car и ElectricCar
        Car car = new Car(
                "Volvo XC60", "A123AA 77", "Black", 2022,
                "Ivan Ivanov", "INS-CAR-001", "Petrol");
        ElectricCar electricCar = new ElectricCar(
                "Tesla Model 3", "B456BB 77", "White", 2024,
                "Maria Smirnova", "INS-EV-002", 75);

        // 2. Полиморфизм: ссылки на родительский класс Vehicle
        Vehicle carAsVehicle = car;
        Vehicle electricCarAsVehicle = electricCar;

        System.out.println("Исходные данные транспортных средств (до изменения):");
        System.out.println(carAsVehicle.toString());
        System.out.println(electricCarAsVehicle.toString());
        System.out.println();

        // 3. Изменение свойств через сеттеры (инкапсуляция)
        updateCommonProperties(
                carAsVehicle, "Volvo XC60 Recharge", "A123AA 77", "Graphite",
                2023, "Ivan Petrov", "INS-CAR-101", "Hybrid");
        updateCommonProperties(
                electricCarAsVehicle, "Tesla Model 3 Long Range", "B456BB 77", "Red",
                2025, "Maria Smirnova", "INS-EV-202", electricCarAsVehicle.getEngineType());
        electricCar.setBatteryCapacity(82);

        // 4. Полиморфный вывод информации через массив ссылок типа Vehicle
        System.out.println("Данные после изменения свойств через сеттеры:");
        Vehicle[] vehicles = {carAsVehicle, electricCarAsVehicle};
        for (Vehicle vehicle : vehicles) {
            // Полиморфный вызов методов vehicleType() и toString()
            System.out.println("Тип ТС (vehicleType): " + vehicle.vehicleType());
            System.out.println("Описание (toString):  " + vehicle.toString());
            System.out.println("------------------------------------------");
        }
    }

    private static void updateCommonProperties(
            Vehicle vehicle,
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber,
            String engineType) {
        vehicle.setModel(model);
        vehicle.setLicense(license);
        vehicle.setColor(color);
        vehicle.setYear(year);
        vehicle.setOwnerName(ownerName);
        vehicle.setInsuranceNumber(insuranceNumber);
        vehicle.setEngineType(engineType);
    }
}
```

### 5.4 Результат работы программы (Часть 2. Задача #1)

Команды для компиляции и запуска из каталога `java/pr_2`:

```sh
javac -d /tmp/pr2-classes vehicles/*.java app/*.java
java -cp /tmp/pr2-classes app.TestCar
```

Ожидаемый вывод в терминале:

```text
=== Часть 2. Задача #1 ===
Исходные данные транспортных средств (до изменения):
Car{model='Volvo XC60', license='A123AA 77', color='Black', year=2022, ownerName='Ivan Ivanov', insuranceNumber='INS-CAR-001', engineType='Petrol'}
Electric Car{model='Tesla Model 3', license='B456BB 77', color='White', year=2024, ownerName='Maria Smirnova', insuranceNumber='INS-EV-002', engineType='Electric', batteryCapacity=75 kWh}

Данные после изменения свойств через сеттеры:
Тип ТС (vehicleType): Car
Описание (toString):  Car{model='Volvo XC60 Recharge', license='A123AA 77', color='Graphite', year=2023, ownerName='Ivan Petrov', insuranceNumber='INS-CAR-101', engineType='Hybrid'}
------------------------------------------
Тип ТС (vehicleType): Electric Car
Описание (toString):  Electric Car{model='Tesla Model 3 Long Range', license='B456BB 77', color='Red', year=2025, ownerName='Maria Smirnova', insuranceNumber='INS-EV-202', engineType='Electric', batteryCapacity=82 kWh}
------------------------------------------
```

> 📸 **[СКРИНШОТ №3 — Часть 2. Задача #1]**  
> *Вставьте сюда скриншот запуска `app.TestCar` в терминале / IDE (демонстрация полиморфного вызова `vehicleType()` и `toString()` для объектов `Car` и `ElectricCar` через ссылки типа `Vehicle` до и после изменения свойств сеттерами).*

---

## 6. Заключение

В ходе выполнения практической работы №2 были изучены и практически применены ключевые принципы объектно-ориентированного программирования в Java:
1. **Организация кода по пакетам**: классы предметной области выделены в пакет `vehicles`, а демонстрационные и тестовые классы — в пакет `app`.
2. **Инкапсуляция и модификаторы доступа**: внутренние поля классов защищены от прямого несанкционированного изменения модификатором `private`, а чтение и модификация состояния объектов выполняются через публичные геттеры и сеттеры. Поле `engineType` объявлено как `protected`, что позволило обращаться к нему напрямую в классах-наследниках (`ElectricCar`), сохранив защиту от прямого доступа из внешних пакетов.
3. **Наследование и абстракция**: общие свойства транспортных средств вынесены в абстрактный класс `Vehicle` с абстрактным методом `vehicleType()`, от которого построена иерархия `Vehicle -> Car -> ElectricCar`.
4. **Полиморфизм**: в тестовом классе `TestCar` продемонстрирована работа с объектами `Car` и `ElectricCar` через ссылки на базовый тип `Vehicle` с динамическим вызовом переопределённых методов `vehicleType()` и `toString()`.
