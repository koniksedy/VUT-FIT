package ija.maps;

/**
 * Rozhraní počítadla posleního projetého zlomu ulice.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
public interface Chackpoint {

    /**
     * Nastaví počítadlo na zadaný index.
     * @param x Nový index počítadla.
     */
    void set(int x);

    /**
     * Získá hodnotu počítadla.
     * @return Hodnota počítadla. (Index posledníhoh chackpointu.)
     */
    int get();

    /**
     * Inkrementuje počítadlo.
     */
    void inc();

    /**
     * Dekrementuje počítadlo.
     */
    void dec();
}
