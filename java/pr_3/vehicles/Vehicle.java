package vehicles;

/**
 * Абстрактный базовый класс с общими характеристиками транспортного средства.
 *
 * <p>Содержит поля {@code model}, {@code license}, {@code color}, {@code year},
 * {@code ownerName}, {@code insuranceNumber}, {@code engineType}, геттеры и сеттеры
 * для каждого поля, а также абстрактный метод {@link #vehicleType()}.</p>
 */
public abstract class Vehicle {
    private String model;
    private String license;
    private String color;
    private int year;
    private String ownerName;
    private String insuranceNumber;
    /** Тип двигателя: доступен классам-наследникам напрямую. */
    protected String engineType;

    protected Vehicle() {
        this("Неизвестно", "Неизвестно", "Неизвестно", 0, "Неизвестно", "Неизвестно", "Неизвестно");
    }

    protected Vehicle(
            String model,
            String license,
            String color,
            int year,
            String ownerName,
            String insuranceNumber,
            String engineType) {
        this.model = model;
        this.license = license;
        this.color = color;
        this.year = year;
        this.ownerName = ownerName;
        this.insuranceNumber = insuranceNumber;
        this.engineType = engineType;
    }

    /**
     * Возвращает тип транспортного средства.
     *
     * @return строковое обозначение типа, например {@code "Car"} или {@code "Electric Car"}
     */
    public abstract String vehicleType();

    public String getModel() {
        return model;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public String getLicense() {
        return license;
    }

    public void setLicense(String license) {
        this.license = license;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }

    public String getOwnerName() {
        return ownerName;
    }

    public void setOwnerName(String ownerName) {
        this.ownerName = ownerName;
    }

    public String getInsuranceNumber() {
        return insuranceNumber;
    }

    public void setInsuranceNumber(String insuranceNumber) {
        this.insuranceNumber = insuranceNumber;
    }

    public String getEngineType() {
        return engineType;
    }

    public void setEngineType(String engineType) {
        this.engineType = engineType;
    }

    /**
     * Формирует общую часть описания транспортного средства без закрывающей скобки.
     * Подклассы дополняют описание собственными полями.
     */
    protected String commonDescription() {
        StringBuilder result = new StringBuilder(vehicleType())
                .append("{model='").append(model).append('\'')
                .append(", license='").append(license).append('\'')
                .append(", color='").append(color).append('\'')
                .append(", year=").append(year)
                .append(", ownerName='").append(ownerName).append('\'')
                .append(", insuranceNumber='").append(insuranceNumber).append('\'')
                .append(", engineType='").append(engineType).append('\'');
        return result.toString();
    }

    /**
     * Возвращает строку с описанием транспортного средства.
     */
    @Override
    public String toString() {
        return commonDescription() + "}";
    }
}
