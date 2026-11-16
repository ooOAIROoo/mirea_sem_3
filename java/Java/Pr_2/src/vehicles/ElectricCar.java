package vehicles;

public class ElectricCar extends Car {
    private int batteryCapacity;

    public ElectricCar(String model, String license, String color, String ownerName, String engineType, int year, int insuranceNumber){
        super(model, license, color, ownerName, engineType = "Electric", year, insuranceNumber);
        this.batteryCapacity = batteryCapacity;
    }

    public int set_batteryCapacity(int batteryCapacity){
        return this.batteryCapacity = batteryCapacity;
    }

    public int get_batteryCapacity(){
        return batteryCapacity;
    }

    public void vehicleType(){
        System.out.print("Electric Car");
    }

}