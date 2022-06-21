package ija.myMaps;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.util.StdConverter;
import ija.Main_controller;
import ija.Vehicle.Vehicle;
import ija.maps.Coordinate;
import ija.myMaps.MyCoordinate;
import ija.maps.Stop;
import ija.myDraw.Drawable;
import ija.myTraffic.MyLine;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.shape.Shape;

/**
 * Implementace rozhraní zastávky.
 * Každá zastávky je identifikována svým jménem, nachází se na ulici
 * a má přidělemé koordináty na mapě.
 * Zastávka je jednoznačně identifikovaná svým jménem.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */

@JsonDeserialize(converter = MyStop.stopCtor.class)
@JsonIdentityInfo(generator = ObjectIdGenerators.PropertyGenerator.class, property = "id")
public class MyStop implements Stop, Drawable {
    @JsonIgnore
    private List<Shape> gui;
    @JsonIgnore
    public Main_controller controller;
    /**
     * Jméno zastávky.
     */
    private String id;
    /**
     * Ulice na které se zastávka nachází, může být null.
     */
    @JsonIgnore
    private MyStreet street;
    /**
     * Souřadnice zastávky na mapě.
     */

    private MyCoordinate coords;
    @JsonIgnore
    public Circle kruh = null;

    private MyStop(){}

    /**
     * Konstruktor oběktu zastávka. Přidělí zastávce název a koordináty.
     * Ulici nechá nevyplněnou.
     * @param id Jméno vytvářené zastávky.

     */
    public MyStop(String id, Coordinate position) {
        this.coords = (MyCoordinate)position;
        this.id = id;
        this.stopMaker();

    }
    public void addControler(Main_controller controller){
        this.controller = controller;
    }
    @JsonIgnore
    public static MyStop last_stop_clicked;

    public void highlight(){
        unhighlight();
        last_stop_clicked = this;
        controller.stop_text.setText(this.street.id);
        controller.stopNameString.setText(this.id);
        this.kruh.setFill(Color.GREEN);
    }

    public void unhighlight() {
        if (last_stop_clicked != null) {
            if(Vehicle.lastBusClicked == null && (Main_controller.last_clicked_line == null || (!Main_controller.last_clicked_line.getStops().contains(last_stop_clicked)))) {
                last_stop_clicked.kruh.setRadius(4);
            }
            last_stop_clicked.kruh.setFill(Color.RED);
            controller.stop_text.setText("None");
            controller.stopNameString.setText("None");
        }
    }
    /**
     * Vrátí jméno zastávky.
     * @return Jméno zastávky.
     */
    @Override
    public String getId() {
        return this.id;
    }

    /**
     * Vrátí koordináty zastávky na mapě.
     * @return Koordináty zastávky.
     */
    @Override
    public MyCoordinate getCoords() {
        return this.coords;
    }

    @Override
    public void setId(String id) {
        this.id = id;
    }

    @Override
    public void setCoords(MyCoordinate coords) {
        this.coords = coords;
    }

    /**
     * Nastaví jméno ulice na které se nachází zastávka.
     * <STRONG>Nepřidělje zastávku do ulice,</STRONG> nutno udělat ručně.
     * Koordináty zastávky musí souhlasit s pozicí na ulici. Pokud ne, je navráceno false.
     * @param s Ulice na kterou bude přidána zastávka.
     * @return Úspěšch vložení ulice do zastávky.
     */
    @JsonIgnore
    @Override
    public boolean setStreet(MyStreet s) {
        this.street = s;
        return true;
    }

    /**
     * Vrátí ulici na které se nachází zastávka, může být null.
     * @return Ulice na které se nachází zastávka.
     */
    @JsonIgnore
    @Override
    public MyStreet getStreet() {
        return this.street;
    }

    /**
     * Dvě zastávky jsou shodné, pukud se rovnají jejich názvy.
     * @param o Druhá porovnávaná zastávka.
     * @return Booleovská hodnota shodnosti dvou zastávek.
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        MyStop myStop = (MyStop) o;
        return Objects.equals(id, myStop.id);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }

    /**
     * Přepíše zastávku do syntaxe "stop(jméno)", nebo "null".
     * @return Zastávka ve formátu "stop(jméno)", nebo "null".
     */
    @Override
    public String toString() {
        if(this.id == null) {
            return "null";
        }
        return "stop(" + this.id + ")";
    }
    @JsonIgnore
    @Override
    public List<Shape> getGui() {
        return gui;
    }

    /**
     * Startovní funkce vytvářející gui pri načítání dat aplikace.
     */
    private void stopMaker() {
        this.gui = new ArrayList<>();
        Circle kruh = new Circle(this.coords.getX(), this.coords.getY(), 4, Color.RED) ;
        this.kruh = kruh;
        this.kruh.setOnMouseClicked(event -> highlight());
        this.gui.add(this.kruh);
    }

    /**
     * Vrátí vzdálenost dvou zastávek v seznamu ulic. Nadcházející zastávka se v seznamu
     * musí vždy nacházet ve smětu rostoucího indexu.
     * @param stopB Druhá zastávka pro vměření vzdálenosti.
     * @param streets Seznam ulic, na kterých se nachází zastávky.
     * @return Vzdálenost dvou zastávek.
     */
    @JsonIgnore
    public int getDistance(MyStop stopB, List<MyStreet> streets){
        // Prohledání aktuální ulice
        int idx = streets.indexOf(this.street);
        if(this.street.getStops().contains(stopB)){
            return (int) this.getCoords().getDistance(stopB.getCoords());
        }

        // Získání vzdálenosti do konce ulice s první zastávkou
        int toRet = 0;
        if(this.street.end().equals(streets.get(idx+1).begin()) || this.street.end().equals(streets.get(idx+1).end())){
            toRet += this.getCoords().getDistance(this.street.end());
        }
        else{
            toRet += this.getCoords().getDistance(this.street.begin());
        }

        // Prohledávání ostatních ulic
        for(int i = streets.indexOf(this.street)+1; i < streets.size() && i != -1; i++){
            if(streets.get(i).getStops().contains(stopB)){
                if(streets.get(i).end().equals(streets.get(idx).begin()) || streets.get(i).end().equals(streets.get(idx).end())){
                    return (int) (toRet + stopB.getCoords().getDistance(streets.get(i).end()));
                }
                else {
                    return (int) (toRet + stopB.getCoords().getDistance(streets.get(i).begin()));
                }
            }
            else{
                toRet += streets.get(i).begin().getDistance(streets.get(i).end());
            }
            idx++;
        }
        return toRet;
    }


    static class stopCtor extends StdConverter<MyStop, MyStop> {


        @Override
        public MyStop convert(MyStop myStop) {
            myStop.stopMaker();
            return myStop;
        }
    }
}
