---
lang: ru-RU
toc-title: СОДЕРЖАНИЕ
---

::: {custom-style="StructuralHeading"}
ВВЕДЕНИЕ
:::

Настоящий отчёт посвящён выполнению практической работы № 3 по дисциплине «Программирование на языке Java» на тему «Объекты и классы: абстрактные классы и интерфейсы, пакеты».

Цель работы — изучить организацию программы по пакетам, применение абстрактных классов и интерфейсов, наследование и полиморфизм.

Для достижения цели решаются следующие задачи:

- разработать иерархию транспортных средств и интерфейс электромобиля;
- проверить создание объектов, изменение их свойств и полиморфный вывод;
- изучить устройство проекта велосипедов `bikeproject` и ответить на вопросы задания;
- добавить интерфейсы характеристик велосипедов и реализовать их методы;
- изменить высоту подседельного штыря `bike1` с 22 на 20 и подтвердить изменение выводом.

# 1 Теоретические сведения

**Пакеты (`package`).** Пакет — это механизм группировки логически связанных классов и интерфейсов. Пакет задаётся директивой `package` в первой строке исходного файла, а его имя соответствует структуре каталогов: класс из пакета `vehicles` размещается в каталоге `vehicles`. Классы других пакетов подключаются директивой `import`. Использование пакетов позволяет избежать конфликтов имён и управлять видимостью классов.

**Абстрактные классы.** Абстрактный класс объявляется с модификатором `abstract`, может содержать как обычные, так и абстрактные методы (объявленные без тела). Создать экземпляр абстрактного класса нельзя. Класс-наследник обязан реализовать все унаследованные абстрактные методы либо сам должен быть объявлен абстрактным. При этом абстрактный класс может содержать поля, конструкторы и готовую реализацию части методов, которые наследуются подклассами.

**Интерфейсы.** Интерфейс (`interface`) описывает контракт поведения, которому должен соответствовать класс. Методы интерфейса неявно являются `public abstract`, а поля — `public static final` (константами). Класс реализует интерфейс с помощью ключевого слова `implements` и обязан реализовать все методы интерфейса; при этом один класс может реализовывать несколько интерфейсов. Начиная с Java 8 интерфейсы могут также содержать методы с реализацией (`default` и `static`).

**Сравнение абстрактного класса и интерфейса:**

Table: Таблица 1 — Сравнение абстрактного класса и интерфейса

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

# 2 Постановка задачи и структура проекта

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

## 2.1 Файлы проекта

Исходный код первой части расположен в пакетах `vehicles` и `app`; вторая часть находится в `uploaded/src/bikeproject`. Соответствие файлов их назначению представлено в таблице 2.

Table: Таблица 2 — Файлы практической работы № 3

| Файл | Назначение |
|---|---|
| `vehicles/Vehicle.java` | Абстрактный класс транспортного средства |
| `vehicles/ElectricVehicle.java` | Интерфейс электромобиля |
| `vehicles/Car.java` | Автомобиль с двигателем внутреннего сгорания |
| `vehicles/ElectricCar.java` | Электромобиль, реализующий ElectricVehicle |
| `app/TestCar.java` | Проверка транспортных средств |
| `uploaded/src/bikeproject/BikeParts.java` | Общий интерфейс велосипеда |
| `uploaded/src/bikeproject/MountainParts.java` | Интерфейс горного велосипеда |
| `uploaded/src/bikeproject/RoadParts.java` | Интерфейс шоссейного велосипеда |
| `uploaded/src/bikeproject/Bike.java` | Базовый класс велосипеда |
| `uploaded/src/bikeproject/MountainBike.java` | Горный велосипед |
| `uploaded/src/bikeproject/RoadBike.java` | Шоссейный велосипед |
| `uploaded/src/bikeproject/BikeDriver.java` | Проверка велосипедов и изменения bike1 |

Отчёт расположен в `report.md`, изображения результатов — в `screenshots/`, общий шаблон Word — в `../gost-reference.docx`.

