package vehicles;

/**
 * Часть 1. Задача #1 и #2.
 * Класс автомобиля с инкапсулированными полями ownerName, insuranceNumber
 * и защищённым полем engineType.
 */
public class Car_t1 {
    private String ownerName;
    private String insuranceNumber;
    protected String engineType;

    public Car_t1() {
        this.ownerName = "Неизвестно";
        this.insuranceNumber = "Неизвестно";
        this.engineType = "Бензиновый";
    }

    public Car_t1(String ownerName, String insuranceNumber, String engineType) {
        this.ownerName = ownerName;
        this.insuranceNumber = insuranceNumber;
        this.engineType = engineType;
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

    public void printInfo() {
        System.out.println("Владелец (private ownerName): " + ownerName);
        System.out.println("Страховой номер (private insuranceNumber): " + insuranceNumber);
        System.out.println("Тип двигателя (protected engineType): " + engineType);
        System.out.println("------------------------------------------");
    }
}
