package vehicles;

/**
 * Обычный автомобиль, наследующий абстрактный класс {@link Vehicle}.
 *
 * <p>В конструкторах тип двигателя задаётся как {@code "Combustion"} —
 * двигатель внутреннего сгорания.</p>
 */
public class Car extends Vehicle {
    /** Тип двигателя обычного автомобиля. */
    private static final String ENGINE_TYPE = "Combustion";

    public Car() {
        super();
        this.engineType = ENGINE_TYPE;
    }

    public Car(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber) {
        super(model, license, color, year, ownerName, insuranceNumber, ENGINE_TYPE);
    }

    /**
     * Возвращает тип транспортного средства.
     *
     * @return {@code "Car"}
     */
    @Override
    public String vehicleType() {
        return "Car";
    }
}
