# Отчёт по практической работе №3

**Дисциплина:** Программирование на языке Java  
**Тема:** Объекты и классы: абстрактные классы и интерфейсы, пакеты.

---

## Содержание

1. [Цель работы и теоретические сведения](#1-цель-работы-и-теоретические-сведения)
2. [Постановка задачи](#2-постановка-задачи)
3. [Структура проекта](#3-структура-проекта)
4. [Ход выполнения работы (Часть 1)](#4-ход-выполнения-работы-часть-1)
5. [Результат работы программы (Часть 1)](#5-результат-работы-программы-часть-1)
6. [Часть 2. Проект bikeproject: ответы на вопросы и модернизация](#6-часть-2-проект-bikeproject-ответы-на-вопросы-и-модернизация)
7. [Результат работы программы (Часть 2)](#7-результат-работы-программы-часть-2)
8. [Заключение](#8-заключение)

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

**Часть 2. Проект `bikeproject` (проектирование интерфейсов).**

Импортируйте файл `bikeproject.zip` в папку на вашем компьютере — проект размещён в каталоге [`java/pr_3/uploaded`](./uploaded) (Eclipse-проект `bikeproject`).

Полезные ссылки: [Интерфейсы](https://docs.oracle.com/javase/tutorial/java/concepts/interface.html), [Методы интерфейса](https://docs.oracle.com/javase/tutorial/java/IandI/override.html), [Использование `super`](https://docs.oracle.com/javase/tutorial/java/IandI/super.html).

**Ответьте на следующие вопросы:**
- Приведите пример примитивного типа данных, который используется для хранения полей в классе.
- Приведите пример того, где происходит конкатенация строк.
- Каковы имена объектов, созданных в этой программе?
- Сколько конструкторов у каждого класса?
- Наследование является частью этой программы. Определите супер- и подклассы из этой программы.
- Горные и шоссейные велосипеды могут быть сконструированы либо с использованием значений по умолчанию (стандартный велосипед), либо настроены в соответствии с потребностями клиента. Используя таблицу значений, определите примеры значений, назначенных одному из каждого типа стандартного велосипеда.

**Модернизация программы:**
1. Создайте интерфейс в проекте велосипеда, который устанавливает название компании-производителя велосипедов как неизменяемое значение. Он также определяет методы, которые должны быть реализованы любым классом, использующим интерфейс.
2. Создайте интерфейс с именем **`MountainParts`**, который имеет константу с именем **`TERRAIN`**, хранящую строковое значение `"off_road"`. Интерфейс определит два метода, которые принимают строковый аргумент с именем `newValue`, и два метода, которые будут возвращать текущее значение поля экземпляра. Методы должны быть названы: `getSuspension`, `setSuspension`, `getType`, `setType`.
3. Создайте интерфейс **`RoadParts`**, который имеет константу с именем `terrain`, хранящую строковое значение `"track_racing"`. Интерфейс определит два метода, которые принимают строковый аргумент с именем `newValue`, и два метода, которые будут возвращать текущее значение поля экземпляра. Методы должны быть названы: `getTyreWidth`, `setTyreWidth`, `getPostHeight`, `setPostHeight`.
4. Используйте интерфейс **`BikeParts`** с классом **`Bike`**, добавляя любые необходимые нереализованные методы. Добавьте требуемый внутренний код для каждого из добавленных методов.
5. Используйте интерфейс **`MountainParts`** с классом **`MountainBike`**, добавив все необходимые нереализованные методы. Добавьте требуемый внутренний код для каждого из добавленных методов.
6. Используйте интерфейс **`RoadParts`** с классом **`RoadBike`**, добавив все необходимые нереализованные методы. Добавьте требуемый внутренний код для каждого из добавленных методов.
7. Запустите и протестируйте свою программу — она должна работать точно так же, как и раньше.
8. В нижней части класса драйвера обновите высоту столба для `bike1` до 20 вместо 22.
9. Выведите значения `bike1` на экран, чтобы подтвердить изменение.
10. Запустите и протестируйте свою программу.

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

**Структура проекта Части 2 (`java/pr_3/uploaded`):**

```text
java/pr_3/uploaded/                     # Eclipse-проект bikeproject из архива
├── .classpath                          # настройки проекта Eclipse (JavaSE-1.8)
├── .project
├── .settings/
│   └── org.eclipse.jdt.core.prefs
├── README (Pr. 3).md                   # задание Части 2 (проектирование интерфейсов)
├── src/bikeproject/                    # исходный код проекта
│   ├── Bike.java                       # базовый класс (реализует BikeParts)
│   ├── RoadBike.java                   # шоссейный велосипед (реализует RoadParts)
│   ├── MountainBike.java               # горный велосипед (реализует MountainParts)
│   ├── BikeDriver.java                 # драйвер с методом main
│   ├── BikeParts.java                  # интерфейс: константа MAKE и общие методы
│   ├── RoadParts.java                  # интерфейс: константа terrain и методы шоссейного велосипеда
│   └── MountainParts.java              # интерфейс: константа TERRAIN и методы горного велосипеда
└── bin/bikeproject/                    # результат компиляции (*.class)
```

---

## 4. Ход выполнения работы (Часть 1)

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

## 5. Результат работы программы (Часть 1)

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

## 6. Часть 2. Проект `bikeproject`: ответы на вопросы и модернизация

### 6.1 Исходный проект

Проект из архива `bikeproject.zip` размещён в каталоге `java/pr_3/uploaded` (Eclipse-проект: исходники — `src/bikeproject`, результат сборки — `bin`). В исходной версии объявлено четыре класса:

| Класс | Назначение | Поля | Конструкторы |
|---|---|---|---|
| `Bike` | базовый велосипед | `handleBars`, `frame`, `tyres`, `seatType` (`String`), `NumGears` (`int`), `make` (`final String`) | `Bike()`, `Bike(String, String, String, String, int)` |
| `RoadBike` | шоссейный велосипед | `tyreWidth`, `postHeight` (`int`) | `RoadBike()`, `RoadBike(int postHeight)`, `RoadBike(String, String, String, String, int, int, int)` |
| `MountainBike` | горный велосипед | `suspension`, `type` (`String`), `frameSize` (`int`) | `MountainBike()`, `MountainBike(String, String, String, String, int, String, String, int)` |
| `BikeDriver` | драйвер с методом `main` | — | явных нет |

Иерархия классов исходного проекта:

```text
Bike
├── RoadBike
└── MountainBike
```

### 6.2 Ответы на вопросы

**1. Пример примитивного типа данных для хранения полей класса.**  
Примитивный тип `int` используется для полей `NumGears` в классе `Bike`, `tyreWidth` и `postHeight` в классе `RoadBike`, `frameSize` в классе `MountainBike`. Поля типа `String` (`handleBars`, `frame`, `tyres`, `seatType`, `make`, `suspension`, `type`) примитивами не являются — это ссылочный тип.

**2. Пример конкатенации строк.**  
Конкатенация выполняется в методе `printDescription()` каждого класса при помощи оператора `+`:

```java
System.out.println("\n" + this.make + "\n"
        + "This bike has " + this.handleBars + " handlebars on a "
        + this.frame + " frame with " + this.NumGears + " gears."
        + "\nIt has a " + this.seatType + " seat with " + this.tyres + " tyres.");
```

При конкатенации строки с числом (`this.NumGears`, `this.tyreWidth`, `this.postHeight`, `this.frameSize`) число автоматически преобразуется в строку, например: `"This Roadbike bike has " + this.tyreWidth + "mm tyres"`.

**3. Имена объектов, созданных в программе.**

| Переменная | Класс | Строка в `BikeDriver` |
|---|---|---|
| `bike1` | `RoadBike` | `RoadBike bike1 = new RoadBike();` |
| `bike2` | `RoadBike` | `RoadBike bike2 = new RoadBike("drop", "tourer", "semi-grip", "comfort", 14, 25, 18);` |
| `bike3` | `MountainBike` | `MountainBike bike3 = new MountainBike();` |
| `bike4` | `Bike` | `Bike bike4 = new Bike();` |

**4. Количество конструкторов у каждого класса.**

| Класс | Количество конструкторов | Перечень |
|---|---:|---|
| `Bike` | 2 | `Bike()`, `Bike(String handleBars, String frame, String tyres, String seatType, int numGears)` |
| `RoadBike` | 3 | `RoadBike()`, `RoadBike(int postHeight)`, `RoadBike(String, String, String, String, int, int tyreWidth, int postHeight)` |
| `MountainBike` | 2 | `MountainBike()`, `MountainBike(String, String, String, String, int, String suspension, String type, int frameSize)` |
| `BikeDriver` | 0 (явных) | используется неявный конструктор по умолчанию |

**5. Наследование: суперкласс и подклассы.**  
Суперкласс (родительский класс) — `Bike`: в нём объявлены общие для всех велосипедов поля (`handleBars`, `frame`, `tyres`, `seatType`, `NumGears`, `make`) и метод `printDescription()`.  
Подклассы — `RoadBike` (`class RoadBike extends Bike`) и `MountainBike` (`class MountainBike extends Bike`): они наследуют поля и методы `Bike`, добавляют собственные поля и переопределяют `printDescription()`, дополняя описание родителя через `super.printDescription()`.  
Класс `BikeDriver` в наследовании не участвует — он только создаёт объекты и вызывает их методы.

**6. Примеры значений для стандартного велосипеда каждого типа.**

| Поле | `bike1` — `RoadBike()` (стандарт) | `bike2` — `RoadBike(...)` (под заказ) | `bike3` — `MountainBike()` (стандарт) | `bike4` — `Bike()` (стандарт) |
|---|---|---|---|---|
| `make` | Oracle Cycles | Oracle Cycles | Oracle Cycles | Oracle Cycles |
| `handleBars` | drop | drop | Bull Horn | `null` (не задано) |
| `frame` | racing | tourer | Hardtail | `null` (не задано) |
| `tyres` | tread less | semi-grip | Maxxis | `null` (не задано) |
| `seatType` | razor | comfort | dropper | `null` (не задано) |
| `NumGears` | 19 | 14 | 27 | 0 |
| `tyreWidth` | 20 | 25 | — | — |
| `postHeight` | 22 | 18 | — | — |
| `suspension` | — | — | RockShox XC32 | — |
| `type` | — | — | Pro | — |
| `frameSize` | — | — | 19 | — |

Конструктор по умолчанию каждого класса вызывает другой конструктор того же класса через `this(...)` и передаёт ему стандартный набор значений, поэтому «стандартный» велосипед отличается от «настроенного» только аргументами конструктора.

### 6.3 Создание интерфейсов (пункты 1–3)

**Пункт 1. Интерфейс `BikeParts`.** Название компании-производителя задано константой `MAKE`. Поля интерфейса неявно объявляются как `public static final`, поэтому значение `MAKE` изменить нельзя — требование «неизменяемое значение» выполняется. Тот же интерфейс определяет методы, обязательные для реализующего класса: `getMake()` (доступ к названию производителя) и `printDescription()` (вывод описания велосипеда).

**Файл `src/bikeproject/BikeParts.java`:**

```java
package bikeproject;

/**
 * Интерфейс с общими требованиями к велосипеду.
 *
 * <p>Константа {@link #MAKE} хранит название компании-производителя
 * велосипедов. В интерфейсах поля неявно объявляются как
 * {@code public static final}, поэтому значение этой константы
 * изменить нельзя.</p>
 *
 * <p>Интерфейс также определяет методы, которые обязан реализовать
 * любой класс, его использующий.</p>
 */
public interface BikeParts {
	/** Название компании-производителя велосипедов (неизменяемое значение). */
	String MAKE = "Oracle Cycles";

	/** Возвращает название компании-производителя. */
	String getMake();

	/** Выводит описание велосипеда. */
	void printDescription();
}
```

**Пункт 2. Интерфейс `MountainParts`.** Содержит константу `TERRAIN = "off_road"` и четыре метода: `getSuspension()`, `setSuspension(String newValue)`, `getType()`, `setType(String newValue)`.

**Файл `src/bikeproject/MountainParts.java`:**

```java
package bikeproject;

/**
 * Интерфейс с характеристиками горного велосипеда.
 *
 * <p>Константа {@link #TERRAIN} хранит тип местности {@code "off_road"}
 * (бездорожье). Методы интерфейса позволяют читать и изменять
 * значения полей экземпляра.</p>
 */
public interface MountainParts {
	/** Тип местности для горного велосипеда. */
	String TERRAIN = "off_road";

	/** Возвращает текущее значение поля suspension. */
	String getSuspension();

	/** Задаёт новое значение поля suspension. */
	void setSuspension(String newValue);

	/** Возвращает текущее значение поля type. */
	String getType();

	/** Задаёт новое значение поля type. */
	void setType(String newValue);
}
```

**Пункт 3. Интерфейс `RoadParts`.** Содержит константу `terrain = "track_racing"` и четыре метода: `getTyreWidth()`, `setTyreWidth(String newValue)`, `getPostHeight()`, `setPostHeight(String newValue)`.

**Файл `src/bikeproject/RoadParts.java`:**

```java
package bikeproject;

/**
 * Интерфейс с характеристиками шоссейного велосипеда.
 *
 * <p>Константа {@link #terrain} хранит тип местности
 * {@code "track_racing"} (шоссейные гонки). Методы интерфейса
 * позволяют читать и изменять значения полей экземпляра.</p>
 */
public interface RoadParts {
	/** Тип местности для шоссейного велосипеда. */
	String terrain = "track_racing";

	/** Возвращает текущее значение поля tyreWidth. */
	int getTyreWidth();

	/** Задаёт новое значение поля tyreWidth (строковый аргумент). */
	void setTyreWidth(String newValue);

	/** Возвращает текущее значение поля postHeight. */
	int getPostHeight();

	/** Задаёт новое значение поля postHeight (строковый аргумент). */
	void setPostHeight(String newValue);
}
```

### 6.4 Подключение интерфейсов к классам (пункты 4–6)

**Пункт 4. Класс `Bike` реализует `BikeParts`.** В объявление класса добавлено `implements BikeParts`. Вместо строкового литерала `"Oracle Cycles"` в конструкторах теперь используется константа интерфейса `MAKE` (единая точка хранения названия производителя). Добавлен метод `getMake()`, а метод `printDescription()` изменён с `protected` на `public`: методы интерфейса неявно являются `public`, поэтому реализующий их метод нельзя сузить по видимости.

**Файл `src/bikeproject/Bike.java`:**

```java
package bikeproject;

public class Bike implements BikeParts {

	private String handleBars, frame, tyres, seatType;
	private int NumGears;
	private final String make;
	
	public Bike(){	
		this.make = MAKE;
	}//end constructor
	
	public Bike(String handleBars, String frame, String tyres, String seatType, int numGears) {
		this.handleBars = handleBars;
		this.frame = frame;
		this.tyres = tyres;
		this.seatType = seatType;
		NumGears = numGears;
		this.make = MAKE;
	}//end constructor

	@Override
	public String getMake() {
		return this.make;
	}//end method getMake

	@Override
	public void printDescription()
	{
		System.out.println("\n" + this.make + "\n" 
				          + "This bike has " + this.handleBars + " handlebars on a " 
				          + this.frame + " frame with " + this.NumGears + " gears."  
				          + "\nIt has a " + this.seatType + " seat with " + this.tyres + " tyres.");
	}//end method printDescription

}//end class Bike
```

**Пункт 5. Класс `MountainBike` реализует `MountainParts`.** В объявление класса добавлено `implements MountainParts`, реализованы четыре метода интерфейса: геттеры возвращают текущие значения полей `suspension` и `type`, сеттеры записывают в эти поля значение аргумента `newValue`.

**Файл `src/bikeproject/MountainBike.java`:**

```java
package bikeproject;

public class MountainBike extends Bike implements MountainParts{

	private String suspension, type;
	private int frameSize;
	
	public MountainBike()
	{
		this("Bull Horn", "Hardtail", "Maxxis", "dropper", 27, "RockShox XC32", "Pro", 19);
	}//end constructor
	
    public MountainBike(String handleBars, String frame, String tyres, String seatType, int numGears, 
    		            String suspension, String type, int frameSize) {
		super(handleBars, frame, tyres, seatType, numGears);
		this.suspension = suspension;
		this.type = type;
		this.frameSize = frameSize;
	}//end constructor

	@Override
	public String getSuspension() {
		return this.suspension;
	}//end method getSuspension

	@Override
	public void setSuspension(String newValue) {
		this.suspension = newValue;
	}//end method setSuspension

	@Override
	public String getType() {
		return this.type;
	}//end method getType

	@Override
	public void setType(String newValue) {
		this.type = newValue;
	}//end method setType
    
	public void printDescription()
	{
		super.printDescription();
		System.out.println("This mountain bike is a " + this.type + " bike and has a " + this.suspension + " suspension and a frame size of " + this.frameSize + "inches.");
		
	}//end method printDescription
}//end class MountainBike
```

**Пункт 6. Класс `RoadBike` реализует `RoadParts`.** В объявление класса добавлено `implements RoadParts`, реализованы четыре метода интерфейса. Поля `tyreWidth` и `postHeight` имеют тип `int`, а сеттеры по условию принимают строковый аргумент `newValue`, поэтому внутри сеттеров выполняется преобразование строки в число: `Integer.parseInt(newValue.trim())`. Геттеры возвращают текущие значения полей.

**Файл `src/bikeproject/RoadBike.java`:**

```java
package bikeproject;

public class RoadBike extends Bike implements RoadParts{
	
	private int  tyreWidth, postHeight;
	
	public RoadBike()
	{
		this("drop", "racing", "tread less", "razor", 19, 20, 22);
	}//end constructor
	
	public RoadBike(int postHeight)
	{
		this("drop", "racing", "tread less", "razor", 19, 20, postHeight);
	}//end constructor
		
	public RoadBike(String handleBars, String frame, String tyres, String seatType, int numGears,
			int tyreWidth, int postHeight) {
		super(handleBars, frame, tyres, seatType, numGears);
		this.tyreWidth = tyreWidth;
		this.postHeight = postHeight;
	}//end constructor

	@Override
	public int getTyreWidth() {
		return this.tyreWidth;
	}//end method getTyreWidth

	@Override
	public void setTyreWidth(String newValue) {
		this.tyreWidth = Integer.parseInt(newValue.trim());
	}//end method setTyreWidth

	@Override
	public int getPostHeight() {
		return this.postHeight;
	}//end method getPostHeight

	@Override
	public void setPostHeight(String newValue) {
		this.postHeight = Integer.parseInt(newValue.trim());
	}//end method setPostHeight

	public void printDescription()
	{
		super.printDescription();
		System.out.println("This Roadbike bike has " + this.tyreWidth + "mm tyres and a post height of " + this.postHeight + ".");
	}//end method printDescription
}//end class RoadBike
```

### 6.5 Изменение драйвера (пункты 8–9)

В нижней части метода `main` после вывода всех четырёх велосипедов добавлено:

- **пункт 8** — изменение высоты подседельного штыря `bike1` с 22 на 20: `bike1.setPostHeight("20");` (значение передаётся строкой, как требует интерфейс `RoadParts`);
- **пункт 9** — вывод значений `bike1` (`getMake()`, `getTyreWidth()`, `getPostHeight()`) и его описания для подтверждения изменения.

**Файл `src/bikeproject/BikeDriver.java`:**

```java
package bikeproject;

public class BikeDriver {

	public static void main(String[] args) {
		
		RoadBike bike1 = new RoadBike();
		RoadBike bike2 = new RoadBike("drop", "tourer", "semi-grip", "comfort", 14, 25, 18);		
		MountainBike bike3 = new MountainBike();
		Bike bike4 = new Bike();
		
		bike1.printDescription();
		bike2.printDescription();
		bike3.printDescription();
		bike4.printDescription();

		// Пункт 8: для bike1 высота подседельного штыря изменяется с 22 на 20
		bike1.setPostHeight("20");

		// Пункт 9: вывод значений bike1 для подтверждения изменения
		System.out.println("\nbike1 values after change:");
		System.out.println("make: " + bike1.getMake());
		System.out.println("tyreWidth: " + bike1.getTyreWidth());
		System.out.println("postHeight: " + bike1.getPostHeight());
		bike1.printDescription();
	}//end method main

}//end class BikeDriver
```

### 6.6 Компиляция и запуск (пункты 7 и 10)

```sh
cd java/pr_3/uploaded
javac -encoding UTF-8 -d bin src/bikeproject/*.java
java -cp bin bikeproject.BikeDriver
```

---

## 7. Результат работы программы (Часть 2)

### 7.1 Проверка совместимости (пункт 7)

После подключения интерфейсов программа запускается и первые четыре вызова `printDescription()` дают **точно такой же** вывод, что и исходная версия: содержимое полей не изменилось, а константа `MAKE` интерфейса `BikeParts` хранит ту же строку `"Oracle Cycles"`, которая раньше была записана в конструкторах литералом. Построчное сравнение вывода исходной и модернизированной версий (`diff`) различий не выявило.

Вывод, который дают первые четыре вызова `printDescription()` (он полностью совпадает с выводом исходной версии программы):

```text

Oracle Cycles
This bike has drop handlebars on a racing frame with 19 gears.
It has a razor seat with tread less tyres.
This Roadbike bike has 20mm tyres and a post height of 22.

Oracle Cycles
This bike has drop handlebars on a tourer frame with 14 gears.
It has a comfort seat with semi-grip tyres.
This Roadbike bike has 25mm tyres and a post height of 18.

Oracle Cycles
This bike has Bull Horn handlebars on a Hardtail frame with 27 gears.
It has a dropper seat with Maxxis tyres.
This mountain bike is a Pro bike and has a RockShox XC32 suspension and a frame size of 19inches.

Oracle Cycles
This bike has null handlebars on a null frame with 0 gears.
It has a null seat with null tyres.
```

### 7.2 Итоговый вывод (пункты 8–10)

После выполнения пункта 8 высота подседельного штыря `bike1` равна 20 вместо 22, а пункт 9 выводит значения `bike1` для подтверждения изменения:

```text

Oracle Cycles
This bike has drop handlebars on a racing frame with 19 gears.
It has a razor seat with tread less tyres.
This Roadbike bike has 20mm tyres and a post height of 22.

Oracle Cycles
This bike has drop handlebars on a tourer frame with 14 gears.
It has a comfort seat with semi-grip tyres.
This Roadbike bike has 25mm tyres and a post height of 18.

Oracle Cycles
This bike has Bull Horn handlebars on a Hardtail frame with 27 gears.
It has a dropper seat with Maxxis tyres.
This mountain bike is a Pro bike and has a RockShox XC32 suspension and a frame size of 19inches.

Oracle Cycles
This bike has null handlebars on a null frame with 0 gears.
It has a null seat with null tyres.

bike1 values after change:
make: Oracle Cycles
tyreWidth: 20
postHeight: 20

Oracle Cycles
This bike has drop handlebars on a racing frame with 19 gears.
It has a razor seat with tread less tyres.
This Roadbike bike has 20mm tyres and a post height of 20.
```

Сравнение с первоначальным выводом: у `bike1` изменилась только высота штыря (`post height of 22` → `post height of 20`), описание остальных велосипедов (`bike2`, `bike3`, `bike4`) не изменилось.

| Пункт задания | Что сделано | Где видно в выводе |
|---|---|---|
| 7 | Программа после подключения интерфейсов работает так же, как раньше | первые четыре блока вывода совпадают с исходной версией |
| 8 | Высота штыря `bike1` изменена на 20 вместо 22 | `postHeight: 20` и `post height of 20.` |
| 9 | Значения `bike1` выведены на экран | блок `bike1 values after change:` |
| 10 | Программа повторно запущена и протестирована | полный вывод в п. 7.2 |

> 📸 **[СКРИНШОТ №2 — Часть 2. Проект `bikeproject`]**  
> *Вставьте сюда скриншот запуска `bikeproject.BikeDriver` в терминале / Eclipse (вывод четырёх велосипедов, затем блок `bike1 values after change:` и повторное описание `bike1` с высотой штыря 20).*

---

## 8. Заключение

В ходе выполнения практической работы №3 были изучены и практически применены механизмы объектно-ориентированного программирования в Java:

1. **Организация программы по пакетам.** Классы предметной области (`Vehicle`, `Car`, `ElectricCar`) и интерфейс `ElectricVehicle` размещены в пакете `vehicles`, а тестовый класс `TestCar` — в пакете `app`. Структура каталогов соответствует именам пакетов, классы подключаются директивами `import`.
2. **Абстрактный класс.** Общие характеристики транспортного средства вынесены в абстрактный класс `Vehicle` с абстрактным методом `vehicleType()`. Создать экземпляр такого класса нельзя, а каждый конкретный подкласс обязан реализовать метод `vehicleType()`.
3. **Интерфейс.** Интерфейс `ElectricVehicle` задаёт контракт поведения электромобиля (`getBatteryCapacity()` и `setBatteryCapacity()`). Класс `ElectricCar` реализует этот интерфейс, предоставляя конкретную реализацию обоих методов и поле `batteryCapacity`.
4. **Наследование и инкапсуляция.** Построена иерархия `Vehicle -> Car -> ElectricCar`. Поля класса `Vehicle` скрыты модификатором `private` и доступны через геттеры и сеттеры, а поле `engineType` объявлено `protected`, что позволяет задавать тип двигателя в конструкторах наследников (`"Combustion"` для `Car` и `"Electric"` для `ElectricCar`).
5. **Полиморфизм.** В тестовом классе продемонстрирована работа с объектами через ссылку типа интерфейса `ElectricVehicle` и через массив ссылок базового типа `Vehicle[]`, что позволяет единообразно вызывать переопределённые методы `vehicleType()` и `toString()`.
6. **Интерфейсы как контракт поведения.** В Части 2 в проекте `bikeproject` созданы интерфейсы `BikeParts`, `MountainParts` и `RoadParts`. Классы `Bike`, `MountainBike` и `RoadBike` реализуют соответствующие интерфейсы (`implements`), предоставляя конкретную реализацию всех объявленных методов.
7. **Константы интерфейса.** Название компании-производителя вынесено в константу `MAKE` интерфейса `BikeParts`, а типы местности — в константы `TERRAIN` и `terrain` интерфейсов `MountainParts` и `RoadParts`. Поля интерфейса неявно являются `public static final`, поэтому эти значения изменить нельзя.
8. **Сохранение поведения при рефакторинге.** Подключение интерфейсов не изменило вывод программы: исходная и модернизированная версии `bikeproject` выводят одинаковые описания четырёх велосипедов. После этого высота подседельного штыря `bike1` изменена с 22 на 20 через метод интерфейса `setPostHeight`, и результат изменения подтверждён выводом значений `bike1` на экран.

Таким образом, все требования постановки задачи выполнены: программа организована по пакетам, содержит абстрактный класс и интерфейс, а тестовый класс демонстрирует создание объектов, изменение их свойств, получение ёмкости батареи и вывод описаний методом `toString()`.

В Части 2, в свою очередь, в проект `bikeproject` добавлены интерфейсы `BikeParts`, `MountainParts` и `RoadParts` с константами и набором методов, реализованными в классах `Bike`, `MountainBike` и `RoadBike`, а работоспособность программы проверена запуском. Таким образом, обе части практической работы №3 выполнены полностью.
