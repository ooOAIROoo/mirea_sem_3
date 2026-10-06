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
        System.out.println("---------------");
    }
}
