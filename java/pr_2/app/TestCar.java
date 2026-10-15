package app;

import vehicles.Car;
import vehicles.ElectricCar;
import vehicles.Vehicle;

/** Проверка инкапсуляции, наследования и полиморфизма. */
public class TestCar {
    public static void main(String[] args) {
        Car car = new Car(
                "Volvo XC60", "A123AA 77", "Black", 2022,
                "Ivan Ivanov", "INS-CAR-001", "Petrol");
        ElectricCar electricCar = new ElectricCar(
                "Tesla Model 3", "B456BB 77", "White", 2024,
                "Maria Smirnova", "INS-EV-002", 75);

        // Изменяем общие поля только через setters, даже когда объекты
        // используются по ссылкам на родительский класс Vehicle.
        Vehicle carAsVehicle = car;
        Vehicle electricCarAsVehicle = electricCar;

        updateCommonProperties(
                carAsVehicle, "Volvo XC60 Recharge", "A123AA 77", "Graphite",
                2023, "Ivan Petrov", "INS-CAR-101", "Hybrid");
        updateCommonProperties(
                electricCarAsVehicle, "Tesla Model 3 Long Range", "B456BB 77", "Red",
                2025, "Maria Smirnova", "INS-EV-202", electricCarAsVehicle.getEngineType());
        electricCar.setBatteryCapacity(82);

        Vehicle[] vehicles = {carAsVehicle, electricCarAsVehicle};
        for (Vehicle vehicle : vehicles) {
            // Вызов vehicleType() и toString() выбирается по реальному типу объекта.
            System.out.println(vehicle);
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