## 2.2 Компиляция

Команды выполняются из каталога `java/pr_3`. Результаты сборки вынесены за пределы исходных файлов.

::: {custom-style="ListingCaption"}
Листинг 1 — Команды компиляции проекта
:::

```sh
javac -encoding UTF-8 -d /tmp/pr3-classes vehicles/*.java app/*.java
javac -encoding UTF-8 -d /tmp/pr3-bike-classes uploaded/src/bikeproject/*.java
```

# 3 Ход выполнения работы


## 3.1 Абстрактный класс транспортного средства

Общие поля инкапсулированы в `Vehicle`: доступ к ним осуществляется через геттеры и сеттеры. Поле `engineType` защищённое, чтобы наследники могли задавать тип двигателя. Метод `vehicleType()` абстрактный. `commonDescription()` формирует общую часть описания, используемую в `toString()`.

**Файл `vehicles/Vehicle.java`:**

::: {custom-style="ListingCaption"}
Листинг 2 — Исходный код файла vehicles/Vehicle.java
:::

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


## 3.2 Интерфейс электромобиля

Интерфейс определяет получение и изменение ёмкости батареи. Ссылка типа `ElectricVehicle` позволяет обращаться к этим операциям независимо от реализации электромобиля.

**Файл `vehicles/ElectricVehicle.java`:**

::: {custom-style="ListingCaption"}
Листинг 3 — Исходный код файла vehicles/ElectricVehicle.java
:::

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


## 3.3 Автомобиль с двигателем внутреннего сгорания

Класс `Car` наследует `Vehicle`. Конструкторы задают тип двигателя `Combustion`, а `vehicleType()` возвращает `Car`.

**Файл `vehicles/Car.java`:**

::: {custom-style="ListingCaption"}
Листинг 4 — Исходный код файла vehicles/Car.java
:::

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


## 3.4 Электромобиль, реализующий ElectricVehicle

Класс `ElectricCar` наследует `Car` и реализует `ElectricVehicle`. Конструктор заменяет тип двигателя на `Electric`. Поле `batteryCapacity` хранит ёмкость батареи; переопределённый `toString()` дополняет описание этим значением.

**Файл `vehicles/ElectricCar.java`:**

::: {custom-style="ListingCaption"}
Листинг 5 — Исходный код файла vehicles/ElectricCar.java
:::

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


## 3.5 Проверка транспортных средств

Тест создаёт автомобиль и электромобиль, изменяет год, владельца и страховку. Ёмкость батареи считывается и изменяется через интерфейс. Массив `Vehicle[]` демонстрирует динамическое связывание методов `vehicleType()` и `toString()`.

**Файл `app/TestCar.java`:**

::: {custom-style="ListingCaption"}
Листинг 6 — Исходный код файла app/TestCar.java
:::

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


## 3.6 Общий интерфейс велосипеда

Константа `MAKE` хранит название производителя `Oracle Cycles`; методы `getMake()` и `printDescription()` задают общий контракт.

**Файл `uploaded/src/bikeproject/BikeParts.java`:**

::: {custom-style="ListingCaption"}
Листинг 7 — Исходный код файла uploaded/src/bikeproject/BikeParts.java
:::

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


## 3.7 Интерфейс горного велосипеда

Константа `TERRAIN` равна `off_road`. Методы интерфейса читают и изменяют подвеску и тип горного велосипеда.

**Файл `uploaded/src/bikeproject/MountainParts.java`:**

::: {custom-style="ListingCaption"}
Листинг 8 — Исходный код файла uploaded/src/bikeproject/MountainParts.java
:::

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


## 3.8 Интерфейс шоссейного велосипеда

Константа `terrain` равна `track_racing`. Геттеры возвращают числовые характеристики, а сеттеры принимают строки согласно заданию.

**Файл `uploaded/src/bikeproject/RoadParts.java`:**

