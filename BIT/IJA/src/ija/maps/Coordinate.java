package ija.maps;

import com.fasterxml.jackson.annotation.JacksonAnnotation;
import com.fasterxml.jackson.annotation.JsonCreator;
import ija.myMaps.MyCoordinate;

/**
 * Rozhraní Coordinate (souřadnic na mapě).
 * Coordináty obsahují X-ovou a Y-ovou složku.
 * Dva koordináta jsou totožné v případě shodnosti X a Y.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
public interface Coordinate {

    /**
     * Vytvoří oběkt koordináty. Všechny koordináty musí být kladné.
     * Pokud je některý z nich záporný, vrátí operace null.
     * @param x X-ová souřadnice.
     * @param y Y-ová souřadnice.
     * @return Booleovská hodnota úspěšnosti vytvoření koordinát.
     */
    static Coordinate createCoordinate(double x, double y) {
        if (x < 0 || y < 0) {
            return null;
        }
        MyCoordinate c = new MyCoordinate();
        c.setX(x);
        c.setY(y);
        return c;
    }

    /**
     * Vrátí X-ovou souřadnici.
     * @return X-ová souřadnice.
     */
    double getX();

    /**
     * Vrátí Y-ovou souřadnici.
     * @return Y-ová soluřadnice.
     */
    double getY();

    double getDistance(Coordinate b);

    /**
     * Nastavní X-ovou složku koordinátu na zadanou hodnotu.
     * @param x Hodnota, na kterou bude X-ová složka koordinátu nastavena.
     */
    void setX(double x);

    /**
     * Nastavní Y-ovou složku koordinátu na zadanou hodnotu.
     * @param y Hodnota, na kterou bude Y-ová složka koordinátu nastavena.
     */
    void setY(double y);

    /**
     * Vrátí absolutní rozdíl X-ových souřadnic dvou koordinátů.
     * @param c Druhý oběktk koordinátů.
     * @return Absolutní rozdíl X-ových souřadnic.
     */
    double diffX(Coordinate c);

    /**
     * Vrátí absolutní rozdíl Y-ových souřadnic dvou koordinátů.
     * @param c Druhý oběktk koordinátů.
     * @return Absolutní rozdíl Y-ových souřadnic.
     */
    double diffY(Coordinate c);

    /**
     * Porovná, zda leží dva koordináty rovnoběžně s osou X nebo Y,
     * thel.
     * @param c Druhý koordinát k porovnání.
     * @return Booleovská hodnota shodnosti X-ových, nebo Y-ových souřadnic.
     */
    boolean isOrtogonal(Coordinate c);
}
