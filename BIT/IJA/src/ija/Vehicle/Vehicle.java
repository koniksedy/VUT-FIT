package ija.Vehicle;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.JsonSerializer;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.util.Converter;
import com.fasterxml.jackson.databind.util.StdConverter;
import ija.Main_controller;
import ija.Path;
import ija.Time_update;
import ija.maps.Coordinate;
import ija.maps.Stop;
import ija.myDraw.Drawable;
import ija.myMaps.MyCoordinate;
import ija.myMaps.MyStop;
import ija.myMaps.MyStreet;
import ija.myTraffic.MyLine;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Shape;

import java.io.IOException;
import java.time.LocalDate;
import java.time.LocalTime;
import java.util.*;

import static ija.Main_controller.actualTime;
import static java.lang.Integer.min;

@JsonDeserialize(converter = Vehicle.vehicleCtor.class)
@JsonIdentityInfo(generator = ObjectIdGenerators.PropertyGenerator.class, property = "id")

/**
 * Implementace vozidla a jeho FX modelu
 * V tomto modulu je implementováno vytvoření vozidla
 * výpočet jeho součadnice a také přiřazení jednotlivých
 * prvků.
 *  @author Michal Šedý
 *  @author Ondřej Pavlacký
 */
public class Vehicle implements Drawable, Time_update {
    private String id;
    public MyCoordinate position;
    @JsonIgnore
    private double speed = 1;
    private Path path;
    @JsonIgnore
    private double distance = 0;
    @JsonIgnore
    private List<Shape> gui;
    @JsonIgnore
    private boolean pitStop = false;
    @JsonIgnore
    private boolean forward = true;
    @JsonIgnore
    private int streetIdx = 0;
    @JsonIgnore
    private MyLine line;
    private List<MyStreet> streets = new ArrayList<>();
    @JsonIgnore
    private LocalTime pitStopTime;
    @JsonIgnore
    private MyCoordinate home;
    private int pendelTimes;

    @JsonIgnore
    private boolean lastDir;

    @JsonIgnore
    private int actualPendel;

    @JsonIgnore
    private List<List<LocalTime>> timeTable = new ArrayList<>();

    @JsonDeserialize(using = Vehicle.LocalDateConverter.class)
    private LocalTime startTime;
    @JsonIgnore
    private LocalTime endTime;

    private List<MyStop> stops = new ArrayList<>();
    @JsonIgnore
    public static Vehicle lastBusClicked;
    @JsonIgnore
    public Main_controller controller;

    @JsonIgnore
    private int delay = 0;
    @JsonIgnore
    private boolean lost = false;

    private Vehicle(){}

    /**
     * Funkce která zjišťuje delay vozidla.
     * @return delay - zpozdeni vozidla
     */
    @JsonIgnore
    public int getDelay() {
        return delay;
    }

    /**
     * Nastavuije delay vozidla
     * @param delay zpozdeni vozidla
     */
    @JsonIgnore
    public void setDelay(int delay) {
        this.delay = delay;
    }

    /**
     * Přidání dalšího zpoždění k aktuálnímu.
     * @param delay - zpoždění
     */
    public void addDelay(int delay){
        this.delay += delay;
    }


    /**
     * Získání startovního času
     * @return startovní čas
     */
    public LocalTime getStartTime() {
        return startTime;
    }

    /**
     * Nastavení startovního času.
     * @param startTime
     */
    public void setStartTime(LocalTime startTime) {
        this.startTime = startTime;
    }

    /**
     * Získání konečného času.
     * @return
     */
    @JsonIgnore
    public LocalTime getEndTime() {
        return endTime;
    }


    /**
     * Nastavit koncový čas.
     * @param endTime
     */
    @JsonIgnore
    public void setEndTime(LocalTime endTime) {
        this.endTime = endTime;
    }


    /**
     * Získání linky.
     * @return
     */
    public MyLine getLine() {
        return line;
    }

    /**
     * Nastavení linky.
     * @param line
     */
    public void setLine(MyLine line) {
        this.line = line;
    }

    /**
     * Nastavení controlleru.
     * @param controller
     */
    @JsonIgnore
    public void setController(Main_controller controller) {
        this.controller = controller;
    }

    /**
     * Získání o kolikátou jízdu se jedná.
     */
    @JsonIgnore
    public int getActualPendel() {
        return actualPendel;
    }

    /**
     * Zvýšení jízdy.
     */
    public void incActualPendel() {
        this.actualPendel += 1;
    }

