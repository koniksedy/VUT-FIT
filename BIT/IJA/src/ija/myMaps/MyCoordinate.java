package ija.myMaps;
import com.fasterxml.jackson.annotation.*;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import ija.maps.Coordinate;


import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import static java.lang.Math.abs;

/**
 * Implementace rozhraní Coordinate (souřadnic na mapě).
 * Coordináty obsahují X-ovou a Y-ovou složku.
 * Dva koordináta jsou totožné v případě shodnosti X a Y.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
public class MyCoordinate implements Coordinate {
    /**
     * X-ová souřadnice.
     */
    private double x;
    /**
     * Y-ová souřadníce.
     */
    private double y;


    public MyCoordinate(){}

    /**
     * Vrátí X-ovou souřadnici.
     * @return X-ová souřadnice.
     */
    @Override
    public double getX() {
        return this.x;
    }

    /**
     * Vrátí Y-ovou souřadnici.
     * @return Y-ová soluřadnice.
     */
    @Override
    public double getY() {
        return this.y;
    }

    /**
     * Nastavní X-ovou složku koordinátu na zadanou hodnotu.
     * @param x Hodnota, na kterou bude X-ová složka koordinátu nastavena.
     */
    @Override
    public void setX(double x) {
        this.x = x;
    }

    @JsonIgnore
    public double getDistance(Coordinate b) {
        return Math.sqrt(Math.pow((this.getX() - b.getX()), 2) + Math.pow(this.getY() - b.getY(), 2));
    }

    /**
     * Nastavní Y-ovou složku koordinátu na zadanou hodnotu.
     * @param y Hodnota, na kterou bude Y-ová složka koordinátu nastavena.
     */

    @Override
    public void setY(double y) {
        this.y = y;
    }

    /**
     * Vrátí rozdíl X-ových souřadnic dvou koordinátů.
     * @param c Druhý oběktk koordinátů.
     * @return Absolutní rozdíl X-ových souřadnic.
     */

    @Override
    public double diffX(Coordinate c) {
        return abs(this.x - c.getX());
    }

    /**
     * Vrátí absolutní rozdíl Y-ových souřadnic dvou koordinátů.
     * @param c Druhý oběktk koordinátů.
     * @return Absolutní rozdíl Y-ových souřadnic.
     */

    @Override
    public double diffY(Coordinate c) {
        return abs(this.y - c.getY());
    }

    /**
     * Porovná, zda leží dva koordináty rovnoběžně s osou X nebo Y,
     * tedy svírajíhel.
     * @param c Druhý koordinát k porovnání.
     * @return Booleovská hodnota shodnosti X-ových, nebo Y-ových souřadnic.
     */

    @Override
    public boolean isOrtogonal(Coordinate c) {
        return this.x == c.getX() || this.y == c.getY();
    }

    /**
     * Dva koordináty jsou stejné, pokud se rovnají jejich X-ové a Y-ové složky.
     * @param o Druhý koordinát k porovnání.
     * @return Booleovská hodnota rovnosti dvou koordinátů.
     */

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        MyCoordinate that = (MyCoordinate) o;
        return x == that.x &&
                y == that.y;
    }

    @Override
    public int hashCode() {
        return Objects.hash(x, y);
    }
}
