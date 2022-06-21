
package ija.traffic;

import ija.maps.Coordinate;
import ija.maps.Stop;
import ija.myMaps.MyStop;
import ija.myMaps.MyStreet;

import java.util.List;
import java.lang.String;

/**
 * Rozhraní autobusu.
 * Autobus je jednoznačně identifikován svým názvem (číslo autobusu).
 * Autobus jezdí po linkách s vlastním jízdním řádem. Pro danou ulici na které se
 * autobus nachází obsahuje informaci o směru, ve kterém jede po ulici.
 * Obsahuje také vlastní coordináty, kde se práve nachází. Autobus také obsahuje seznam
 * ulic a zastávek, kterými projíždí. Autobus také nese informaci o aktuálním zpoždění.
 * Autobus také nese informaci, jakým indexem zlomu projel na poslední ulici.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
public interface Bus {
    /**
     * Získá jméno autobusu.
     * @return Jméno autobusu.
     */
    String getId();

    /**
     * Vrátí id linky.
     * @return Id linky.
     */
    String getLineId();

    /**
     * Získá ulice, kterými autobus projíždí (redundantní informace).
     * Jméno ulice se páruje se jménem zastávky, na ulici.
     * Pokud autobus ulici pouze projíždí ja na pozici zastávky null.
     * @return Ulice, kterými autobus projíždí.
     */
    List<MyStreet> getStreets();


    /**
     * Získá zastávky, kterými autobus projíždí.
     * @return Zastávky, na kterým by měl autobus zastavovat.
     */
    List<MyStop> getStops();

    /**
     * Vrátí zastávky, kterými autbus ještě neprojel.
     * @return Zastávky, kterými autobus ještě neprojel.
     */
    List<Stop> getStopsAhead();

    /**
     * Změní zpoždění autobusu.
     * @param d Nové zpoždění.
     */
    void setDelay(int d);

    /**
     * Získá zpoždění autobusu.
     * @return Zpoždění autobusu.
     */
    int getDealy();

    /**
     * Získá aktuální pozici autobusu v koordinátech.
     * @return Koordináty autobusu.
     */
    Coordinate getPosition();

    /**
     * Zahájí jízdu autobusu. Nastaví index cest na 0 zjistí směr pohybu po první silnici,
     * zpoždějí nastaví na 0. Koordináty budou první zastávky.
     */
    void start(boolean d);

    /**
     * Autobus najede na novou ulici. Aktualizuje hodnoty indexu posledního
     * projetého zlomu na ulici a určí směr pohybu po ulici.
     * @param s Ulice, na kterou autobus najíždí.
     */
    void startStreet(MyStreet s);

    /**
     * Aktualizuje pozici autobusu, autobus se pohne kupředu o daný počet jednotek.
     * @param energy Počet jednotek, o které se může autobus pohnou kupředu.
     */
    boolean driveBus(double energy);

}