    /**
     * Získání id vozidla.
     * @return
     */
    public String getId() {
        return id;
    }

    /**
     * Získání pozice vozidla.
     * @return pozice vozidla
     */
    public MyCoordinate getPosition() {
        return position;
    }


    /**
     * Získání cesty vozidla.
     * @return cesta
     */
    public Path getPath() {
        return path;
    }

    /**
     * Získání seznamu lic z vozidla.
     * @return seznam vozidel
     */
    public List<MyStreet> getStreets() {
        return streets;
    }

    /**
     * Získání seznamu zastávek z vozidla.
     * @return
     */
    public List<MyStop> getStops() {
        return stops;
    }


    /**
     * Nastavení id vozidla
     * @param id
     */
    public void setId(String id) {
        this.id = id;
    }

    /**
     * Nastavení pozice vozidla
     * @param position
     */
    public void setPosition(MyCoordinate position) {
        this.position = position;
    }

    /**
     * Nastavení cesta vozidla.
     * @param path
     */
    public void setPath(Path path) {
        this.path = path;
    }

    /**
     * Nastavení seznamu ulic.
     */
    public void setStreets(List<MyStreet> streets) {
        this.streets = streets;
    }

    /**
     * Nastavení seznamu zastávek.
     * @param stops
     */
    public void setStops(List<MyStop> stops) {
        this.stops = stops;
    }


    /**
     * Získání aktuálního jízdního řádu.
     * @return
     */
    public List<LocalTime> getActualTimetable(){
        return this.timeTable.get(this.actualPendel);
    }

    /**
     * Přidání zastávky do seznamu.
     * @param stop
     */
    public void addStop(Stop stop) {
        this.stops.add((MyStop)stop);
    }

    /**
     * Přidání ulice do seznamu.
     * @param street
     */
    public void addStreet(MyStreet street) {
        this.streets.add(street);
    }

    /**
     * Získání času jízdy.
     * @return
     */
    public int getPendelTimes() {
        return pendelTimes;
    }

    /**
     * Nastavení času jízdy.
     * @param pendelTimes
     */
    public void setPendelTimes(int pendelTimes) {
        this.pendelTimes = pendelTimes;
    }

    /**
     * Nastavení FX modelu pro vykreslení.
     * @param coordinate
     */
    private void modeGUI(Coordinate coordinate) {
        for (Shape shape : gui) {
            shape.setTranslateX((coordinate.getX() - position.getX()) + shape.getTranslateX());
            shape.setTranslateY((coordinate.getY() - position.getY()) + shape.getTranslateY());
        }
    }

    /**
     * Načteme si autobus z GUI
     * @return
     */
    @JsonIgnore
    @Override
    public List<Shape> getGui() {
        this.gui.get(0).setOnMouseClicked(e -> highlight());
        return this.gui;
    }


    /**
     * Zvýraznění linky po kliknutí.
     */
    public void highlight() {
        unhighlight();
        lastBusClicked = this;
        controller.setBus_name();
        controller.setLineSelector(this.line.getId());
        controller.onLineChange();
        this.gui.get(0).setFill(Color.GREEN);
        this.controller.setText_delay(Vehicle.lastBusClicked);

    }

    /**
     * Zrušení zvýraznění.
     */
    public void unhighlight() {

        if(lastBusClicked != null) {
            MyStreet street = lastBusClicked.streets.get(0);
            controller.unsetBus_name();
            controller.setLineSelector("None");
            controller.onLineChange();
            lastBusClicked.gui.get(0).setFill(Color.BLUE);
            lastBusClicked = null;
            street.unhighlight();
            this.controller.timeDelayString.setText("0 min 0 sec");
        }

    }

    /**
     * Nastavení pokud je ztracen.
     */
    @JsonIgnore
    public void setLost(boolean l){
        this.lost = l;
    }

    /**
     * Získání id ulice na kterí autobus je.
     * @return
     */
    public int getStreetIdx() {
        return streetIdx;
    }


