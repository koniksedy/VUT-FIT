/**
 * Implementace Ulice.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
package ija.myMaps;

import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.util.StdConverter;
import ija.Main_controller;
import ija.Time_update;
import ija.Vehicle.Vehicle;
import ija.myTraffic.MyLine;
import javafx.scene.input.MouseEvent;

import java.time.LocalTime;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;

import ija.maps.Chackpoint;
import ija.maps.Coordinate;
import ija.maps.Stop;
import ija.myDraw.Drawable;
import javafx.event.EventHandler;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.scene.shape.Shape;
import javafx.scene.text.Text;

import com.fasterxml.jackson.annotation.JsonIgnore;

import static java.lang.Double.min;

/**
 * Implementace rozhraní ulice (Street).
 * Každá ulice je identifikována svým jménem. Ulice je dále definována seznamem
 * soužadnic, které identifikují zlomy ulice. Zlomy jsou povoleny pouze pravoůhlé.
 * Na ulici se nachází seznam zastáve, ulice může být zaké prázdná.
 */
@JsonDeserialize(converter = MyStreet.streetCtor.class)
@JsonIdentityInfo(generator = ObjectIdGenerators.PropertyGenerator.class, property = "id")
public class MyStreet implements  Drawable, Time_update {

    public String id;

    @JsonIgnore
    public Coordinate begin;
    @JsonIgnore
    public Coordinate end;

    private List<MyCoordinate> coords = new ArrayList<>();

    private List<MyStop> stops = new ArrayList<>();
    @JsonIgnore
    public Main_controller controller;
    @JsonIgnore
    private List<MyLine> linesOnStreet = new ArrayList<>();
    @JsonIgnore
    public Line line = null;
    @JsonIgnore
    public static MyStreet last_street_clicked;
    @JsonIgnore
    public List<String> lines;

    private MyStreet(){}


    public MyStreet(String id, Coordinate begin, Coordinate end) {
        this.id = id;
        this.coords.add(0,  (MyCoordinate)begin);
        this.coords.add(1,  (MyCoordinate)end);
    }

    @JsonIgnore
    public void setBegin(MyCoordinate begin) {
        this.begin = begin;
    }

    @JsonIgnore
    public void setEnd(MyCoordinate end) {
        this.end = end;
    }


    public void setCoords(List<MyCoordinate> coords) {
        this.coords = coords;
    }

    public void addLine(MyLine line) {
        this.linesOnStreet.add(line);
    }

    public List<MyLine> getLinesOnStreet() {
        return linesOnStreet;
    }

    /**
     * Jméno ulice.
     */

    public void addControler(Main_controller controller){
        this.controller = controller;
    }

    public static MyStreet createStreet(String id, Coordinate coords, Coordinate coords1) {
            MyStreet street = new MyStreet(id,coords,coords1);
            return street;
    }

    /**
     * Hustota provozu na ulici. 1 - standartní hustota
     */
    private double trafic = 1;
    /**
     * Seznam koordinát definujících ulici.
     * Na indexu [0] značí začátek ulice a poslední index značí konec.
     */




    /**
     * Konstruktor ulice. Vytvoří prázdnou ulici pouze se jménem.
     * @param id Jméno nové ulice.
     */
    public MyStreet(String id) {
        this.id = id;
    }

    /**
     * Přidá koordináty do ulice.
     * Ulici vlastně uozšíří. Počítá se s tím, že koordináty jsou již zkontrolované.
     * @param c Koordináty pro přidání do ulice.
     */

    public void addCoordToStreet(Coordinate c){
        this.coords.add((MyCoordinate)c);
    }

    @JsonIgnore
    @Override
    public List<Shape> getGui() {
        Text jmeno;
        if(this.begin().getY() == this.end().getY()) {
            jmeno = new Text(min(this.begin().getX(), this.end().getX()) + (this.begin().diffX(this.end()) / 2),
                    (this.begin().getY() > 30) ?  (this.begin().getY() - 20) : (this.begin().getY() + 20), this.id);

        } else {
            jmeno = new Text(this.begin().getX() + 15, min(this.begin().getY(), this.end().getY()) + (this.begin().diffY(this.end()) / 2), this.id);
        }
        Line linka = new Line(this.begin().getX(), this.begin().getY(), this.end().getX(), this.end().getY());
        this.line = linka;
        this.line.setStrokeWidth(2);
        linka.setOnMouseClicked(e -> highlight());
        jmeno.setOnMouseClicked(e -> highlight());

       // linka.addEventHandler(MouseEvent.MOUSE_CLICKED, eventHandler);
        return Arrays.asList(jmeno, linka);
    }




