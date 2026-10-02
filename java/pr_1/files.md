```java
import java.util.Scanner;

public class task_1 {
    public static void main(String[] args) {
        int yuan;
        final double rub_per_yuan = 11.91;

        Scanner input = new Scanner(System.in);

        System.out.println("Введите количество юаней: ");
        yuan = input.nextInt();

        int roubles = (int) Math.ceil(rub_per_yuan * yuan);

        System.out.println("Перевод ");
        System.out.println(roubles);
    }
}
```

```java
import java.util.Scanner;

public class task_2 {
    public static void main(String[] args) {
        final double roubles_per_yuan = 11.91;
        int yuan, digit;
        double roubles;

        Scanner input = new Scanner(System.in);

        System.out.println("Введите количество юаней: ");
        yuan = input.nextInt();

        roubles = roubles_per_yuan * yuan;

        digit = yuan % 10;

        if (digit == 1) {
            System.out.println("Юань");
        }
        
        else if (digit == 2 || digit == 3 || digit == 4) {
            System.out.println("Юаня");
        }

        else {
            System.out.println("Юаней");
        }   
    }
}
```

```java
public class task_3 {
	public static void main(String[] args) {
        Car_t3 car1 = new Car_t3();
        car1.printInfo();
        
        Car_t3 car2 = new Car_t3("Лада", "Е0932КА790", "Белый", 2010);
        car2.printInfo();
        
        Car_t3 car3 = new Car_t3("Ford Focus", "О324ОО777");
        car3.printInfo();
    }
}
```

```java
public class Car_t3 {
    String model;
    String license;
    String color;
    int year;
    
    public Car_t3() //1 Конструктор по умолчанию
    {
        this.model = "Неизвестно";
        this.license = "Неизвестно";
        this.color = "Неизвестно";
        this.year = 0;
    }

    public Car_t3(String model, String license, String color, int year) //2 Конструктор, включает все поля
    {
        this.model = model;
        this.license = license;
        this.color = color;
        this.year = year;
    }

    public Car_t3(String model, String license){ //3 Конструктор по выбору
        this.model = model;
        this.license = license;
        this.color = "Зелёный";
        this.year = 2015;
    }

    public void printInfo() {
        System.out.println("Модель: " + model);
        System.out.println("Номер: " + license);
        System.out.println("Цвет: " + color);
        System.out.println("Год выпуска: " + year);
        System.out.println("-------------------------");
    }
}
```

```java
public class task_4 {
    public static void main(String[] args) {
        // 1) объект через конструктор по умолчанию
        Car_t4 car1 = new Car_t4();
        car1.To_String();
        System.out.println("возраст: " + car1.getAge() + " лет");
        System.out.println();
        
        // 2) объект через конструктор со всеми полями
        Car_t4 car2 = new Car_t4("лада", "е112ра8777", "жёлтый", 2010);
        car2.To_String();
        System.out.println("возраст: " + car2.getAge() + " лет");
        System.out.println();
        
        // 3) объект через конструктор по выбору (model + license)
        Car_t4 car3 = new Car_t4("gelentvagen", "о111оо777");
        car3.To_String();
        System.out.println("возраст: " + car3.getAge() + " лет");
        System.out.println();

        car3.setColor("красный");
        car3.setYear(2018);
        
        System.out.println("после изменения через сеттеры:");
        car3.To_String();
        System.out.println("возраст: " + car3.getAge() + " лет");
    }
}
```

```java
public class Car_t4 {
    public static final int current_year = 2026;
    String model;
    String license;
    String color;
    
    int year;

    public Car_t4() //1 Конструктор по умолчанию
    {
        this.model = "Неизвестно";
        this.license = "Неизвестно";
        this.color = "Неизвестно";
        this.year = 2026;
    }
    
    public Car_t4(String model, String license, String color, int year) //2 Конструктор, включает все поля
    {
        this.model = model;
        this.license = license;
        this.color = color;
        this.year = year;
    }
    
    public Car_t4(String model, String license){ //3 Конструктор по выбору
        this.model = model;
        this.license = license;
        this.color = "Зелёный";
        this.year = 2015;
    }
    
    public void To_String() {
        System.out.println("Модель: " + model);
        System.out.println("Номер: " + license);
        System.out.println("Цвет: " + color);
        System.out.println("Год выпуска: " + year);
        System.out.println("-------------------------");
    }
    
    public String getModel() {return model;}
    
    public void setModel(String model) {this.model = model;}
    
    public String getLicense() {return license;}
    
    public void setLicense(String license) {this.license = license;}
    
    public String getColor() {return color;}
    
    public void setColor(String color) {this.color = color;}
    
    public int getYear() {return year;}
    
    public void setYear(int year) {this.year = year;}
    
    public int getAge() {return current_year - year;}
}
```
