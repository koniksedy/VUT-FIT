package ija.maps;

import ija.Vehicle.Vehicle;
import ija.myMaps.MyStop;
import ija.myMaps.MyStreet;
import ija.myTraffic.MyLine;
import ija.time.Time;
import ija.traffic.Line;

import java.util.List;

/**
 * Rozhraní mapy.
 * Mapa obsahuje ulice, zastávyk a linky. Mapa má svě měřítko, podle kterého
 * se vypočítává hodnota pohybu autobusů. Mapa obsahuje i svě interní hodiny.
 * Pro mapu je možné určit časové rozmezí aktualizace pohybu autobusů. Jedná se,
 * ale o pouhý simulovaný čas. Je proto potřeba pokud chceme čas 1:1 ke skutečnému
 * zavolat aktualizaci mapy práve za tento uplynulý čas.
 */
public interface Map {
    /**
     * Vrátí seznam všech linek na mapě.
     * @return Seznam všech linek na mapě.
     */
    List<MyLine> getLines();

    /**
     * Vrátí seznam všech zastávek na mapě.
     * @return Seznam všech zastávek na mapě.
     */
    List<MyStop> getStops();

    /**
     * Vrátí seznam všech ulic na mapě.
     * @return Sezanm všech ulic na mapě.
     */
    List<MyStreet> getStreets();

    /**
     * Získá všechny autobusy na mapě. Jedoucí i stojící.
     * @return Všechny autobusy všech linek.
     */
    List<Vehicle> getBuses();

    void setBuses(List<Vehicle> buses);

    void setStreets(List<MyStreet> streets);

    void setLines(List<MyLine> lines);

    void setStops(List<MyStop> stops);

}