package vehicles;

/**
 * Электромобиль: наследует класс {@link Car} и реализует интерфейс
 * {@link ElectricVehicle}.
 *
 * <p>Дополнительно хранит ёмкость батареи, а в конструкторах задаёт
 * тип двигателя {@code "Electric"}.</p>
 */
public class ElectricCar extends Car implements ElectricVehicle {
    /** Ёмкость батареи в киловатт-часах. */
    private int batteryCapacity;

    public ElectricCar() {
        super();
        this.engineType = "Electric";
    }

    public ElectricCar(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber,
            int batteryCapacity) {
        super(model, license, color, year, ownerName, insuranceNumber);
        this.engineType = "Electric";
        this.batteryCapacity = batteryCapacity;
    }

    @Override
    public int getBatteryCapacity() {
        return batteryCapacity;
    }

    @Override
    public void setBatteryCapacity(int batteryCapacity) {
        this.batteryCapacity = batteryCapacity;
    }

    /**
     * Возвращает тип транспортного средства.
     *
     * @return {@code "Electric Car"}
     */
    @Override
    public String vehicleType() {
        return "Electric Car";
    }

    /**
     * Возвращает строку с описанием электромобиля, дополненную ёмкостью батареи.
     */
    @Override
    public String toString() {
        return commonDescription() + ", batteryCapacity=" + batteryCapacity + " kWh}";
    }
}
