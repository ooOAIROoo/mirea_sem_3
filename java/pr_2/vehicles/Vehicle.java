package vehicles;

/**
 * Базовый абстрактный класс с общими характеристиками транспортного средства.
 */
public abstract class Vehicle {
    private String model;
    private String license;
    private String color;
    private int year;
    private String ownerName;
    private String insuranceNumber;
    protected String engineType;

    protected Vehicle() {
        this("", "", "", 0, "", "", "Unknown");
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

    /** Возвращает конкретный тип транспортного средства. */
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

    /** Дополнительные характеристики, которые добавляют подклассы. */
    protected String additionalDetails() {
        return "";
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder(vehicleType())
                .append("{model='").append(model).append('\'')
                .append(", license='").append(license).append('\'')
                .append(", color='").append(color).append('\'')
                .append(", year=").append(year)
                .append(", ownerName='").append(ownerName).append('\'')
                .append(", insuranceNumber='").append(insuranceNumber).append('\'')
                .append(", engineType='").append(engineType).append('\'');

        String extra = additionalDetails();
        if (!extra.isEmpty()) {
            result.append(", ").append(extra);
        }
        return result.append('}').toString();
    }
}
