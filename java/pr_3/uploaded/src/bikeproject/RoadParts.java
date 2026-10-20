package bikeproject;

/**
 * Интерфейс с характеристиками шоссейного велосипеда.
 *
 * <p>Константа {@link #terrain} хранит тип местности
 * {@code "track_racing"} (шоссейные гонки). Методы интерфейса
 * позволяют читать и изменять значения полей экземпляра.</p>
 */
public interface RoadParts {
	/** Тип местности для шоссейного велосипеда. */
	String terrain = "track_racing";

	/** Возвращает текущее значение поля tyreWidth. */
	int getTyreWidth();

	/** Задаёт новое значение поля tyreWidth (строковый аргумент). */
	void setTyreWidth(String newValue);

	/** Возвращает текущее значение поля postHeight. */
	int getPostHeight();

	/** Задаёт новое значение поля postHeight (строковый аргумент). */
	void setPostHeight(String newValue);
}
