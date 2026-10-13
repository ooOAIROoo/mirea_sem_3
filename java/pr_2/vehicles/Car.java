package vehicles;

/** Обычный автомобиль. */
public class Car extends Vehicle {
    private static final String DEFAULT_ENGINE_TYPE = "Gasoline";

    public Car() {
        this("", "", "", 0, "", "", DEFAULT_ENGINE_TYPE);
    }

    public Car(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber) {
        this(model, license, color, year, ownerName, insuranceNumber, DEFAULT_ENGINE_TYPE);
    }

    public Car(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber,
            String engineType) {
        super(model, license, color, year, ownerName, insuranceNumber, engineType);
    }

    @Override
    public String vehicleType() {
        return "Car";
    }
}
