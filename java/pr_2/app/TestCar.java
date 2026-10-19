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
