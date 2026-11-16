package builder;

/**
 * Порождающие паттерны. Вариант №4 (Builder).
 * Демонстрация сборки заказов в ресторане с разными конфигурациями.
 */
public class RestaurantDemo {
    public static void main(String[] args) {
        OrderBuilder builder = new OrderBuilder();

        // 1. Полный заказ из четырёх позиций
        Order fullOrder = builder
                .setMainDish("Стейк из говядины")
                .setSideDish("Запечённый картофель")
                .setDrink("Клюквенный морс")
                .setDessert("Чизкейк")
                .build();

        // 2. Бизнес-ланч без десерта
        Order lunchOrder = builder
                .setMainDish("Куриное филе на гриле")
                .setSideDish("Рис с овощами")
                .setDrink("Зелёный чай")
                .build();

        // 3. Лёгкий заказ: только напиток и десерт
        Order coffeeBreakOrder = builder
                .setDrink("Капучино")
                .setDessert("Яблочный штрудель")
                .build();

        System.out.println("Заказ №1 (полный):      " + fullOrder);
        System.out.println("Заказ №2 (бизнес-ланч): " + lunchOrder);
        System.out.println("Заказ №3 (кофе-брейк):  " + coffeeBreakOrder);
    }
}
