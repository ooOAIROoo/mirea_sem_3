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
