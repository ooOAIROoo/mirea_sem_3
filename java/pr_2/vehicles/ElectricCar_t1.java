package vehicles;

/**
 * Часть 1. Задача #2.
 * Класс электромобиля, наследующий Car_t1 и использующий protected-поле engineType.
 */
public class ElectricCar_t1 extends Car_t1 {
    private int batteryCapacity;

    public ElectricCar_t1() {
        super();
        this.engineType = "Electric";
        this.batteryCapacity = 0;
    }

    public ElectricCar_t1(String ownerName, String insuranceNumber, int batteryCapacity) {
        super(ownerName, insuranceNumber, "Electric");
        // Прямой доступ к protected-полю родительского класса Car_t1
        this.engineType = "Electric";
        this.batteryCapacity = batteryCapacity;
    }

    public int getBatteryCapacity() {
        return batteryCapacity;
    }

    public void setBatteryCapacity(int batteryCapacity) {
        this.batteryCapacity = batteryCapacity;
    }

    @Override
    public void printInfo() {
        System.out.println("Владелец (через getOwnerName): " + getOwnerName());
        System.out.println("Страховой номер (через getInsuranceNumber): " + getInsuranceNumber());
        System.out.println("Тип двигателя (protected engineType): " + this.engineType);
        System.out.println("Ёмкость батареи (private batteryCapacity): " + batteryCapacity + " кВт·ч");
        System.out.println("------------------------------------------");
    }
}