    /**
     * Zresetování ulic po kterých autobus jezdí.
     * @param closedStreets
     * @param openStreets
     */
    public void resetBusStreets(List<MyStreet> closedStreets, List<MyStreet> openStreets){

        double lenClosed = 0;
        for(MyStreet street: closedStreets){
            lenClosed += street.begin().getDistance(street.end());
        }
        double lenOpen = 0;
        for(MyStreet street: openStreets){
            lenOpen += street.begin().getDistance(street.end());
        }
        this.delay += (lenOpen-lenClosed)/this.speed;

        // Nalazení nejnižšího indexu z this.streets podle ulic v closed
        int minIdx = this.streets.size()-1;
        for(MyStreet street: closedStreets){
            if(this.streets.indexOf(street) > 0 && this.streets.indexOf(street) < minIdx){
                minIdx = this.streets.indexOf(street);
            }
        }

        // Seřazení open
        List<MyStreet> sortedOpen = new ArrayList<>();
        MyStreet lastStreet = this.streets.get(minIdx);
        for(int i = 0; i < openStreets.size(); i++){
            for(MyStreet streetNext: openStreets){
                if(sortedOpen.contains(streetNext)){
                    continue;
                }
                if(lastStreet.follows(streetNext)){
                    sortedOpen.add(streetNext);
                    lastStreet = streetNext;
                    break;
                }
            }
        }

        //Přidání linky do nových ulic
        for(MyStreet street: openStreets){
            if(!street.getLinesOnStreet().contains(this.line)){
                street.addLine(this.line);
            }
        }

        //Získání staré ulice
        List<MyStreet> oldStreets = new ArrayList<>();
        oldStreets.addAll(this.streets);

        //Mazání
        this.streets.removeAll(closedStreets);
        this.streets.addAll(minIdx, sortedOpen);

        //Kalkulace streetIndex
        if(this.streetIdx >= minIdx && this.streetIdx <= minIdx + closedStreets.size()-1) {
            if (this.forward) {
                this.streetIdx = minIdx + openStreets.size() - closedStreets.size();
            } else {
                this.streetIdx = minIdx;
            }
        }
        else if(this.streetIdx >  minIdx + closedStreets.size()-1){
            this.streetIdx = this.streetIdx + openStreets.size() - closedStreets.size();
        }


        List<MyStreet> newStreets = new ArrayList<>();
        for(MyStreet street: this.streets){
            newStreets.add(street);
        }
        if(!oldStreets.equals(newStreets)) {
            List<MyStreet> reverseStreets = new ArrayList<>();
            reverseStreets.addAll(this.line.getStreets());
            Collections.reverse(reverseStreets);
            if(!newStreets.equals(this.line.getStreets()) && !newStreets.equals(reverseStreets)) {
                this.line.setBackUpStreets();
            }
        }
        this.line.setStreets(newStreets);
    }

    /**
     * Resetování jednotlivých vozidel.
     */
    public void resetVehicle(){

        this.actualPendel = 0;
        this.position.setX(this.home.getX());
        this.position.setY(this.home.getY());
        this.pitStop = false;

        this.getGui().get(0).setFill(Color.TRANSPARENT);
        this.getGui().get(0).setMouseTransparent(true);
        this.getGui().get(0).setTranslateX(0);
        this.getGui().get(0).setTranslateY(0);

        this.streetIdx = 0;

        if(home.equals(streets.get(0).begin()) || home.equals(streets.get(0).end())){

            this.distance = 0;
            this.forward = true;

        }
        else{
            this.streetIdx = this.streets.size() - 1;
            this.forward = false;
        }

        List<MyCoordinate> tmp = new ArrayList<>();
        if(home.equals((streets.get(this.streetIdx).begin()))) {
            tmp.add(this.streets.get(this.streetIdx).begin());
            tmp.add(this.streets.get(this.streetIdx).end());
        }
        else{
            tmp.add(this.streets.get(this.streetIdx).end());
            tmp.add(this.streets.get(this.streetIdx).begin());

        }
        this.path.setPath(tmp);
    }


    /**
     * Pokud jedeme dopředu.
     * @return
     */
    @JsonIgnore
    public boolean isForward() {
        return forward;
    }

    /**
     * Nastavení toho, že jedeme dopředu.
     * @param forward
     */
    @JsonIgnore
    public void setForward(boolean forward) {
        this.forward = forward;
    }

