package vehicles;

/**
 * Интерфейс, описывающий специфические методы электромобиля.
 *
 * <p>Все методы интерфейса являются публичными и абстрактными
 * (модификаторы {@code public abstract} указываются неявно).</p>
 */
public interface ElectricVehicle {
    /**
     * Возвращает ёмкость батареи электромобиля.
     *
     * @return ёмкость батареи в киловатт-часах
     */
    int getBatteryCapacity();

    /**
     * Задаёт ёмкость батареи электромобиля.
     *
     * @param batteryCapacity новая ёмкость батареи в киловатт-часах
     */
    void setBatteryCapacity(int batteryCapacity);
}
