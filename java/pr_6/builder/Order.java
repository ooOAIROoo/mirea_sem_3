package builder;

/**
 * Порождающие паттерны. Вариант №4 (Builder).
 * Класс заказа в ресторане с четырьмя параметрами: основное блюдо, гарнир, напиток, десерт.
 */
public class Order {
    private String mainDish;
    private String sideDish;
    private String drink;
    private String dessert;

    public String getMainDish() {
        return mainDish;
    }

    public void setMainDish(String mainDish) {
        this.mainDish = mainDish;
    }

    public String getSideDish() {
        return sideDish;
    }

    public void setSideDish(String sideDish) {
        this.sideDish = sideDish;
    }

    public String getDrink() {
        return drink;
    }

    public void setDrink(String drink) {
        this.drink = drink;
    }

    public String getDessert() {
        return dessert;
    }

    public void setDessert(String dessert) {
        this.dessert = dessert;
    }

    private String formatItem(String value) {
        return (value == null || value.isEmpty()) ? "не выбрано" : value;
    }

    @Override
    public String toString() {
        return "Заказ {"
                + "основное блюдо='" + formatItem(mainDish) + "', "
                + "гарнир='" + formatItem(sideDish) + "', "
                + "напиток='" + formatItem(drink) + "', "
                + "десерт='" + formatItem(dessert) + "'}";
    }
}
