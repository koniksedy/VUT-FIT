package ija;

import java.time.LocalTime;

/**
 * Implementace rozhraní pro aktualizovatelné položky.
 * @author Michal Šedý
 * @author Ondřej Pavlacký
 */

public interface Time_update {
   void update(boolean simulation, boolean disabledStop) throws InterruptedException;

}
