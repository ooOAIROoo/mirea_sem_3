package vehicles;

public class Car extends Vehicle {
    public Car(String model, String license, String color, String ownerName, String engineType, int year, int insuranceNumber){
        super(model, license, color, ownerName, engineType = "Combustion", year, insuranceNumber);
    }

    public void vehicleType(){
        System.out.print("Car");
    }

}