# Отчёт по практической работе №3

**Дисциплина:** Программирование на языке Java  
**Тема:** Объекты и классы: абстрактные классы и интерфейсы, пакеты.

---

## Содержание

1. [Цель работы и теоретические сведения](#1-цель-работы-и-теоретические-сведения)
2. [Постановка задачи](#2-постановка-задачи)
3. [Структура проекта](#3-структура-проекта)
4. [Ход выполнения работы](#4-ход-выполнения-работы)
5. [Результат работы программы](#5-результат-работы-программы)
6. [Заключение](#6-заключение)

---

## 1. Цель работы и теоретические сведения

### 1.1 Цель работы

Изучить организацию программы по пакетам (`package`), создание и использование абстрактных классов (`abstract class`) и интерфейсов (`interface`), наследование классов и реализацию интерфейсов, а также полиморфизм при работе с объектами через ссылки базового типа и типа интерфейса.

### 1.2 Краткие теоретические сведения

**Пакеты (`package`).** Пакет — это механизм группировки логически связанных классов и интерфейсов. Пакет задаётся директивой `package` в первой строке исходного файла, а его имя соответствует структуре каталогов: класс из пакета `vehicles` размещается в каталоге `vehicles`. Классы других пакетов подключаются директивой `import`. Использование пакетов позволяет избежать конфликтов имён и управлять видимостью классов.

**Абстрактные классы.** Абстрактный класс объявляется с модификатором `abstract`, может содержать как обычные, так и абстрактные методы (объявленные без тела). Создать экземпляр абстрактного класса нельзя. Класс-наследник обязан реализовать все унаследованные абстрактные методы либо сам должен быть объявлен абстрактным. При этом абстрактный класс может содержать поля, конструкторы и готовую реализацию части методов, которые наследуются подклассами.

**Интерфейсы.** Интерфейс (`interface`) описывает контракт поведения, которому должен соответствовать класс. Методы интерфейса неявно являются `public abstract`, а поля — `public static final` (константами). Класс реализует интерфейс с помощью ключевого слова `implements` и обязан реализовать все методы интерфейса; при этом один класс может реализовывать несколько интерфейсов. Начиная с Java 8 интерфейсы могут также содержать методы с реализацией (`default` и `static`).

**Сравнение абстрактного класса и интерфейса:**

| Свойство | Абстрактный класс | Интерфейс |
|---|---|---|
| Ключевое слово | `abstract class` | `interface` |
| Отношение | `extends` — наследование (один родитель) | `implements` — реализация (несколько интерфейсов) |
| Создание экземпляра | невозможно | невозможно |
| Конструкторы | есть | нет |
| Поля | любых типов, в том числе нестатические | только `public static final` (константы) |
| Методы | обычные и абстрактные | абстрактные, `default`, `static` (Java 8+) |
| Смысл | отношение «является» (is-a), общая реализация | контракт поведения (can-do) |

**Полиморфизм.** Ссылка на абстрактный класс `Vehicle` может указывать на объект `Car` или `ElectricCar`; при вызове переопределённых методов `vehicleType()` и `toString()` выполняется метод фактического класса объекта (динамическое связывание). Аналогично ссылка на интерфейс `ElectricVehicle` может указывать на объект `ElectricCar`, что позволяет работать с методами `getBatteryCapacity()` и `setBatteryCapacity()`, не зная конкретного класса объекта.

---

## 2. Постановка задачи

**Практическая работа №3. Объекты и классы: абстрактные классы и интерфейсы, пакеты.**

**Часть 1. Задача #1.**

Ваша программа должна быть организована по пакетам:
- пакет `vehicles` — для классов `Vehicle`, `Car`, `ElectricCar` и интерфейса `ElectricVehicle`;
- пакет `app` — для тестового класса `TestCar`.

Создайте абстрактный класс `Vehicle`, который будет представлять общие характеристики любого транспортного средства. Включите следующие поля: `model`, `license`, `color`, `year`, `ownerName`, `insuranceNumber`, `engineType`.

Определите методы: геттеры и сеттеры для каждого поля, а также метод `toString()`, который возвращает строку с описанием транспортного средства.

Добавьте абстрактный метод `vehicleType()`, который будет возвращать тип транспортного средства (например, `"Car"`, `"Electric Car"`). Класс `Car` должен наследовать абстрактный класс `Vehicle`. В конструкторе задайте тип двигателя как `"Combustion"`. Реализуйте метод `vehicleType()`, который возвращает `"Car"`.

Определите интерфейс `ElectricVehicle`, который будет описывать специфические методы для электромобилей. В интерфейсе должны быть следующие методы: `getBatteryCapacity()` и `setBatteryCapacity()`. Класс `ElectricCar` должен наследовать класс `Car` и реализовывать интерфейс `ElectricVehicle`. Реализуйте методы интерфейса и добавьте поле `batteryCapacity` для хранения информации о ёмкости батареи. В конструкторе задайте тип двигателя как `"Electric"`.

**Требования к тестированию программы:**
- созданы экземпляры классов `Car` и `ElectricCar`;
- изменение года выпуска и имени владельца;
- изменение страхового номера;
- получение информации о ёмкости батареи у электромобиля;
- вывод информации о транспортных средствах в консоль с помощью метода `toString()`.

---

## 3. Структура проекта

```text
java/pr_3/
├── README.md                     # Описание структуры проекта, файлов и запуска
├── report.md                     # Отчёт о проделанной работе
├── vehicles/                     # Пакет vehicles
│   ├── Vehicle.java              # Абстрактный класс транспортного средства
│   ├── Car.java                  # Класс обычного автомобиля (наследует Vehicle)
│   ├── ElectricVehicle.java      # Интерфейс электромобиля
│   └── ElectricCar.java          # Класс электромобиля (наследует Car, реализует ElectricVehicle)
└── app/                          # Пакет app
    └── TestCar.java              # Тестовый класс с методом main
```

| Требование задания | Файл |
|---|---|
| Пакет `vehicles` | `vehicles/Vehicle.java`, `vehicles/Car.java`, `vehicles/ElectricVehicle.java`, `vehicles/ElectricCar.java` |
| Пакет `app` | `app/TestCar.java` |
| Абстрактный класс `Vehicle`, поля и методы доступа, `toString()` | `vehicles/Vehicle.java` |
| Абстрактный метод `vehicleType()` | `vehicles/Vehicle.java` (реализация — в `Car.java` и `ElectricCar.java`) |
| Класс `Car`, тип двигателя `"Combustion"` | `vehicles/Car.java` |
| Интерфейс `ElectricVehicle` | `vehicles/ElectricVehicle.java` |
| Класс `ElectricCar`, поле `batteryCapacity`, тип двигателя `"Electric"` | `vehicles/ElectricCar.java` |
| Тестирование программы | `app/TestCar.java` |

---

## 4. Ход выполнения работы

### 4.1 Абстрактный класс `Vehicle`

В пакете `vehicles` создан абстрактный класс `Vehicle`. Все общие характеристики транспортного средства объявлены приватными (`private`), чтобы доступ к ним осуществлялся только через публичные геттеры и сеттеры — это обеспечивает инкапсуляцию. Исключение составляет поле `engineType`: оно объявлено как `protected`, поскольку тип двигателя задаётся непосредственно в конструкторах классов-наследников `Car` и `ElectricCar`.

Класс содержит:
- два защищённых конструктора: по умолчанию и с полным набором параметров;
- абстрактный метод `vehicleType()`, который обязаны реализовать все конкретные подклассы;
- геттеры и сеттеры для каждого из семи полей;
- переопределённый метод `toString()`, возвращающий строку с описанием транспортного средства. Общая часть описания формируется в защищённом методе `commonDescription()`, чтобы подклассы могли дополнять её своими полями.

**Файл `vehicles/Vehicle.java`:**

```java
package vehicles;

/**
 * Абстрактный базовый класс с общими характеристиками транспортного средства.
 *
 * <p>Содержит поля {@code model}, {@code license}, {@code color}, {@code year},
 * {@code ownerName}, {@code insuranceNumber}, {@code engineType}, геттеры и сеттеры
 * для каждого поля, а также абстрактный метод {@link #vehicleType()}.</p>
 */
public abstract class Vehicle {
    private String model;
    private String license;
    private String color;
    private int year;
    private String ownerName;
    private String insuranceNumber;
    /** Тип двигателя: доступен классам-наследникам напрямую. */
    protected String engineType;

    protected Vehicle() {
        this("Неизвестно", "Неизвестно", "Неизвестно", 0, "Неизвестно", "Неизвестно", "Неизвестно");
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

    /**
     * Возвращает тип транспортного средства.
     *
     * @return строковое обозначение типа, например {@code "Car"} или {@code "Electric Car"}
     */
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

    /**
     * Формирует общую часть описания транспортного средства без закрывающей скобки.
     * Подклассы дополняют описание собственными полями.
     */
    protected String commonDescription() {
        StringBuilder result = new StringBuilder(vehicleType())
                .append("{model='").append(model).append('\'')
                .append(", license='").append(license).append('\'')
                .append(", color='").append(color).append('\'')
                .append(", year=").append(year)
                .append(", ownerName='").append(ownerName).append('\'')
                .append(", insuranceNumber='").append(insuranceNumber).append('\'')
                .append(", engineType='").append(engineType).append('\'');
        return result.toString();
    }

    /**
     * Возвращает строку с описанием транспортного средства.
     */
    @Override
    public String toString() {
        return commonDescription() + "}";
    }
}
```

### 4.2 Интерфейс `ElectricVehicle`

Интерфейс описывает специфические методы электромобиля — чтение и изменение ёмкости батареи. Методы интерфейса неявно являются публичными и абстрактными, поэтому класс, реализующий интерфейс, обязан предоставить их реализацию.

**Файл `vehicles/ElectricVehicle.java`:**

```java
package vehicles;

/**
 * Интерфейс, описывающий специфические методы электромобиля.
 *
 * <p>Все методы интерфейса являются публичными и абстрактными
 * (модификаторы {@code public abstract} указываются неявно).</p>
 */
public interface ElectricVehicle {
    /**
     * Возвращает ёмкость батареи электромобиля.
     *
     * @return ёмкость батареи в киловатт-часах
     */
    int getBatteryCapacity();

    /**
     * Задаёт ёмкость батареи электромобиля.
     *
     * @param batteryCapacity новая ёмкость батареи в киловатт-часах
     */
    void setBatteryCapacity(int batteryCapacity);
}
```

### 4.3 Класс `Car`

Класс `Car` наследует абстрактный класс `Vehicle` и реализует его абстрактный метод `vehicleType()`, возвращая строку `"Car"`. В обоих конструкторах тип двигателя задаётся как `"Combustion"` — двигатель внутреннего сгорания.

**Файл `vehicles/Car.java`:**

```java
package vehicles;

/**
 * Обычный автомобиль, наследующий абстрактный класс {@link Vehicle}.
 *
 * <p>В конструкторах тип двигателя задаётся как {@code "Combustion"} —
 * двигатель внутреннего сгорания.</p>
 */
public class Car extends Vehicle {
    /** Тип двигателя обычного автомобиля. */
    private static final String ENGINE_TYPE = "Combustion";

    public Car() {
        super();
        this.engineType = ENGINE_TYPE;
    }

    public Car(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber) {
        super(model, license, color, year, ownerName, insuranceNumber, ENGINE_TYPE);
    }

    /**
     * Возвращает тип транспортного средства.
     *
     * @return {@code "Car"}
     */
    @Override
    public String vehicleType() {
        return "Car";
    }
}
```

### 4.4 Класс `ElectricCar`

Класс `ElectricCar` наследует `Car` и реализует интерфейс `ElectricVehicle`:

```java
public class ElectricCar extends Car implements ElectricVehicle
```

В классе добавлено приватное поле `batteryCapacity` (ёмкость батареи в кВт·ч) и реализованы методы интерфейса `getBatteryCapacity()` и `setBatteryCapacity()`. В конструкторах после вызова конструктора родительского класса `super(...)` тип двигателя переопределяется как `"Electric"`. Метод `vehicleType()` возвращает `"Electric Car"`, а метод `toString()` дополняет описание, полученное из родительского класса, значением ёмкости батареи.

**Файл `vehicles/ElectricCar.java`:**

```java
package vehicles;

/**
 * Электромобиль: наследует класс {@link Car} и реализует интерфейс
 * {@link ElectricVehicle}.
 *
 * <p>Дополнительно хранит ёмкость батареи, а в конструкторах задаёт
 * тип двигателя {@code "Electric"}.</p>
 */
public class ElectricCar extends Car implements ElectricVehicle {
    /** Ёмкость батареи в киловатт-часах. */
    private int batteryCapacity;

    public ElectricCar() {
        super();
        this.engineType = "Electric";
    }

    public ElectricCar(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber,
            int batteryCapacity) {
        super(model, license, color, year, ownerName, insuranceNumber);
        this.engineType = "Electric";
        this.batteryCapacity = batteryCapacity;
    }

    @Override
    public int getBatteryCapacity() {
        return batteryCapacity;
    }

    @Override
    public void setBatteryCapacity(int batteryCapacity) {
        this.batteryCapacity = batteryCapacity;
    }

    /**
     * Возвращает тип транспортного средства.
     *
     * @return {@code "Electric Car"}
     */
    @Override
    public String vehicleType() {
        return "Electric Car";
    }

    /**
     * Возвращает строку с описанием электромобиля, дополненную ёмкостью батареи.
     */
    @Override
    public String toString() {
        return commonDescription() + ", batteryCapacity=" + batteryCapacity + " kWh}";
    }
}
```

### 4.5 Тестовый класс `TestCar`

Тестовый класс `TestCar` пакета `app` последовательно выполняет все требования к тестированию:

1. создаёт экземпляры классов `Car` и `ElectricCar`;
2. изменяет год выпуска и имя владельца через сеттеры `setYear()` и `setOwnerName()`;
3. изменяет страховой номер через сеттер `setInsuranceNumber()`;
4. получает информацию о ёмкости батареи электромобиля через ссылку типа `ElectricVehicle` и изменяет её методом `setBatteryCapacity()`;
5. выводит информацию о транспортных средствах в консоль методом `toString()` — как напрямую, так и полиморфно, через массив ссылок `Vehicle[]`.

**Файл `app/TestCar.java`:**

```java
package app;

import vehicles.Car;
import vehicles.ElectricCar;
import vehicles.ElectricVehicle;
import vehicles.Vehicle;

/**
 * Часть 1. Задача #1.
 *
 * <p>Тестовый класс для проверки иерархии {@code Vehicle -> Car -> ElectricCar},
 * интерфейса {@link ElectricVehicle} и работы методов доступа и {@code toString()}.</p>
 */
public class TestCar {
    public static void main(String[] args) {
        System.out.println("=== Практическая работа №3. Часть 1. Задача #1 ===");
        System.out.println();

        // 1. Создание экземпляров классов Car и ElectricCar
        Car car = new Car(
                "Лада Веста", "А123ВС 77", "Белый", 2019,
                "Иванов Иван Иванович", "СТР-001-77");
        ElectricCar electricCar = new ElectricCar(
                "Tesla Model 3", "В456ЕЕ 77", "Красный", 2023,
                "Петрова Мария Сергеевна", "СТР-002-77", 75);

        System.out.println("1. Созданные экземпляры Car и ElectricCar:");
        System.out.println(car.toString());
        System.out.println(electricCar.toString());
        System.out.println();

        // 2. Изменение года выпуска и имени владельца
        car.setYear(2021);
        car.setOwnerName("Сидоров Алексей Петрович");
        electricCar.setYear(2024);
        electricCar.setOwnerName("Смирнова Анна Викторовна");

        System.out.println("2. Изменение года выпуска и имени владельца:");
        System.out.println("   Car:         year=" + car.getYear()
                + ", ownerName='" + car.getOwnerName() + "'");
        System.out.println("   ElectricCar: year=" + electricCar.getYear()
                + ", ownerName='" + electricCar.getOwnerName() + "'");
        System.out.println();

        // 3. Изменение страхового номера
        car.setInsuranceNumber("СТР-101-77");
        electricCar.setInsuranceNumber("СТР-102-77");

        System.out.println("3. Изменение страхового номера:");
        System.out.println("   Car:         insuranceNumber='" + car.getInsuranceNumber() + "'");
        System.out.println("   ElectricCar: insuranceNumber='" + electricCar.getInsuranceNumber() + "'");
        System.out.println();

        // 4. Получение информации о ёмкости батареи через интерфейс ElectricVehicle
        ElectricVehicle electricVehicle = electricCar;
        System.out.println("4. Ёмкость батареи электромобиля (ссылка типа ElectricVehicle):");
        System.out.println("   getBatteryCapacity() = " + electricVehicle.getBatteryCapacity() + " кВт·ч");
        electricVehicle.setBatteryCapacity(82);
        System.out.println("   После setBatteryCapacity(82): " + electricVehicle.getBatteryCapacity() + " кВт·ч");
        System.out.println();

        // 5. Вывод информации о транспортных средствах методом toString()
        System.out.println("5. Вывод информации о транспортных средствах через toString():");
        Vehicle[] vehicles = {car, electricCar};
        for (Vehicle vehicle : vehicles) {
            System.out.println("   Тип ТС (vehicleType): " + vehicle.vehicleType());
            System.out.println("   Описание (toString):  " + vehicle.toString());
            System.out.println("   ------------------------------------------");
        }
    }
}
```

---

## 5. Результат работы программы

Команды для компиляции и запуска из каталога `java/pr_3`:

```sh
javac -encoding UTF-8 -d /tmp/pr3-classes vehicles/*.java app/*.java
java -cp /tmp/pr3-classes app.TestCar
```

Ожидаемый вывод в терминале:

```text
=== Практическая работа №3. Часть 1. Задача #1 ===

1. Созданные экземпляры Car и ElectricCar:
Car{model='Лада Веста', license='А123ВС 77', color='Белый', year=2019, ownerName='Иванов Иван Иванович', insuranceNumber='СТР-001-77', engineType='Combustion'}
Electric Car{model='Tesla Model 3', license='В456ЕЕ 77', color='Красный', year=2023, ownerName='Петрова Мария Сергеевна', insuranceNumber='СТР-002-77', engineType='Electric', batteryCapacity=75 kWh}

2. Изменение года выпуска и имени владельца:
   Car:         year=2021, ownerName='Сидоров Алексей Петрович'
   ElectricCar: year=2024, ownerName='Смирнова Анна Викторовна'

3. Изменение страхового номера:
   Car:         insuranceNumber='СТР-101-77'
   ElectricCar: insuranceNumber='СТР-102-77'

4. Ёмкость батареи электромобиля (ссылка типа ElectricVehicle):
   getBatteryCapacity() = 75 кВт·ч
   После setBatteryCapacity(82): 82 кВт·ч

5. Вывод информации о транспортных средствах через toString():
   Тип ТС (vehicleType): Car
   Описание (toString):  Car{model='Лада Веста', license='А123ВС 77', color='Белый', year=2021, ownerName='Сидоров Алексей Петрович', insuranceNumber='СТР-101-77', engineType='Combustion'}
   ------------------------------------------
   Тип ТС (vehicleType): Electric Car
   Описание (toString):  Electric Car{model='Tesla Model 3', license='В456ЕЕ 77', color='Красный', year=2024, ownerName='Смирнова Анна Викторовна', insuranceNumber='СТР-102-77', engineType='Electric', batteryCapacity=82 kWh}
   ------------------------------------------
```

| Требование к тестированию | Строки вывода |
|---|---|
| Созданы экземпляры `Car` и `ElectricCar` | блок 1 |
| Изменение года выпуска и имени владельца | блок 2 |
| Изменение страхового номера | блок 3 |
| Получение информации о ёмкости батареи | блок 4 |
| Вывод информации методом `toString()` | блок 5 |

> 📸 **[СКРИНШОТ №1 — Часть 1. Задача #1]**  
> *Вставьте сюда скриншот запуска `app.TestCar` в терминале / IDE (создание объектов `Car` и `ElectricCar`, изменение года выпуска, имени владельца и страхового номера, получение ёмкости батареи и вывод описаний методом `toString()`).*

---

## 6. Заключение

В ходе выполнения практической работы №3 были изучены и практически применены механизмы объектно-ориентированного программирования в Java:

1. **Организация программы по пакетам.** Классы предметной области (`Vehicle`, `Car`, `ElectricCar`) и интерфейс `ElectricVehicle` размещены в пакете `vehicles`, а тестовый класс `TestCar` — в пакете `app`. Структура каталогов соответствует именам пакетов, классы подключаются директивами `import`.
2. **Абстрактный класс.** Общие характеристики транспортного средства вынесены в абстрактный класс `Vehicle` с абстрактным методом `vehicleType()`. Создать экземпляр такого класса нельзя, а каждый конкретный подкласс обязан реализовать метод `vehicleType()`.
3. **Интерфейс.** Интерфейс `ElectricVehicle` задаёт контракт поведения электромобиля (`getBatteryCapacity()` и `setBatteryCapacity()`). Класс `ElectricCar` реализует этот интерфейс, предоставляя конкретную реализацию обоих методов и поле `batteryCapacity`.
4. **Наследование и инкапсуляция.** Построена иерархия `Vehicle -> Car -> ElectricCar`. Поля класса `Vehicle` скрыты модификатором `private` и доступны через геттеры и сеттеры, а поле `engineType` объявлено `protected`, что позволяет задавать тип двигателя в конструкторах наследников (`"Combustion"` для `Car` и `"Electric"` для `ElectricCar`).
5. **Полиморфизм.** В тестовом классе продемонстрирована работа с объектами через ссылку типа интерфейса `ElectricVehicle` и через массив ссылок базового типа `Vehicle[]`, что позволяет единообразно вызывать переопределённые методы `vehicleType()` и `toString()`.

Таким образом, все требования постановки задачи выполнены: программа организована по пакетам, содержит абстрактный класс и интерфейс, а тестовый класс демонстрирует создание объектов, изменение их свойств, получение ёмкости батареи и вывод описаний методом `toString()`.
