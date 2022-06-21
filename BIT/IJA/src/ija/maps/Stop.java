package ija.maps;

import ija.myMaps.MyCoordinate;
import ija.myMaps.MyStreet;

import java.lang.String;

/**
 * Rozhraní zastávky.
 * Každá zastávky je identifikována svým jménem, nachází se na ulici
 * a má přidělemé koordináty na mapě.
 * Zastávka je jednoznačně identifikovaná svým jménem.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */
public interface Stop {
   /**
     * Vrátí jméno zastávky.
     * @return Jméno zastávky.
     */
    String getId();

    /**
     * Vrátí koordináty zastávky na mapě.
     * @return Koordináty zastávky.
     */
    Coordinate getCoords();

    void setId(String id);

    void setCoords(MyCoordinate coords);

    /**
     * Nastaví jméno ulice na které se nachází zastávka.
     * <STRONG>Nepřidělje zastávku do ulice,</STRONG> nutno udělat ručně.
     * Koordináty zastávky musí souhlasit s pozicí na ulici. Pokud ne, je navráceno false.
     * @param s Ulice na kterou bude přidána zastávka.
     * @return Úspěšch vložení ulice do zastávky.
     */
    boolean setStreet(MyStreet s);

    /**
     * Vrátí ulici na které se nachází zastávka, může být null.
     * @return Ulice na které se nachází zastávka.
     */
    MyStreet getStreet();
}
