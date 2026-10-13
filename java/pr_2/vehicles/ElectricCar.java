package vehicles;

/** Электромобиль, наследующий характеристики обычного автомобиля. */
public class ElectricCar extends Car {
    private int batteryCapacity;

    public ElectricCar() {
        this("", "", "", 0, "", "", 0);
    }

    public ElectricCar(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber,
            int batteryCapacity) {
        super(model, license, color, year, ownerName, insuranceNumber, "Electric");
        // engineType защищён в Vehicle и доступен наследникам.
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
    public String vehicleType() {
        return "Electric Car";
    }

    @Override
    protected String additionalDetails() {
        return "batteryCapacity=" + batteryCapacity + " kWh";
    }
}