    public void highlight() {
        unhighlight();
        if(!controller.getClosedStreets().contains(this) && !controller.getDetourStreets().contains(this)) {
            this.line.setStroke(Color.BLUE);
            this.line.setStrokeWidth(10);
        }
        last_street_clicked = this;
        controller.setText_pole(this.id);
        controller.setText_stops(this.getStops());
        controller.setText_lines(this.getLinesOnStreet());
        controller.setTraficSpinnerValue((int)this.trafic);
    }
    public void unhighlight() {
//        if(controller.choice1 != null){
//            for (MyLine linka: controller.liner
//            ) {
//                if(linka.getId() == controller.choice1){
//                    for (MyStreet ulice: linka.streets
//                    ) {
//                        if(Vehicle.lastBusClicked == null) {
//                            ulice.line.setStroke(Color.BLACK);
//                        }
//                    }
//                }
//
//            }
//        }
        if(last_street_clicked != null) {
            if(Vehicle.lastBusClicked == null && (Main_controller.last_clicked_line == null || (!Main_controller.last_clicked_line.getStreets().contains(last_street_clicked)))
            && !controller.getDetourStreets().contains(last_street_clicked) && !controller.getClosedStreets().contains(last_street_clicked)) {
                last_street_clicked.line.setStrokeWidth(2);
                last_street_clicked.line.setStroke(Color.BLACK);
            }
            controller.unsetText_pole();
            controller.unsetText_stops();
            controller.unsetText_lines();
        }
    }



    EventHandler<MouseEvent> eventHandler = new EventHandler<MouseEvent>() {
        @Override
        public void handle(MouseEvent e) {
            List<Drawable> elements = new ArrayList<>();
            MyStreet.this.line.setStroke(Color.RED);
            MyStreet.this.line.setStrokeWidth(10);


        }
    };


    /**
     * Rozhodne, zda se dotazované číslo nachází v intervalu &lt;min(a,b), max(a,b)&gt;.
     * @param x Kontrolovaná hodnota.
     * @param a První hranice intervalu.
     * @param b Druhá hranice intervalu.
     * @return Booleovská hodnota existence x na intervatu &lt;min(a,b), max(a,b)&gt;.
     */
    static public boolean isBetween(double x, double a, double b) {
        if(a <= b) {
            return x >= a && x <= b;
        }
        else {
            return x >= b && x <= a;
        }

    }

