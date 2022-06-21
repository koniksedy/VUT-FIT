package ija.myTraffic;

import ija.maps.Chackpoint;
import ija.maps.Coordinate;
import ija.maps.Stop;
import ija.myMaps.MyChackpoint;
import ija.myMaps.MyStop;
import ija.myMaps.MyStreet;
import ija.time.Time;
import ija.traffic.Bus;
import ija.traffic.Line;

import java.util.ArrayList;
import java.util.List;
import java.lang.String;

/**
 * Implementace rozhraní autbusu (Bus).
 * Autobus je jednoznačně identifikován svým názvem (číslo autobusu).
 * Autobus jezdí po linkách s vlastním jízdním řádem. Pro danou ulici na které se
 * autobus nachází obsahuje informaci o směru, ve kterém jede po ulici.
 * Obsahuje také vlastní coordináty, kde se práve nachází. Autobus také obsahuje seznam
 * ulic a zastávek, kterými projíždí. Autobus také nese informaci o aktuálním zpoždění.
 * Autobus také nese informaci, jakým indexem zlomu projel na poslední ulici.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
public class MyBus implements Bus {
    /**
     * Název autobusu.
     */
    private String id;
    /**
     * Název linky autobusu.
     */
    private String lineId;
    /**
     * Počítadlo bodů definujících ulici (chackpoint),
     * kterými již autobus projel.
     */
    private Chackpoint breakIdx = new MyChackpoint(0);
    /**
     * Určuje směr pohybu na aktuální ulici.
     */
    private boolean direction;
    /**
     * Zpoždění autobusu.
     */
    private int delay = 0;
    /**
     * Pozice autobusu.
     */
    private Coordinate position;
    /**
     * Index (ze seznamu ulic na trace autobusu) ulice na, které se autobus nachází.
     */
    private Integer streetIdx = 0;
    /**
     * Seznam ulic, po kterých atobus jede.
     */
    private List<MyStreet> streets;
    /**
     * Seznam zastávek na, kterých autobus zastavuje.
     */
    private List<MyStop> stops;
    /**
     * Jízdní řád autobusu.
     */
    private List<Time> timetable;
    /**
     * Odkaz na linku, pro kterou jezdí autobus.
     */
    private Line line;


    /**
     * Konstruktor autobusu. Nastaví id autobausu, a získá základní údaje z linky:
     * id linky, ulice a zastávky a jízdní řád pro daný autobus.
     * @param id Id autobusu.
     * @param l Linka, pro kterou autobus jezdí.
     * @param c Počáteční koordináty autobusu.
     */
    public MyBus(String id, Line l, Coordinate c) {
        this.id = id;
        this.lineId = l.getId();
        this.position = c;
        this.streets = l.getStreets();
        this.stops = l.getStops();
        this.line = l;
    }

    /**
     * Získá jméno autobusu.
     *
     * @return Jméno autobusu.
     */
    @Override
    public String getId() {
        return this.id;
    }

    /**
     * Vrátí id linky.
     * @return Id linky.
     */
    @Override
    public String getLineId() {
        return this.lineId;
    }

    /**
     * Získá ulice, kterými autobus projíždí (redundantní informace).
     * Jméno ulice se páruje se jménem zastávky, na ulici.
     * Pokud autobus ulici pouze projíždí ja na pozici zastávky null.
     *
     * @return Ulice, kterými autobus projíždí.
     */
    @Override
    public List<MyStreet> getStreets() {
        return this.streets;
    }

    /**
     * Získá zastávky, kterými autobus projíždí.
     *
     * @return Zastávky, na kterým by měl autobus zastavovat.
     */
    @Override
    public List<MyStop> getStops() {
        return this.stops;
    }

    /**
     * Vrátí zastávky, kterými autbus ještě neprojel.
     *
     * @return Zastávky, kterými autobus ještě neprojel.
     */
    @Override
    public List<Stop> getStopsAhead() {
        List<Stop> stops = new ArrayList<>();
        for(int i = this.streetIdx; i < this.streets.size(); i++) {
            if(this.stops.get(i) != null && this.streets.get(i).isAhead(this.position, this.stops.get(i).getCoords(), this.direction)){
                stops.add(this.stops.get(i));
            }
        }
        return stops;
    }

    /**
     * Změní zpoždění autobusu.
     * @param d Nové zpoždění v sekundách.
     */
    @Override
    public void setDelay(int d) {
        this.delay = d;
    }

    /**
     * Získá zpoždění autobusu.
     *
     * @return Zpoždění autobusu.
     */
    @Override
    public int getDealy() {
        return this.delay;
    }

    /**
     * Získá aktuální pozici autobusu v koordinátech.
     *
     * @return Koordináty autobusu.
     */
    @Override
    public Coordinate getPosition() {
        return this.position;
    }

    /**
     * Autobus najede na novou ulici. Aktualizuje hodnoty indexu posledního
     * projetého zlomu na ulici a určí směr pohybu po ulici.
     *
     * @param s Ulice, na kterou autobus najíždí.
     */
    @Override
    public void startStreet(MyStreet s) {
        this.direction = s.getDirection(this.position);
        if(direction) {
            this.breakIdx.set(0);
        }
        else {
            this.breakIdx.set(s.getCoordinates().size() -1);
        }

    }

    /**
     * Získá ze seznamu ulic, kterými autobus projíždí index nové navazující.
     * Pokud takovou ulici nenajde, vrátí false.
     * @return Úspěšnost získání další ulice, po které lze pokračovat.
     */
    private boolean nextStreet() {
        for(int i = this.streetIdx; i < this.streets.size(); i++) {
            if(!this.streets.get(i).equals(this.streets.get(this.streetIdx))){
                this.streetIdx = i;
                return true;
            }
        }
        return false;
    }

    /**
     * Zahájí jízdu autobusu. Nastaví index cest na 0 zjistí směr pohybu po první silnici,
     * zpoždějí nastaví na 0. Koordináty budou první zastávky.
     * @param dir Směr pohybu po ulici.
     */
    @Override
    public void start(boolean dir) {
        this.streetIdx = 0;
        this.position = this.stops.get(0).getCoords();
        this.delay = 0;
        this.direction = dir;
    }

    /**
     * Aktualizuje pozici autobusu, autobus se pohne kupředu o daný počet jednotek.
     *
     * @param energy Počet jednotek, o které se může autobus pohnou kupředu.
     * @return Booleovská hodnota vyjetí veškeré energie. false --&gt; konec linky.
     */
    @Override
    public boolean driveBus(double energy) {
        while(energy != 0) {
            // Provedení jízdy a spočítání zbylé energie.
            energy = this.streets.get(this.streetIdx).driveStreet(this.position, this.breakIdx, energy, this.direction);
            // Pokud jsme nevyjeli veškerou energii, je potřeba získat novou ulici.
            if(energy != 0) {
                // Kontrola, zda nová ulice existuje
                if(!this.nextStreet()) {
                    return false;
                }
            }
        }
        return true;
    }
}
