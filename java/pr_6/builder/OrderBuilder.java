package builder;

/**
 * Порождающие паттерны. Вариант №4 (Builder).
 * Строитель для пошагового создания объекта Order.
 */
public class OrderBuilder {
    private Order order;

    public OrderBuilder() {
        this.order = new Order();
    }

    public OrderBuilder setMainDish(String mainDish) {
        order.setMainDish(mainDish);
        return this;
    }

    public OrderBuilder setSideDish(String sideDish) {
        order.setSideDish(sideDish);
        return this;
    }

    public OrderBuilder setDrink(String drink) {
        order.setDrink(drink);
        return this;
    }

    public OrderBuilder setDessert(String dessert) {
        order.setDessert(dessert);
        return this;
    }

    public Order build() {
        Order readyOrder = order;
        order = new Order();
        return readyOrder;
    }
}
