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
