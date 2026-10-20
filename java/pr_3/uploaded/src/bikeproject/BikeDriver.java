package bikeproject;

public class BikeDriver {

	public static void main(String[] args) {
		
		RoadBike bike1 = new RoadBike();
		RoadBike bike2 = new RoadBike("drop", "tourer", "semi-grip", "comfort", 14, 25, 18);		
		MountainBike bike3 = new MountainBike();
		Bike bike4 = new Bike();
		
		bike1.printDescription();
		bike2.printDescription();
		bike3.printDescription();
		bike4.printDescription();

		// Пункт 8: для bike1 высота подседельного штыря изменяется с 22 на 20
		bike1.setPostHeight("20");

		// Пункт 9: вывод значений bike1 для подтверждения изменения
		System.out.println("\nbike1 values after change:");
		System.out.println("make: " + bike1.getMake());
		System.out.println("tyreWidth: " + bike1.getTyreWidth());
		System.out.println("postHeight: " + bike1.getPostHeight());
		bike1.printDescription();
	}//end method main

}//end class BikeDriver