    /**
     * Rozhodne, zda se na ulici nachází zadaná zastávka..
     * @param stop Zkoumaná zastávka.
     * @return Booleovská hodnota výskytu zastávky přímo na ulici.
     */
    private boolean isOnStreet(Stop stop) {
        for(int i = 1; i < this.coords.size(); i++) {
            Coordinate first = this.coords.get(i-1);
            Coordinate second = this.coords.get(i);
            if(first.getX() == second.getX() && stop.getCoords().getX() == first.getX()) {
                if(isBetween(stop.getCoords().getY(), first.getY(), second.getY())) {
                    return true;
                }
            }
            if(first.getY() == second.getY() && stop.getCoords().getY() == first.getY()) {
                if(isBetween(stop.getCoords().getX(), first.getX(), second.getX())) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Přidá do ulice zastávku, ale pouze za předpokladu, že zastávky souřadnicemi
     * leží přímo na ulici. Pokud zastávka neodpovídá ulici, vrátí false.
     * @param stop Přidávaný zastávka.
     * @return Booleovská hodnota úspěšnosti vložení zastávky na ulici.
     */

    public boolean addStop(Stop stop) {
        if(this.isOnStreet(stop)) {
            this.stops.add((MyStop)stop);
            stop.setStreet(this);
            return true;
        }
        return false;
    }

    /**
     * Změní hustotu provozu na ulici na danou hodnotu.
     * @param t Nová hustota provozu.
     */

    public void setTrafic(double t) {
        this.trafic = t;
    }

    /**
     * Získá jméno ulice.
     * @return Jméno ulice.
     */

    public String getId() {
        return this.id;
    }

    /**
     * Vrátí hustotu provozu na ulici.
     * @return Hustota provozu na ulici.
     */
    @JsonIgnore
    public double getTrafic() {
        return this.trafic;
    }

    /**
     * Získá veškeré (seznam) koordináty, které definují ulici.
     * @return Seznam koordinátů definujících ulici.
     */
    @JsonIgnore
    public List<MyCoordinate> getCoordinates() {
        return this.coords;
    }

    public List<MyCoordinate> getCoords() {return this.coords;}

    /**
     * Získá veškeré zastávky (seznam), které se nachází na ulici.
     * @return Seznam zastávek na ulici.
     */
    public List<MyStop> getStops() {
        return this.stops;
    }

    public void setStops(List<MyStop> stops) {
        this.stops = stops;
    }

    /**
     * Získá koordináty počátku ulice.
     * @return Počátek ulice (koordináty).
     */

    public MyCoordinate begin() {
        return this.coords.get(0);
    }

    /**
     * Získá koordináty konce ulice.
     * @return Konec ulice (koordináty).
     */

    public MyCoordinate end() {
        return this.coords.get(this.coords.size() - 1);
    }

    /**
     * Kontroluje, zda na sebe dvě ulice navazují. Ulice na sebe můžou navazovat
     * pouze koncem, nebo začátkem.
     * @param s Navazující ulice.
     * @return Booleovská hodnota návaznosti ulic.
     */

    public boolean follows(MyStreet s) {
        return (this.begin().equals(s.begin()) || this.begin().equals(s.end()) ||
                this.end().equals(s.end()) || this.end().equals(s.begin()));
    }

    /**
     * Získá směr, kterým se po nově započaté ulici pohybuje.
     * <STRONG>Metoda se může použít pouze při příjezdu na novou cestu, jinak nefunguje.</STRONG>
     * @param pos Pozice nájezdu na ulici.
     * @return True v případě kladném pohybu po ulici. Index v poli koordinátů definujících
     * ulici roste. False v případě záporného pohybu po ulici. Index v poli koordinátů
     * ulice klesá.
     */
    @JsonIgnore
    public boolean getDirection(Coordinate pos) {
        return pos.equals(this.begin());
    }

    /**
     * Posune zadaný koordinát směrem k dalšímu bodu definujícímu ulici, v závislovti
     * na povolené délce, kterou může urazit. Pokud dospěje k cíli a ještě zbyde možnost
     * pro cestování, vrátí tuto hodnotu, jinak vrátí 0.
     * @param c Koordináty, které se budou aktualizovat.
     * @param next Index dalšího bodu na ulici.
     * @param driveLen Možná maximální urazitelná dálka.
     * @return Zbytek neuražené "energie" (možné urazitelné délky).
     */
    private double goToNextCoordinate(Coordinate c, double next, double driveLen) {

        // Pokud již jsem na konci ulice, nikam nejedeme.
        if(next < 0 || next == this.coords.size()) {
            return driveLen;
        }

        // Získání následujícího chackpointu (cíl)
        Coordinate b = this.coords.get((int) next);

        // Pohyb ve směru Y
        if(c.getX() == b.getX()) {
            // Je energie větší než urazitelná vzdálenost?
            if(c.diffY(b) >= driveLen) {
                if(c.getY() <= b.getY()) {
                    // Pohyb nahoru
                    c.setY(c.getY() + driveLen);
                }
                else {
                    // Pohyb dolů
                    c.setY((c.getY() - driveLen));
                }
                return 0;
            }
            else {
                // Energie je příliš velká, rovnou se posuneme na konec ulice.
                double toRet = driveLen - c.diffY(b);
                c.setY(b.getY());
                return toRet;
            }
        }
        else {
            //Pohyb ve směru X
            if(c.diffX(b) >= driveLen) {
                // Je energie větší než urazitelná vzdálenost?
                if(c.getX() <= b.getX()) {
                    // Pohyb vpravo
                    c.setX(c.getX() + driveLen);
                }
                else {
                    // Pohyb vlevo
                    c.setX((c.getX() - driveLen));
                }
                return 0;
            }
            else {
                // Energie je příliš velká, rovnou se posuneme na konec ulice.
                double toRet = driveLen - c.diffX(b);
                c.setX(b.getX());
                return toRet;
            }
        }
    }

    /**
     * Provede pusun o uvedený počet jednotek po ulici, vrátí zbylý počet neujetých jednotek.
     * Pokud tedy dojede na konec a ještě zbývají jednotky pro jízdu, vrátí jejich počet.
     * Změní pozici na novou.
     * @param pos Pozice, která bude změněna na novou posunem po ulici.
     * @param chackpoint Index posledního projetého zlomu na ulici (bodu definujícího ulici).
     * @param driveLen Počet jednotek k ujetí.
     * @param dir Směr posuvu po ulici.
     * @return Počet zbylých neujetých jednotek.
     */

    public double driveStreet(Coordinate pos, Chackpoint chackpoint, double driveLen, boolean dir) {
        // Dokud se nedojede na konec ulice nebo nevyčerpá energii, jede dál.
        while(chackpoint.get() >= 0 && chackpoint.get() < this.coords.size()) {
            // Zahájí jízdu a získá nevyužitou energii.
            driveLen = goToNextCoordinate(pos, (dir) ? (chackpoint.get()+1) : (chackpoint.get()-1), driveLen);
            // Pokud vše ujel, konec.
            if(driveLen == 0) {
                return 0;
            }
            // Pokud ještě zbívá anergie, posuneme chackpoint ve směru pohybu po ulici.
            if(dir) {
                chackpoint.inc();
            }
            else
            {
                chackpoint.dec();
            }
        }

        // Pokud je projeta celá ulice, ale ještě zbývá energie, bude navrácená.
        return driveLen;
    }

    /**
     * Zkontroluje, zda koordinát zastávky nachází v ulici před zadaným koordinátem.
     * @param c Aktuální pozice.
     * @param b Zkoumaná pozice.
     * @param dir Směr pohybu po ulici.
     * @return Výsledek existejce zastávky před (dopředu) aktuální pozicí.
     */
    public boolean isAhead(Coordinate c, Coordinate b, boolean dir) {
        if(dir) {
            for(int i = (dir) ? 1 : this.coords.size()-2, j = (dir) ? (i-1) : (i+1);
                (dir) ? (i < this.coords.size()) : (i >= 0 );
                i += (dir) ? (1) : (-1), j += (dir) ? (1) : (-1)) {

                if(this.coords.get(j).getY() == this.coords.get(i).getY()) {
                    if(isBetween(c.getX(), this.coords.get(j).getX(), this.coords.get(i).getX())) {
                        if(isBetween(b.getX(), this.coords.get(j).getX(), this.coords.get(i).getX())) {
                            if(this.coords.get(j).getX() < this.coords.get(i).getX()) {
                                if(b.getX() > c.getX()) {
                                    return true;
                                }
                                else {
                                    return false;
                                }
                            }
                        }
                        else {
                            return true;
                        }
                    }
                    else if(isBetween(b.getX(), this.coords.get(j).getX(), this.coords.get(i).getX())) {
                        return false;
                    }
                }
                else {
                    if(isBetween(c.getY(), this.coords.get(j).getY(), this.coords.get(i).getY())) {
                        if(isBetween(b.getY(), this.coords.get(j).getY(), this.coords.get(i).getY())) {
                            if(this.coords.get(j).getY() < this.coords.get(i).getY()) {
                                if(b.getY() > c.getY()) {
                                    return true;
                                }
                                else {
                                    return false;
                                }
                            }
                        }
                        else {
                            return true;
                        }
                    }
                    else if(isBetween(b.getY(), this.coords.get(j).getY(), this.coords.get(i).getY())) {
                        return false;
                    }
                }
            }
        }
        return false;
    }



    /**
     * Kontroluje ekvivalenci dvou ulic, dvě ulice jsou stejné, pokud mají shodné jméno.
     * @param o Druhá ulice k porovnání.
     * @return Booleovská hodnota ekvivalence dvou ulic.
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        MyStreet myStreet = (MyStreet) o;
        return Objects.equals(id, myStreet.id);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }




    private void streetToStops() {
        for(MyStop stop: this.stops) {
            stop.setStreet(this);
        }
    }

    @Override
    public void update(boolean simulation, boolean disabledStop) throws InterruptedException {

    }

    static class streetCtor extends StdConverter<MyStreet, MyStreet> {
        @Override
        public MyStreet convert(MyStreet myStreet) {
            myStreet.streetToStops();
            return myStreet;
        }
    }
}
