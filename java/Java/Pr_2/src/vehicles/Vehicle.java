package vehicles;

public abstract class Vehicle {
    private String model, license, color, ownerName;
    protected String engineType;
    private int year, insuranceNumber;

    public Vehicle(String model, String license, String color, String ownerName, String engineType, int year, int insuranceNumber){
        this.model = model;
        this.license = license;
        this.color = color;
        this.ownerName = ownerName;
        this.engineType = engineType;
        this.year = year;
        this.insuranceNumber = insuranceNumber;
    }

    public abstract void vehicleType();

    public String set_model(String model){
        return this.model = model;
    }

    public String get_model(){
        return model;
    }

    public String set_license(String license){
        return this.license = license;
    }

    public String get_license(){
        return license;
    }

    public String set_color(String color){
        return this.color = color;
    }

    public String get_color(){
        return color;
    }

    public String set_ownerName(String ownerName){
        return this.ownerName = ownerName;
    }

    public String get_ownerName(){
        return ownerName;
    }

    public int set_insuranceNumber(int insuranceNumber){
        return this.insuranceNumber = insuranceNumber;
    }

    public int get_insuranceNumber(){
        return insuranceNumber;
    }

    public String set_engineType(String engineType){
        return this.engineType = engineType;
    }

    public String get_engineType(){
        return engineType;
    }

    private void To_String(){
        System.out.print(model + license + color + year + ownerName + insuranceNumber + engineType);
    }
}