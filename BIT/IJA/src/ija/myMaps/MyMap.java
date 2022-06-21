package ija.myMaps;

import ija.Vehicle.Vehicle;
import ija.maps.Map;
import ija.maps.Stop;
import ija.myTime.MyTime;
import ija.myTraffic.MyBus;
import ija.myTraffic.MyLine;
import ija.time.Time;
import ija.traffic.Line;

import java.util.ArrayList;
import java.util.List;
import java.time.format.DateTimeFormatter;
import java.time.LocalDateTime;

/**
 * Implementace rozhraní mapy (Map).
 * Mapa obsahuje ulice, zastávyk a linky. Mapa má svě měřítko, podle kterého
 * se vypočítává hodnota pohybu autobusů. Mapa obsahuje i svě interní hodiny.
 * Pro mapu je možné určit časové rozmezí aktualizace pohybu autobusů. Jedná se,
 * ale o pouhý simulovaný čas. Je proto potřeba pokud chceme čas 1:1 ke skutečnému
 * zavolat aktualizaci mapy práve za tento uplynulý čas.
 */
public class MyMap implements Map {
    /**
     * Seznam všech linky autobusů.
     */

    private List<MyLine> lines = new ArrayList<>();
    /**
     * Seznam všech zastávek.
     */
    private List<MyStop> stops = new ArrayList<>();
    /**
     * Seznam všech ulic.
     */
    private List<MyStreet> streets = new ArrayList<>();

    private List<Vehicle> buses = new ArrayList<>();


    public MyMap(){}

    /**
     * Vrátí seznam všech linek na mapě.
     * @return Seznam všech linek na mapě.
     */
    @Override
    public List<MyLine> getLines() {
        return this.lines;
    }

    /**
     * Vrátí seznam všech zastávek na mapě.
     * @return Seznam všech zastávek na mapě.
     */
    @Override
    public List<MyStop> getStops() {
        return this.stops;
    }

    /**
     * Vrátí seznam všech ulic na mapě.
     * @return Sezanm všech ulic na mapě.
     */
    @Override
    public List<MyStreet> getStreets() {
        return this.streets;
    }

    /**
     * Získá všechny autobusy na mapě. Jedoucí i stojící.
     * @return Všechny autobusy všech linek.
     */
    @Override
    public List<Vehicle> getBuses() {
        return buses;
    }
    @Override
    public void setLines(List<MyLine> lines) {
        this.lines = lines;
    }
    @Override
    public void setStops(List<MyStop> stops) {
        this.stops = stops;
    }
    @Override
    public void setStreets(List<MyStreet> streets) {
        this.streets = streets;
    }
    @Override
    public void setBuses(List<Vehicle> buses) {
        this.buses = buses;
    }
}