    /**
     * V této funkci překresluveme autobus na scéně.
     * @param simulation
     * @param disabledStop
     * @throws InterruptedException
     */
    public void update(boolean simulation, boolean disabledStop) throws InterruptedException {

        MyStreet lostStreet = null;
        controller.setActualTime();
        this.setLastDir(this.forward);
        boolean oldForward = this.forward;
        int oldstreetIdx = this.streetIdx;
        double rest = 1;
        while(rest != 0) {
            rest = 0;
            int tmpStreetIdx = this.streetIdx;
            if (this.distance > this.path.getPathSize()) {
                rest = this.distance - this.path.getPathSize();
                if (this.forward) {
                    // Linka jede dopředu
                    if (this.streetIdx == this.streets.size() - 1) {
                        // Je potřeba se otočit
                        this.forward = false;
                        this.incActualPendel();
                        // jedeme po stejné cestě zpět
                    } else {
                        // Pokračujeme v jízdě dopředu
                        this.streetIdx += 1;
                    }

                } else {
                    // Linka jede dozadu
                    if (this.streetIdx == 0) {
                        // Je potřeba se otočit
                        this.forward = true;
                        this.incActualPendel();
                        // Jedeme v otočeném spěru ještě jednou po stejné ulici
                    } else {
                        // Pokračujeme v jízdě zpět.
                        this.streetIdx -= 1;
                    }
                }

                // Nastavení startu a konce nové cesty
                if (this.path.path.get(1).equals(this.streets.get(this.streetIdx).end()) && !this.lost) {
                    // Ulici jedeme od konce k začátku
                    this.path.path.set(0, this.streets.get(this.streetIdx).end());
                    this.path.path.set(1, this.streets.get(this.streetIdx).begin());
                } else if (this.path.path.get(1).equals(this.streets.get(this.streetIdx).begin()) && !this.lost) {
                    // Ulici jedeme od začátku ke konci
                    this.path.path.set(1, this.streets.get(this.streetIdx).end());
                    this.path.path.set(0, this.streets.get(this.streetIdx).begin());
                } else {
                    this.lost = true;
                    this.streetIdx = oldstreetIdx;
                    this.forward = oldForward;
                    //Pokusí se nalézt, zda daná cesta již neexistuje v ulicích linky

                    //Musí se naleznout cesta ze záloh
                    for(MyStreet street: this.line.getBackUpStreets()){
                        if(this.path.path.get(1).equals(street.begin()) && !this.path.path.get(0).equals(street.end())) {
                            this.path.path.set(0, street.begin());
                            this.path.path.set(1, street.end());
                            lostStreet = street;
                            break;
                        }
                        else if(this.path.path.get(1).equals(street.end()) && !this.path.path.get(0).equals(street.begin())){
                            this.path.path.set(0, street.end());
                            this.path.path.set(1, street.begin());
                            lostStreet = street;
                            break;
                        }
                    }
                    // Podíveá se, zda již nenejel na otevřenou cestu
                    for (MyStreet street: this.streets){
                        if((street.begin().equals(this.path.path.get(0)) && street.end().equals(this.path.path.get(1)))
                           || (street.begin().equals(this.path.path.get(1)) && street.end().equals(this.path.path.get(0)))){
                            this.streetIdx = this.streets.indexOf(street);
                            this.lost = false;
                        }
                    }
                }

                // Proč this.distance = speed? Protože vstupní podmínka přejezdu na novou ulici je
                // this.distance > this.path.getPathSize(),
                // tedy autobus již měl být o jedno speed dále, na nové ulici.
                // Zároveň nemůže být return, protože je potřeba udělat pohyb kupředu
                // v tomhle aktualizačním kroku.
                this.distance = rest;
            }

            // Pokud ulice ještě pokračuje, jedu dál.
            if(!disabledStop) {
                for (Stop zastavka : this.stops) {
                    if (zastavka.getCoords().equals(this.position)) {
                        long test = System.currentTimeMillis();
                        if (!this.pitStop) {
                            this.pitStop = true;
                            this.pitStopTime = actualTime.plusSeconds(20);
                        }
                        if (actualTime.isBefore(pitStopTime)) {
                            if (!simulation) {
                                return;
                            }
                        } else {
                            this.pitStop = false;
                        }
                    }
                }
            }

        }
        Coordinate coords = path.getCordByDistance(distance);
        if(coords != null) {
            modeGUI(coords);
            position = (MyCoordinate) coords;
        }

        if(this.lost && lostStreet != null) {
            distance += speed / lostStreet.getTrafic();
            this.addDelay((int) (lostStreet.getTrafic()-1));
        }
        else{
            distance += speed / this.streets.get(this.streetIdx).getTrafic();
            this.addDelay((int) (this.streets.get(this.streetIdx).getTrafic()-1));
        }
    }

    @JsonIgnore
    public MyCoordinate getHome(){
        return this.home;
    }

    /**
     * Zeptání se na poslední adresu
     * @return
     */
    @JsonIgnore
    public boolean isLastDir() {
        return lastDir;
    }

