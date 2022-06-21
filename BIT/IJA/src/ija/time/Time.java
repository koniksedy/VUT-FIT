package ija.time;

/**
 * Rozhraní času.
 * Čas je specifikovám počtem sekund od začátku dne.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
public interface Time {

    /**
     * Nastaví čas na zadanou hodnotu.
     * @param t Nová hodnota času.
     */
    void setTime(Time t);

    /**
     * Získá čas v sekundách.
     * @return Čas v sekundách.
     */
    int getTime();

    /**
     * Porovná, zda je čas před časem t (parametr).
     * @param t Druhý porovnávaný čas.
     * @return Výsledek porovnání času this before t.
     */
    boolean isBefore(Time t);

    /**
     * Provná, zda je čas po čase t (parametr).
     * @param t Druhý porovnávaný čas.
     * @return Výsledek porovnání času this after t.
     */
    boolean isAfter(Time t);

    /**
     * Vrátí rozdíl dvou časů. Nefunguje přez půlnoc.
     * @param b Druhý čas.
     * @return Rozdíl dvou časů.
     */
    int diff(Time b);

    /**
     * Provede tiknutí času o zadaný počet sekund.
     * @param add Přidávaný počet sekund.
     */
    void tick(int add);


}
