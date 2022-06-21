package ija;

import ija.maps.Coordinate;
import ija.myDraw.Drawable;
import javafx.scene.shape.Line;
import javafx.scene.shape.Shape;
import javafx.scene.text.Text;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static jdk.nashorn.internal.objects.NativeMath.min;


/**
 * Vytvoření ulice.
 */
public class Street implements Drawable {
    private Coordinate start;
    private Coordinate stop;
    private String name;




    public Street(String name, Coordinate start, Coordinate stop){
        this.start = start;
        this.stop = stop;
        this.name = name;


    }

    /**
     * Získání ulice z GUI a nastavení textu.
     * @return
     */
    @Override
    public List<Shape> getGui() {
        Text jmeno;
        if(this.start.getY() == this.stop.getY()) {
            jmeno = new Text(min(this.start.getX(), this.stop.getX()) + (this.start.diffX(this.stop) / 2), this.start.getY() + 15, this.name);

        } else {
            jmeno = new Text(this.start.getX() + 15, this.start.getY() + this.start.diffY(this.stop) / 2, this.name);
        }
        return Arrays.asList(jmeno, new Line(this.start.getX(), this.start.getY(), this.stop.getX(), this.stop.getY()));
    }






}