    /**
     * Nastavení poslední adresy.
     * @param lastDir
     */
    @JsonIgnore
    public void setLastDir(boolean lastDir) {
        this.lastDir = lastDir;
    }

    /**
     * Přidání vzdálenosti.
     * @param d
     */
    @JsonIgnore
    public void addDistance(double d){
        boolean disabledStop = false;
        int stopsCnt = 0;
        while(d > 0){
            d -= 1;

            try {
                this.update(true, disabledStop);
            } catch (InterruptedException e){

            }
            disabledStop = false;

            if(this.pitStop == true){
                this.pitStop = false;
                //this.distance -= this.speed;
                disabledStop = true;
                stopsCnt++;
                d -= 20;
            }
        }
    }

    /**
     * Přidání prvku do GUI a jeho vykreslení.
     */
    private void guiMaker()
    {
        this.gui = new ArrayList<>();
        this.gui.add(new Circle(position.getX(), position.getY(), 8, Color.TRANSPARENT));
        // Dá se kliknou přez (pod) něj.
        this.gui.get(0).setMouseTransparent(true);
        this.home = new MyCoordinate();
        this.home.setX(this.position.getX());
        this.home.setY(this.position.getY());

        LocalTime tmpTime = LocalTime.of(this.startTime.getHour(), this.startTime.getMinute(), this.startTime.getSecond());
        // Výpočet časů odjezdů
        tmpTime = tmpTime.plusSeconds(20);

        // Nevím proč, ale spuští se to dvakrát, napodruhé to zakážu
        if(this.timeTable.size() < pendelTimes) {
            for (int i = 0; i < pendelTimes; i++) {
                List<LocalTime> stopTime = new ArrayList<>();
                stopTime.add(LocalTime.of(tmpTime.getHour(), tmpTime.getMinute(), tmpTime.getSecond()));
                if(i % 2 == 0) {
                    for (int j = 1; j < this.stops.size(); j++) {
                        tmpTime = tmpTime.plusSeconds((long) (20 + (this.stops.get(j - 1).getDistance(this.stops.get(j), this.streets) / this.speed)));
                        stopTime.add(LocalTime.of(tmpTime.getHour(), tmpTime.getMinute(), tmpTime.getSecond()));
                    }
                } else {
                    for (int j = this.stops.size()-1; j > 0; j--) {
                        tmpTime = tmpTime.plusSeconds((long) (20 + (this.stops.get(j - 1).getDistance(this.stops.get(j), this.streets) / this.speed)));
                        stopTime.add(LocalTime.of(tmpTime.getHour(), tmpTime.getMinute(), tmpTime.getSecond()));
                    }
                    Collections.reverse(stopTime);
                }
                this.timeTable.add(stopTime);
            }

            int totalLen = 0;
            for (MyStreet street : this.streets) {
                totalLen += street.begin().getDistance(street.end());
            }
            this.endTime = LocalTime.of(this.startTime.getHour(), this.startTime.getMinute());
            this.endTime = this.endTime.plusSeconds((long) (this.pendelTimes * (20 * this.stops.size() + (totalLen / this.speed))) + 2);
        }

    }

    /**
     * Získání aktuálního jízdního řádu.
     * @param t
     * @return
     */
    public List<LocalTime> getActualTimeTable(LocalTime t){
        int i = 0;
        for(List<LocalTime> timeList: this.timeTable){
            if(i % 2 == 0){
                if(timeList.get(0).isBefore(t) && timeList.get(timeList.size()-1).isAfter(t)){
                    return timeList;
                }
            }
            else {
                if(timeList.get(0).isAfter(t) && timeList.get(timeList.size()-1).isBefore(t)){
                    return timeList;
                }
            }
            i++;
        }
        return null;
    }

    /**
     * Přidání jízdního řádu.
     * @param t
     */
    public void addTimeTable(List<LocalTime> t){
        this.timeTable.add(t);
    }


    /**
     * Konstruktor vektoru.
     */
    static class vehicleCtor extends StdConverter<Vehicle, Vehicle> {
        @Override
        public Vehicle convert(Vehicle vehicle) {
            vehicle.guiMaker();
            return vehicle;
        }
    }

    static class LocalDateConverter extends JsonDeserializer<LocalTime> {
        @Override
        public LocalTime deserialize(JsonParser jsonParser, DeserializationContext deserializationContext) throws IOException, JsonProcessingException {
            return LocalTime.parse(jsonParser.getText());
        }
    }
}