::: {custom-style="ListingCaption"}
Листинг 9 — Исходный код файла uploaded/src/bikeproject/RoadParts.java
:::

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


## 3.9 Базовый класс велосипеда

Класс реализует `BikeParts`, хранит общие характеристики и выводит их методом `printDescription()`. В конструкторе без аргументов строковые характеристики остаются `null`, число передач — 0; это поведение существующего проекта сохранено.

**Файл `uploaded/src/bikeproject/Bike.java`:**

::: {custom-style="ListingCaption"}
Листинг 10 — Исходный код файла uploaded/src/bikeproject/Bike.java
:::

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


## 3.10 Горный велосипед

Класс наследует `Bike` и реализует `MountainParts`. Конструктор без аргументов делегирует параметризованному конструктору, а вывод дополняет описание базового класса сведениями о подвеске, типе и размере рамы.

**Файл `uploaded/src/bikeproject/MountainBike.java`:**

::: {custom-style="ListingCaption"}
Листинг 11 — Исходный код файла uploaded/src/bikeproject/MountainBike.java
:::

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


## 3.11 Шоссейный велосипед

Класс наследует `Bike` и реализует `RoadParts`. Методы `setTyreWidth()` и `setPostHeight()` преобразуют строку в `int` с помощью `Integer.parseInt(newValue.trim())`. Некорректная строка приводит к `NumberFormatException`.

**Файл `uploaded/src/bikeproject/RoadBike.java`:**

::: {custom-style="ListingCaption"}
Листинг 12 — Исходный код файла uploaded/src/bikeproject/RoadBike.java
:::

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


## 3.12 Проверка велосипедов и изменения bike1

Драйвер создаёт четыре велосипеда и выводит исходные характеристики. Затем вызов `bike1.setPostHeight("20")` меняет высоту штыря с 22 на 20; повторный вывод подтверждает изменение.

**Файл `uploaded/src/bikeproject/BikeDriver.java`:**

::: {custom-style="ListingCaption"}
Листинг 13 — Исходный код файла uploaded/src/bikeproject/BikeDriver.java
:::

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


# 4 Результаты выполнения

## 4.1 Транспортные средства

::: {custom-style="ListingCaption"}
Листинг 14 — Команда запуска: Транспортные средства
:::

```sh
java -cp /tmp/pr3-classes app.TestCar
```

::: {custom-style="ListingCaption"}
Листинг 15 — Консольный вывод: Транспортные средства
:::

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

Проверены создание объектов, изменение года выпуска, владельца и страховки, изменение ёмкости батареи с 75 до 82 кВт·ч и полиморфный вывод. Скриншот результата приводится на рисунке 1.

::: {custom-style="Captioned Figure"}
![](<screenshots/20261124_17h39m21s-th.png>){width=15.5cm}
:::

::: {custom-style="Image Caption"}
Рисунок 1 — Результат работы программы app.TestCar
:::

## 4.2 Велосипеды

::: {custom-style="ListingCaption"}
Листинг 16 — Команда запуска: Велосипеды
:::

```sh
java -cp /tmp/pr3-bike-classes bikeproject.BikeDriver
```

::: {custom-style="ListingCaption"}
Листинг 17 — Консольный вывод: Велосипеды
:::

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

Первые четыре описания показывают характеристики созданных велосипедов. В конце вывода `postHeight: 20` и `post height of 20.` подтверждают изменение высоты штыря `bike1`; остальные велосипеды не изменяются. Скриншот результата приводится на рисунке 2.

::: {custom-style="Captioned Figure"}
![](<screenshots/20261124_17h40m56s-th.png>){width=15.5cm}
:::

::: {custom-style="Image Caption"}
Рисунок 2 — Результат работы программы bikeproject.BikeDriver
:::

# 5 Ответы на контрольные вопросы

