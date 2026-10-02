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
