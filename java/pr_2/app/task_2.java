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
