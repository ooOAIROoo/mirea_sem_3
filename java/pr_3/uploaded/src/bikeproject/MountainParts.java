package bikeproject;

/**
 * Интерфейс с характеристиками горного велосипеда.
 *
 * <p>Константа {@link #TERRAIN} хранит тип местности {@code "off_road"}
 * (бездорожье). Методы интерфейса позволяют читать и изменять
 * значения полей экземпляра.</p>
 */
public interface MountainParts {
	/** Тип местности для горного велосипеда. */
	String TERRAIN = "off_road";

	/** Возвращает текущее значение поля suspension. */
	String getSuspension();

	/** Задаёт новое значение поля suspension. */
	void setSuspension(String newValue);

	/** Возвращает текущее значение поля type. */
	String getType();

	/** Задаёт новое значение поля type. */
	void setType(String newValue);
}