**1. Пример примитивного типа данных для хранения полей класса.**\
Примитивный тип `int` используется для полей `NumGears` в классе `Bike`, `tyreWidth` и `postHeight` в классе `RoadBike`, `frameSize` в классе `MountainBike`. Поля типа `String` (`handleBars`, `frame`, `tyres`, `seatType`, `make`, `suspension`, `type`) примитивами не являются — это ссылочный тип.

**2. Пример конкатенации строк.**\
Конкатенация выполняется в методе `printDescription()` каждого класса при помощи оператора `+`:

::: {custom-style="ListingCaption"}
Листинг 18 — Пример конкатенации строк
:::

```java
System.out.println("\n" + this.make + "\n"
        + "This bike has " + this.handleBars + " handlebars on a "
        + this.frame + " frame with " + this.NumGears + " gears."
        + "\nIt has a " + this.seatType + " seat with " + this.tyres + " tyres.");
```

При конкатенации строки с числом (`this.NumGears`, `this.tyreWidth`, `this.postHeight`, `this.frameSize`) число автоматически преобразуется в строку, например: `"This Roadbike bike has " + this.tyreWidth + "mm tyres"`.

**3. Имена объектов, созданных в программе.**

Table: Таблица 3 — Объекты, созданные в BikeDriver

| Переменная | Класс | Строка в `BikeDriver` |
|---|---|---|
| `bike1` | `RoadBike` | `RoadBike bike1 = new RoadBike();` |
| `bike2` | `RoadBike` | `RoadBike bike2 = new RoadBike("drop", "tourer", "semi-grip", "comfort", 14, 25, 18);` |
| `bike3` | `MountainBike` | `MountainBike bike3 = new MountainBike();` |
| `bike4` | `Bike` | `Bike bike4 = new Bike();` |

**4. Количество конструкторов у каждого класса.**

Table: Таблица 4 — Конструкторы классов проекта bikeproject

| Класс | Количество конструкторов | Перечень |
|---|---:|---|
| `Bike` | 2 | `Bike()`, `Bike(String handleBars, String frame, String tyres, String seatType, int numGears)` |
| `RoadBike` | 3 | `RoadBike()`, `RoadBike(int postHeight)`, `RoadBike(String, String, String, String, int, int tyreWidth, int postHeight)` |
| `MountainBike` | 2 | `MountainBike()`, `MountainBike(String, String, String, String, int, String suspension, String type, int frameSize)` |
| `BikeDriver` | 0 (явных) | используется неявный конструктор по умолчанию |

**5. Наследование: суперкласс и подклассы.**\
Суперкласс (родительский класс) — `Bike`: в нём объявлены общие для всех велосипедов поля (`handleBars`, `frame`, `tyres`, `seatType`, `NumGears`, `make`) и метод `printDescription()`.\
Подклассы — `RoadBike` (`class RoadBike extends Bike`) и `MountainBike` (`class MountainBike extends Bike`): они наследуют поля и методы `Bike`, добавляют собственные поля и переопределяют `printDescription()`, дополняя описание родителя через `super.printDescription()`.\
Класс `BikeDriver` в наследовании не участвует — он только создаёт объекты и вызывает их методы.

**6. Примеры значений для стандартного велосипеда каждого типа.**

Table: Таблица 5 — Характеристики стандартных и заказного велосипедов

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

::: {custom-style="StructuralHeading"}
ЗАКЛЮЧЕНИЕ
:::

В ходе работы изучены пакеты Java, абстрактные классы, интерфейсы, наследование и полиморфизм. Разработана иерархия `Vehicle → Car → ElectricCar`; интерфейс `ElectricVehicle` выделяет операции работы с батареей. Тестовый класс демонстрирует создание объектов, изменение их свойств и вызов переопределённых методов через ссылки базового типа.

В проекте `bikeproject` реализованы интерфейсы `BikeParts`, `MountainParts` и `RoadParts`. Общие и специализированные характеристики распределены между базовым классом и наследниками. Изменение высоты подседельного штыря `bike1` с 22 на 20 подтверждено консольным выводом. Обе части задания выполнены.
