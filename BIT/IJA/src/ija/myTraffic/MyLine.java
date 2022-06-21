package ija.myTraffic;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.util.StdConverter;
import ija.Vehicle.Vehicle;
import ija.maps.Stop;
import ija.myMaps.MyStop;
import ija.myMaps.MyStreet;
import ija.myTime.MyTime;
import ija.time.Time;
import ija.traffic.Bus;
import ija.traffic.Line;

import java.util.AbstractMap.SimpleImmutableEntry;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.lang.String;

/**
 * Implementace rozhraní linky (Line).
 * Linka je identifikována jménem. Obsahuje seznam zastávek a ulic,
 * kterými projíždí, a to v přesném požadí, pokud linka ulici pouze projízdí,
 * je použita pro přidání metoda addStreet, která nastaví na stejné úrovni
 * v zastávkách null. Seznam zastávek a ulic spolu 1:1 korespondují.
 * Linka také obsahuje jízdní řád.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
@JsonDeserialize(converter = MyLine.lineCtor.class)
public class MyLine implements Line {
    /**
     * Jméno linky.
     */
    private String id;
    /**
     * Seznam ulic, které linka projíždí.
     */
    public List<MyStreet> streets = new ArrayList<>();
    /**
     * Seznam zastávek, kde linka zastavuje.
     */
    public List<MyStop> stops = new ArrayList<>();

    /**
     * Jízdní řád všech autobusů na lince.
     */
    @JsonIgnore
    private List<List<Time>> timetable = new ArrayList<>();

    /**
     * Všechny autobusy linky, čekající i jedoucí.
     */
    public List<Vehicle> buses = new ArrayList<>();

    /**
     * Jedoucí autobusy linky.
     */
    @JsonIgnore
    private List<Vehicle> runningBusses = new ArrayList<Vehicle>();

    /**
     * Průměrná rychlost autobusů.
     */
    @JsonIgnore
    private int speed = 1;  //TODO - změnit rychlost

    /**
     * Poslední čas (imaginární - vnitřní) aktualizace spojů.
     */
    @JsonIgnore
    private Time lastActualise = new MyTime("00:00");

    private MyLine(){}

    /**
     * Konstrktor linky. Lince přidělí pouze její jméno.
     * @param id Jméno vytvářené linky.
     */
    public MyLine(String id) {
        this.id = id;
    }

    /**
     * Vrátí jméno linky.
     * @return Jméno linky.
     */
    @Override
    public String getId() {
        return this.id;
    }

    /**
     * Získá veškeré cesty, kterými linka projíždí.
     * @return Seznam cest, které linky projíždí.
     */
    @Override
    public List<MyStreet> getStreets() {
        return this.streets;
    }

    /**
     * Přidá ulici do cesty linky. Přidává pouze ulice, které se projíždí
     * a není na nich zastávka. Došlo by k chybě. Přidávaná cesta musí navazovat na
     * poslední cestu z linky a nesmí být s ní shodná (redundance). První cesta lze přidat vždy.
     * @param street Přidávaná cesta do linky.
     * @return Úspěšnost přidání cesty do linky.
     */
    @Override
    public boolean addStreet(MyStreet street) {
        if(this.streets.isEmpty() || (this.lastStreet().follows(street) && !this.lastStreet().equals(street))) {
            this.streets.add(street);
            this.stops.add(null);
            return true;
        }
        return false;
    }

    /**
     * Získá veškeré zastávky, kde linka staví.
     * @return Seznam zastávek linky.
     */
    @Override
    public List<MyStop> getStops() {
        return this.stops;
    }

    /**
     * Přidá do linky zastávku, současně se na stejé úrovni v ulicích
     * jastaví ulice na které je zastávka definována. Proto přidávaná zastávka
     * již musí obsahovat ulici. <STRONG>Vždy používat tuto metodu.</STRONG> Pouze v případě,
     * že se ulice projíždí, je použita jiná metoda "addStreet".
     * Přidávaná zastávka s její ulicí musí navazovat na poslední ulici linky,
     * buď se jedná o stejnou ulici, nebo přímo navazující.
     * @param stop Přidávaná zastávka do linky.
     * @return Úspěšnost přidání zastávky
     */
    @Override
    public boolean addStop(Stop stop) {
        if(this.stops.isEmpty()) {
            this.stops.add((MyStop)stop);
            return true;
        }
        else if(this.lastStreet().follows(stop.getStreet())) {
            this.stops.add((MyStop)stop);
            return true;
        }
        return false;
    }

    /**
     * Vloží do linky autobus s jeho jizdním řádem.
     * @param b Autobus pro vložení.
     * @param t Jízdní řád autobusu.
     */
    @Override
    public void addBus(Vehicle b, List<Time> t) {
        this.buses.add(b);
        this.timetable.add(t);
    }

    @Override
    public void setStreets(List<MyStreet> streets) {
        this.streets = streets;
    }

    @Override
    public void setStops(List<MyStop> stops) {
        this.stops = stops;
    }

    @Override
    public void setBuses(List<Vehicle> buses) {
        this.buses = buses;
    }

    /**
     * Získá veškeré zastávky, kde linka staví.
     * @return Seznam zastávek linky.
     */
    @Override
    public List<Vehicle> getBuses() {
        return this.buses;
    }

    /**
     * Zahájí jízdu autobusu linky.
     * @param idx Index autobusu v poli autobusů linky.
     */
    @Override
    public void startBus(int idx) {
        this.runningBusses.add(this.buses.get(idx));
    }

    /**
     * Získá startovní čas daného řádku jízdního řádu.
     * @param shedule Jízdní řád jednoho autobusu.
     * @return Startovní čas autobusu.
     */
    private static Time startTime(List<Time> shedule) {
        return shedule.get(0);
    }

    /**
     * Získá čas příjezdu autobusu do cíle.
     * @param shedule Jízdní řád jednoho autobusu.
     * @return Čas příjezdu do síle autobusu.
     */
    private static Time endTiem(List<Time> shedule) {
        return shedule.get(shedule.size() - 1);
    }

    /**
     * Vrátí časové údaje z jízdního řádu konkrétního autobusu.
     * @param b Autobus, u kterého dotazujeme jízdní řád.
     * @return Časy jízdního řádu autobusu.
     */
    @Override
    public List<Time> timetableBus(Bus b) {
        return this.timetable.get(buses.indexOf(b));
    }

    /**
     * Spočítá energii (kolik může ujet) autobusu získanou za daný čas.
     * @param sec Počet sekund od minulé aktualizace.
     * @return Vrátí hodnotu energie autobusu na dapší posun.
     */
    private int countEnergy(int sec) {
        //TODO výpočet posunu autobusu za sekundu
        return 55;
    }

    private List<MyStreet> backUpStreets = new ArrayList<>();

    /**
     * Zahájí jízdu celé linky. Použije se při restartu aplikace, nebo jejím zahájení.
     * @param t Aktuální čas.
     */
    @Override
    public void startLine(Time t) {
        this.lastActualise.setTime(t);
        int i = 0;
        // Každý autobu zkontroluje, zda neměl být na svojí jízdě.
        // Pokud ano, nastartuje jej a posune o odpovídající počet.
        for (Vehicle b: this.buses) {
            if(startTime(this.timetable.get(i)).isBefore(t) &&
               t.isBefore(endTiem(this.timetable.get(i)))) {
                this.startBus(i);
                // Kontrola, zda autobus již nedojel, pokud ano, odstraní
                // se ze seznamu jedoucích autobusu.

            }
            i++;
        }

    }

    public void setId(String id) {
        this.id = id;
    }

    /**
     * Aktualizuje informace o lince. Odstartuje případně nové autobusy.
     * @param t Aktuální čas.
     */
    @Override
    public void actualisLine(Time t) {
        int i = 0;
        for (Vehicle b: this.buses) {
            if(this.runningBusses.contains(b)) {
                // Kontrola, zda autobus již nedojel, pokud ano, odstraní
                // se ze seznamu jedoucích autobusu.

            }
            else {
                if(startTime(this.timetable.get(i)).isBefore(t) &&
                        t.isBefore(endTiem(this.timetable.get(i)))) {
                    this.startBus(i);
                    // Kontrola, zda autobus již nedojel, pokud ano, odstraní
                    // se ze seznamu jedoucích autobusu.

                }
            }
            i++;
        }

        // Aktualizuje čas podlení úpravy.
        this.lastActualise.setTime(t);
    }

    /**
     * Získá poslední ulici, na kterou linky projíždí.
     * @return Poslední ulice linky.
     */
    private MyStreet lastStreet() {
        return this.streets.get(this.streets.size() -1);
    }

    /**
     * Vrátí seznam dvojic ulice+zastávka pro danou linku. Pokud linka danou ulici
     * pouze projíždí, je na místo zastávky null.
     * @return Seznam dvojic ulice+zastávka dané linky.
     */
    @Override
    public List<SimpleImmutableEntry<MyStreet, Stop>> getRoute() {
        List<SimpleImmutableEntry<MyStreet,Stop>> l = new ArrayList<>();
        for(int i = 0; i < this.stops.size(); i++) {
            l.add(new SimpleImmutableEntry<MyStreet,Stop>(this.streets.get(i), this.stops.get(i)));
        }
        return l;
    }

    /**
     * Dvě linky jsou sterjné, pokud se rovnají jejich jména.
     * @param o Druhá porovnávaná linka.
     * @return Booleovská hodnota ekvivalence dvou linek.
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        MyLine myLine = (MyLine) o;
        return Objects.equals(id, myLine.id);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }

    public List<MyStreet> getBackUpStreets() {
        return backUpStreets;
    }

    public void setBackUpStreets() {
        if(this.backUpStreets.equals(this.streets)){
            return;
        }
        else {
            List<MyStreet> reversedStreet = new ArrayList<>();
            for(int i = this.streets.size()-1; i >= 0; i--){
                reversedStreet.add(this.streets.get(i));
            }
            if(this.backUpStreets.equals(reversedStreet)){
                return;
            }
        }
        this.backUpStreets.clear();
        this.backUpStreets.addAll(this.streets);
    }

    private void lineToBusStreet() {
        for(Vehicle bus: this.buses) {
            bus.setLine(this);
        }
        for(MyStreet street: this.streets) {
            street.addLine(this);
        }
        this.setBackUpStreets();
    }

    static class lineCtor extends StdConverter<MyLine, MyLine> {
        @Override
        public MyLine convert(MyLine myLine) {
            myLine.lineToBusStreet();
            return myLine;
        }
    }
}
