/**
 * Implementace rozhraní času (Time).
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
package ija.myTime;

import ija.time.Time;

import java.lang.String;

import static java.lang.Math.abs;

/**
 * Implementace rozhraní času.
 * Čas je specifikovám počtem sekund od začátku dne.
 */
public class MyTime implements Time {
    /**
     * Počet sekund od počátku dne.
     */
    private int s;


    /**
     * Konstruktor času.
     * Zadávání času ve formátu HH:MM:SS, nebo HH:MM.
     * Formát času nekontroluje.
     * @param t Převáděný čas ve formátu HH:MM:SS, nebo HH:MM.
     */
    public MyTime(String t) {
        String[] parsed = t.split(":");
        if(parsed.length == 3) {
            this.s = Integer.parseInt(parsed[2]);
        }
        this.s += Integer.parseInt(parsed[1]) * 60;
        this.s += Integer.parseInt(parsed[0]) * 3600;
    }

    /**
     * Nastaví čas na zadanou hodnotu.
     * @param t Nová hodnota času.
     */
    @Override
    public void setTime(Time t) {
        this.s = t.getTime();
    }

    /**
     * Získá čas v sekundách.
     * @return Čas v sekundách.
     */
    @Override
    public int getTime() {
        return this.s;
    }

    /**
     * Porovná, zda je čas před časem t (parametr).
     * @param t Druhý porovnávaný čas.
     * @return Výsledek porovnání času this before t.
     */
    @Override
    public boolean isBefore(Time t) {
        return this.s <= t.getTime();
    }

    /**
     * Provná, zda je čas po čase t (parametr).
     * @param t Druhý porovnávaný čas.
     * @return Výsledek porovnání času this after t.
     */
    @Override
    public boolean isAfter(Time t) {
        return this.s > t.getTime();
    }

    /**
     * Vrátí rozdíl dvou časů. Nefunguje přez půlnoc.
     * @param b Druhý čas.
     * @return Rozdíl dvou časů.
     */
    @Override
    public int diff(Time b) {
        return abs(this.s - b.getTime());
    }

    /**
     * Provede tiknutí času o zadaný počet sekund.
     * @param add Přidávaný počet sekund.
     */
    public void tick(int add) {
        this.s += add;
        // Omezení přetečení. 1 den = 86400 sec.
        if(this.s > 86400) {
            this.s = this.s % 86400;
        }
    }

    /**
     * Vrátí uchovávaný čas ve formátu HH:MM:SS.
     * @return Čas ve formátu HH:MM:SS.
     */
    @Override
    public String toString() {
        String h = String.format("%02d", this.s / 3600);
        String m  = String.format("%02d", (this.s % 3600) / 60);
        String s = String.format("%02d", this.s % 60);
        return h + ":" + m + ":" + s;
    }


}
