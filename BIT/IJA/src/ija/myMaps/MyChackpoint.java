package ija.myMaps;

import ija.maps.Chackpoint;

/**
 * Implementace rozhraní počítadloa Chackpointu.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
public class MyChackpoint implements Chackpoint {
    /**
     * Index posledního projetého chackpointu v seznamu bodů
     * definujících ulici.
     */
    private int val;


    /**
     * Konstruktor vatvoří počítadlo chackpointu se zadaným indexem.
     * @param x Index počítadla.
     */
    public MyChackpoint(int x) {
        this.val = x;
    }

    /**
     * Nastaví počítadlo na zadaný index.
     * @param x Nový index počítadla.
     */
    @Override
    public void set(int x) {
        this.val = x;
    }

    /**
     * Získá hodnotu počítadla.
     * @return Hodnota počítadla. (Index posledníhoh chackpointu.)
     */
    @Override
    public int get() {
        return this.val;
    }

    /**
     * Inkrementuje počítadlo.
     */
    @Override
    public void inc() {
        this.val += 1;
    }

    /**
     * Dekrementuje počítadlo.
     */
    @Override
    public void  dec() {
        this.val -= 1;
    }
}
