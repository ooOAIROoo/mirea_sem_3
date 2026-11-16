public class Car {
    String model;
    String license;
    String color;
    int year;

    public Car (String model, String license, String color, int year){
        this.model = model;
        this.license = license;
        this.color = color;
        this.year = year;
    }

    // public Car (String model, String license, String color, int year){
    //     this.model = "BMW M5";
    //     this.license = "MACAN";
    //     this.color = "BLACK";
    //     this.year = 2019;
    // }

    // public Car (String model, String color, int year){
    //     this.model = "BUGATTI";
    //     this.license = "TAMAEV";
    //     this.year = year;
    // }

    public void To_String(){
        System.out.print(model + ' ' + license + ' ' + color + ' ' + year);
    }

    public void Year(){
        var Year = 25;
        System.out.print("Сейчас " + Year);
    }

    public String getModel(){
        return model;
    }

    public String getLicense(){
        return license;
    }

    public String getColor(){
        return color;
    }

    public int getYear(){
        return year;
    }

    public String setModel(){
        return this.model = model;
    }

    public String setLicense(){
        return this.license = license;
    }

    public String setColor(){
        return this.color = color;
    }

    public int setYear(int year){
        return this.year = year;
    }
}