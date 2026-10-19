package vehicles;

/** Обычный автомобиль, наследующий абстрактный класс Vehicle. */
public class Car extends Vehicle {
    private static final String DEFAULT_ENGINE_TYPE = "Gasoline";

    public Car() {
        super();
        this.engineType = DEFAULT_ENGINE_TYPE;
    }

    public Car(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber) {
        // Использование методов (сеттеров) и защищённого поля родительского класса Vehicle
        setModel(model);
        setLicense(license);
        setColor(color);
        setYear(year);
        setOwnerName(ownerName);
        setInsuranceNumber(insuranceNumber);
        this.engineType = DEFAULT_ENGINE_TYPE;
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
        this.engineType = engineType;
    }

    @Override
    public String vehicleType() {
        return "Car";
    }
}
