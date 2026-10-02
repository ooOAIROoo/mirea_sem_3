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
