package ija;

import com.fasterxml.jackson.annotation.JsonIgnore;
import ija.maps.Coordinate;
import ija.myMaps.MyCoordinate;

import java.util.List;

/**
 * Implementace výpočtu
 * V tomto modulu se inicialuzijí základní části našeho projektu
 * jako je scéna, základní objekty, které jsou uloženy v listu a rozlišovány
 * na ty které se budou překreslovat a ty které jsou staicté. Také se
 * spouští funkce z modulů pro aktualizaci scény a výpočet času a
 * aktualizace jednotlivých prvků scény.
 *  @author Michal Šedý
 *  @author Ondřej Pavlacký
 */
public class Path {

    public List<MyCoordinate> path;

    public Path(){}

    public Path(List<MyCoordinate> path) {
        this.path = path;
    }

    /**
     * Získání cesty.
     * @return
     */
    public List<MyCoordinate> getPath() {
        return path;
    }

    /**
     * Nastavení cesty.
     * @param path
     */
    public void setPath(List<MyCoordinate> path) {
        this.path = path;
    }

    /**
     * Získání vzdálenosti koordinátů.
     * @param a
     * @param b
     * @return
     */
    @JsonIgnore
    private double getDistanceCord(Coordinate a, Coordinate b) {
        return Math.sqrt(Math.pow((a.getX() - b.getX()), 2) + Math.pow(a.getY() - b.getY(), 2));
    }

    @JsonIgnore
    public Coordinate getCordByDistance(double distance) {
        double length = 0;
        double distace1 = 0;
        Coordinate a = null;
        Coordinate b = null;
        Coordinate end = null;
        for (int i = 0; i < path.size() - 1; i++) {
            a = path.get(i);
            b = path.get(i + 1);
            distace1 = getDistanceCord(a, b);
            if (length + distace1 >= distance) {
                break;
            }
            length += distace1;

        }
        if (a == null || b == null) {
            return null;
        }
        double driven = ((distance - length) / distace1);

        return Coordinate.createCoordinate(a.getX() + (b.getX() - a.getX()) * driven, a.getY() + (b.getY() - a.getY()) * driven);
    }

    /**
     * Získání délky cesty.
     * @return
     */
    @JsonIgnore
    public double getPathSize() {
        double size = 0;
        for (int i = 0; i < path.size() - 1; i++) {
            size = getDistanceCord(path.get(i), path.get(i + 1));
        }
        return size;
    }
}
