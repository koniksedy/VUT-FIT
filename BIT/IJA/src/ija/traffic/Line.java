package ija.traffic;

import ija.Vehicle.Vehicle;
import ija.maps.Stop;
import ija.myMaps.MyStop;
import ija.myMaps.MyStreet;
import ija.time.Time;
import javafx.scene.paint.Color;

import java.util.AbstractMap.SimpleImmutableEntry;
import java.util.List;
import java.lang.String;

/**
 * Rozhraní linky (Line).
 * Linka je identifikována jménem. Obsahuje seznam zastávek a ulic,
 * kterými projíždí, a to v přesném požadí, pokud linka ulici pouze projízdí,
 * je použita pro přidání metoda addStreet, která nastaví na stejné úrovni
 * v zastávkách null. Seznam zastávek a ulic spolu 1:1 korespondují.
 * Linka také obsahuje jízdní řád.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
public interface Line {

    /**
     * Vrátí jméno linky.
     * @return Jméno linky.
     */
    String getId();

    void setStreets(List<MyStreet> streets);

    void setStops(List<MyStop> stops);

    void setBuses(List<Vehicle> buses);

    /**
     * Získá veškeré cesty, kterými linka projíždí.
     * @return Seznam cest, které linky projíždí.
     */
    List<MyStreet> getStreets();

    /**
     * Získá veškeré zastávky, kde linka staví.
     * @return Seznam zastávek linky.
     */
    List<MyStop> getStops();

    /**
     * Vrátí seznam všech autobusů linyk.
     * @return Všechny autobusy linky.
     */
    List<Vehicle> getBuses();

    /**
     * Vloží do linky autobus s jeho jizdním řádem.
     * @param b Autobus pro vložení.
     * @param t Jízdní řád autobusu.
     */
    void addBus(Vehicle b, List<Time> t);

    /**
     * Přidá do linky zastávku, současně se na stejé úrovni v ulicích,
     * na které je zastávka definována. Proto přidávaná zastávka
     * již musí obsahovat ulici. <STRONG>Vždy používat tuto metodu.</STRONG> Pouze v případě,
     * že se ulice projíždí, je použita jiná metoda "addStreet".
     * Přidávaná zastávka s její ulicí musí navazovat na poslední ulici linky,
     * buď se jedná o stejnou ulici, nebo přímo navazující.
     * @param stop Přidávaná zastávka do linky.
     * @return Úspěšnost přidání zastávky
     */
    boolean addStop(Stop stop);

    /**
     * Přidá ulici do cesta linky. Přidává pouze ulice, které se projíždí
     * a není na nich zastávka. Došlo by k chybě. Přidávaná cesta musí navazovat na
     * poslední cestu z linky a nesmí být s ní shodná (redundance). První cesta lze přidat vždy.
     * @param street Přidávaná cesta do linky.
     * @return Úspěšnost přidání cesty do linky.
     */
    boolean addStreet(MyStreet street);

    /**
     * Vrátí seznam dvojic ulice+zastávka pro danou linku. Pokud linka danou ulici
     * pouze projíždí, je na místo zastávky null.
     * @return Seznam dvojic ulice+zastávka dané linky.
     */
    List<SimpleImmutableEntry<MyStreet,Stop>> getRoute();

    /**
     * Aktualizuje informace o lince. Odstartuje případně nové autobusy.
     * @param t Aktuální čas.
     */
    void actualisLine(Time t);

    /**
     * Zahájí jízdu celé linky. Použije se při restartu aplikace, nebo jejím zahájení.
     * Nastaruje a vhodně umístí autobusy, které již měly být na cestě.
     * @param t Aktuální čas.
     */
    void startLine(Time t);

    /**
     * Zahájí jízdu autobusu na daném indexu linky.
     * @param idx Index autobusu v poli autobusů linky.
     */
    void startBus(int idx);

    /**
     * Vrátí časové údaje z jízdního řádu konkrétního autobusu.
     * @param b Autobus, u kterého dotazujeme jízdní řád.
     * @return Časy jízdního řádu autobusu.
     */
    List<Time> timetableBus(Bus b);


}
