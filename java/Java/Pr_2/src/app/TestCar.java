package app;
import vehicles.Car;
import vehicles.ElectricCar;


public class TestCar{
    public static void main(String[] arg){
        Car Arsen = new Car("savva",124,"Diesel");
        ElectricCar Tesla = new ElectricCar(99);

        Arsen.set_ownerName("Markaryan");
        Tesla.set_batteryCapacity(112);
        Arsen.To_String();
        Tesla.To_String_electric();
    }
}