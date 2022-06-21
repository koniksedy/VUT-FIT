package ija;

import com.sun.naming.internal.VersionHelper;
import ija.Vehicle.Vehicle;
import ija.maps.Stop;
import ija.myDraw.Drawable;
import ija.myMaps.MyStop;
import ija.myMaps.MyStreet;
import ija.myTraffic.MyLine;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.ScrollEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import java.lang.String;

import java.io.IOException;
import java.time.DateTimeException;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.lang.Integer;


/**
 * Implementace hlavního controlleru pro chod FX části.
 * V tomto modulu se řeší hlavní část FX scény a také výpis jednotlivých objektů
 * a jejich změna v čase podle funkce počítadla které mění scénu za určenou dobu.
 * V tomto modulu je upravována a přidělována funkčnost jednotlivých prvků FX scény.
 *  @author Ondřej Pavlacký
 *  @author Michal Šedý
 */
public class Main_controller {

    static public Main_controller main_controller_static;

    /**
     * Vytvoření proměnných týkajících se FX ale také vnitřního chodu programu
     */
    public String choice1;
    @FXML
    public Button pause_button;
    @FXML
    private Pane content;
    public Timer timer;
    @FXML
    public Text text_pole;
    @FXML
    public TextField hoursField;
    @FXML
    public TextField minsField;
    @FXML
    private Spinner traficSpinner;
    @FXML
    public Text linka_vypsat;
    @FXML
    public Text text_stops;
    @FXML
    public Text timeDelayString;
    private List<MyStreet> closedStreets = new ArrayList<>();
    private List<MyStreet> detourStreets = new ArrayList<>();
    @FXML
    public Text bus_name;
    public int timeScale = 1;
    Boolean timer_stopped = false;
    @FXML
    public Text text_lines;
    @FXML
    public Text stopNameString;
    static public Stage secondStage;
    @FXML
    public Text actualTimeString;
    @FXML
    public Text stop_text;
    int timer_speed = 1;
    @FXML
    private ChoiceBox<String> percentage;
    @FXML
    private ChoiceBox<String> line_selector;
    static public MyLine last_clicked_line;
    static public LocalTime actualTime;
    public List<MyLine> liner = new ArrayList<>();
    public List<Drawable> elements = new ArrayList<>();
    private LocalTime time = LocalTime.now();
    public List<Time_update> updates = new ArrayList<>();

    /**
     * Funkce která nastaví text do textového pole.
     *
     * @param id text který chceme zapsat
     */
    public void setText_pole(String id) {
        this.text_pole.setText(id);
    }

    /**
     * Funkce na nastavení jména autobusu do pole po kliknutí
     * podle posledního kliknutého autobusu.
     */
    public void setBus_name() {
        this.bus_name.setText(Vehicle.lastBusClicked.getId());
    }


    /**
     * Funkce na změnu textu v poli autobusů po odznačení.
     */
    public void unsetBus_name() {
        this.bus_name.setText("-");
    }

    /**
     * Funkce nastavuje text do textového pole, podle toho jestli
     * je jsou nějaké tastávky nebo ne.
     *
     * @param stops
     */
    public void setText_stops(List<MyStop> stops) {
        String stops_string = "";
        if (stops.size() == 0) {
            this.text_stops.setText("No stops on street");
        } else {
            for (Stop stop : stops
            ) {
                stops_string = stops_string.concat(stop.getId());
                stops_string = stops_string.concat(" ");

            }
            this.text_stops.setText(stops_string);
        }
    }

    public void setText_delay(Vehicle bus){
        this.timeDelayString.setText(String.valueOf((int)(bus.getDelay()/60)) +
                " min " + String.valueOf(bus.getDelay()%60) + " sec");
    }

    /**
     * Funkce nastaví text podle toho kolik linek (a jestli nějaká)
     * projíždí ulicí.
     *
     * @param lines
     */
    public void setText_lines(List<MyLine> lines) {
        String lines_string = "";
        if (lines.size() == 0) {
            this.text_lines.setText("No lines on street");
        } else {
            int i = 0;
            for (MyLine line : lines) {
                if (i != 0) {
                    lines_string = lines_string.concat(", ");
                }
                lines_string = lines_string.concat(line.getId());
                i++;
            }
            this.text_lines.setText(lines_string);
        }
    }

    /**
     * Funkce pouze odstraní text z textového pole.
     */
    public void unsetText_lines() {
        this.text_lines.setText(" - ");
    }

    /**
     * Funkce pouze odstraní text z textového pole.
     */
    public void unsetText_stops() {
        this.text_stops.setText(" -");
    }


    /**
     * Funkce pouze odstraní text z textového pole.
     */
    public void unsetText_pole() {
        this.text_pole.setText("No street selected");
    }

    /**
     * Funkce nastaví linku podle toho která byla vybrána.
     *
     * @param seznam_linek seznam dostupných linek
     */
    public void setLine(List<MyLine> seznam_linek) {
        this.line_selector.setValue("None");
        this.line_selector.getItems().add("None");
        for (MyLine linka : seznam_linek
        ) {
            this.line_selector.getItems().add(linka.getId());
        }
        liner = seznam_linek;
    }

    /**
     * Funkce získá a nastaví linku.
     *
     * @param id jméno linky
     */
    public void setLineSelector(String id) {
        this.line_selector.setValue(id);
    }

    /**
     * Funce nastavuje selektor rychlosti autobusů a zadává do něj hodnoty.
     * Nastaví defaultní rychlost na 100%.
     */
    public void getPercentage() {
        this.percentage.getItems().add("100%");
        this.percentage.getItems().add("200%");
        this.percentage.getItems().add("500%");
        this.percentage.getItems().add("1000%");
        this.percentage.setValue("100%");
    }

    /**
     * Funkce která po zmáčknutí klávesy pozastaví/spustí program.
     */
    @FXML
    public void onPressPause() {

        if (timer_stopped == false) {
            timer.cancel();
            pause_button.setText("RUN");
            timer_stopped = true;
        } else {
            timer.cancel();
            pause_button.setText("PAUSE");
            startTime(timer_speed);
            timer_stopped = false;
        }
    }

    /**
     * Funkce vyznačí linku která vybrána. Pokud není vybrána žádná,
     * nic se nestane.
     */
    @FXML
    public void onLineChange() {
        unlineLine();
        choice1 = getChoise(line_selector);
        for (MyLine linka : liner
        ) {
            if (linka.getId() == choice1) {
                last_clicked_line = linka;
                this.linka_vypsat.setText(Integer.toString(linka.buses.size()));
                for (MyStreet ulice : linka.streets
                ) {
                    if (!closedStreets.contains(ulice) && !detourStreets.contains(ulice)) {
                        ulice.line.setStroke(Color.BLUE);
                        ulice.line.setStrokeWidth(5);
                    }
                }
                for (MyStop stop : linka.stops) {
                    stop.kruh.setRadius(10);
                }
            }

        }
        if (choice1 == "None") {
            last_clicked_line = null;
            this.linka_vypsat.setText("None");
        }
    }

    /**
     * Funkce pouze odznačí všechny ulice linky
     */
    private void unlineLine() {
        if (choice1 != null) {
            for (MyLine linka : liner
            ) {
                if (linka.getId() == choice1) {

                    for (MyStreet ulice : linka.streets
                    ) {
                        if (!closedStreets.contains(ulice) && !detourStreets.contains(ulice)) {
                            ulice.line.setStroke(Color.BLACK);
                            ulice.line.setStrokeWidth(2);
                        }
                    }
                    for (MyStop stop : linka.getStops()) {
                        stop.kruh.setRadius(4);
                    }
                }

            }
        }
    }

    /**
     * Funkce si získá zvolenou rychlost a následně se znovu spustí
     * čas a vybere se rychlost.
     */
    @FXML
    private void onTimeScaleChange() {
        String choice = getChoise(percentage);

        if (choice == "100%") {
            timer.cancel();
            startTime(1);
            timer_speed = 1;
            timer_stopped = false;
            timeScale = 1;
            pause_button.setText("PAUSE");
        }
        if (choice == "200%") {
            timer.cancel();
            startTime(2);
            timer_speed = 2;
            timer_stopped = false;
            timeScale = 1;
            pause_button.setText("PAUSE");
        }
        if (choice == "500%") {
            timer.cancel();
            startTime(5);
            timer_speed = 5;
            timer_stopped = false;
            timeScale = 1;
            pause_button.setText("PAUSE");
        }
        if (choice == "1000%") {
            timer.cancel();
            startTime(10);
            timer_speed = 10;
            timeScale = 1;
            timer_stopped = false;
            pause_button.setText("PAUSE");
        }
    }


    /**
     * Funkce získává po kliknutí na tlačítko zvolenou rychlost.
     *
     * @param percentage získaná procenta
     * @return choice - vrátí se hodnota výběru
     */
    public String getChoise(ChoiceBox<String> percentage) {
        String choice = percentage.getValue();
        return choice;
    }


    /**
     * Funkce mění velikost obrazu po scrollu kolečkem.
     *
     * @param event - bereme z aktuálního eventu
     */
    @FXML
    private void onZoom(ScrollEvent event) {
        event.consume();
        double zoom = event.getDeltaY() > 0 ? 1.1 : 0.9;
        if (content.getScaleX() < 1.5 && content.getScaleX() > 0.2) {

            content.setScaleX(zoom * content.getScaleX());
            content.setScaleY(zoom * content.getScaleY());
            content.layout();
        } else {
            if (content.getScaleX() > 1.5 && zoom == 0.9) {
                content.setScaleX(zoom * content.getScaleX());
                content.setScaleY(zoom * content.getScaleY());
                content.layout();
            } else if (content.getScaleX() < 0.2 && zoom == 1.1) {
                content.setScaleX(zoom * content.getScaleX());
                content.setScaleY(zoom * content.getScaleY());
                content.layout();
            }
        }
    }

    /**
     * Funkce na nastavení elementů na scénu a do listu Drawable.
     *
     * @param elements - list vykreslovaných objektů
     */
    public void setElements(List<Drawable> elements) {
        this.elements = elements;
        this.setSpinner();
        actualTime = java.time.LocalTime.now();
        setActualTime();
        for (Drawable drawable : elements) {
            content.getChildren().addAll(drawable.getGui());

            if (drawable instanceof Time_update) {
                updates.add((Time_update) drawable);
            }
        }
        content.setOnMouseClicked(e -> {
            if (e.getButton() == MouseButton.SECONDARY) {
                if (MyStreet.last_street_clicked != null) {
                    MyStreet.last_street_clicked.unhighlight();
                }
                if (MyStop.last_stop_clicked != null) {
                    MyStop.last_stop_clicked.unhighlight();
                }
                if (Vehicle.lastBusClicked != null) {
                    Vehicle.lastBusClicked.unhighlight();
                }
                if (last_clicked_line != null) {
                    this.unlineLine();
                    last_clicked_line = null;
                    this.linka_vypsat.setText("None");
                    this.line_selector.setValue("None");
                }
            }

        });
        for(Time_update update : updates){
            if(update.getClass() == Vehicle.class){
                if(actualTime.isAfter(((Vehicle) update).getStartTime()) && actualTime.isBefore(((Vehicle) update).getEndTime())){
                    ((Vehicle) update).addDistance(actualTime.toSecondOfDay() - ((Vehicle) update).getStartTime().toSecondOfDay());
                }
            }
        }
    }

    /**
     * Nastavení aktuálního času při spuštění.
     */
    @FXML
    public void setActualTime() {
        if (main_controller_static == null) {
            actualTimeString.setText(actualTime.format(DateTimeFormatter.ofPattern("HH:mm:ss")));
        } else {
            main_controller_static.actualTimeString.setText(actualTime.format(DateTimeFormatter.ofPattern("HH:mm:ss")));

        }
    }


    /**
     * Funkce aktualizuje čas a následně překresluje prvky.
     *
     * @param scale - parametr  kterým chceme zrychlovat/zpomalovat
     */
    public void startTime(float scale) {
        timer = new Timer(false);
        double pastTime = 0;
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                if(Vehicle.lastBusClicked != null){
                    setText_delay(Vehicle.lastBusClicked);
                } else {
                    timeDelayString.setText("0 min 0 sec");
                }
                time = time.plusSeconds(1);
                actualTime = actualTime.plusSeconds(timeScale);
                setActualTime();
                for (Time_update update : updates) {
                    try {
                        long test = System.currentTimeMillis();
                        if (update.getClass() == ija.Vehicle.Vehicle.class) {
                            if (((Vehicle) update).getStartTime().isBefore(actualTime) && ((Vehicle) update).getEndTime().isAfter(actualTime)) {
                                // JEDEME
                                // Vyrazí nový autobus
                                if (((Vehicle) update).getGui().get(0).getFill() == Color.TRANSPARENT) {
                                    // Přebarví pouze vyjíždějící autobusy, nikoliv obarvené kliknutím.
                                    ((Vehicle) update).getGui().get(0).setMouseTransparent(false);
                                    ((Vehicle) update).getGui().get(0).setFill(Color.BLUE);
                                }
                            } else {
                                // STOJÍ, NEBO ZASTAVUJE
                                if (((Vehicle) update).getGui().get(0).getFill() != Color.TRANSPARENT && ((Vehicle) update).getDelay() == 0) {
                                    // ZASTAVI
                                    ((Vehicle) update).resetVehicle();
                                    continue;
                                } else if (((Vehicle) update).getGui().get(0).getFill() != Color.TRANSPARENT && ((Vehicle) update).getDelay() != 0) {
                                    if (((Vehicle) update).isForward() != ((Vehicle) update).isLastDir()) {
                                        // ZASTAVI - autobus se zpožděním, co dojel na točnu
                                        ((Vehicle) update).resetVehicle();
                                        continue;
                                    }
                                } else {
                                    // Již stojí, nechat stát
                                    continue;
                                }
                            }
                        }
                        update.update(false, false);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }, 0, (long) (1000 / scale));


    }


    /**
     * Pokud klikneme v nastavení času na Cancel nic se
     * nenastaví.
     */
    @FXML
    public void clickCancel() {
        secondStage.close();
        secondStage = null;
        main_controller_static = null;
    }

    /**
     * Pokud proběhne nastavení času v pořádku změní se
     * hodnota času a následně přepočítá čas.
     */
    @FXML
    public void clickOk() {
        secondStage.close();
        LocalTime oldTime = actualTime;
        try {
            actualTime = LocalTime.of(Integer.parseInt(this.hoursField.getText()), Integer.parseInt(this.minsField.getText()));
        } catch (NumberFormatException e) {
            actualTime = LocalTime.now();
        } catch (DateTimeException e) {
            if (Integer.parseInt(this.hoursField.getText()) < 24) {
                actualTime = LocalTime.of(Integer.parseInt(this.hoursField.getText()), 0);
            } else {
                actualTime = LocalTime.of(0, 0);
            }
        }
        this.setActualTime();
        secondStage = null;

        // Získám všechny autobusy
        List<Vehicle> buses = new ArrayList<>();
        for (Time_update item : main_controller_static.updates) {
            if (item.getClass() == Vehicle.class) {
                buses.add((Vehicle) item);
            }
        }
        for (Vehicle bus : buses) {
            if (actualTime.isAfter(oldTime)) {
                // Posun kupredu
                if (oldTime.isBefore(bus.getStartTime())) {
                    if (actualTime.equals(bus.getStartTime())) {
                        bus.addDistance(actualTime.toSecondOfDay() - bus.getStartTime().toSecondOfDay());
                    } else if (actualTime.isAfter(bus.getStartTime())) {
                        bus.addDistance(actualTime.toSecondOfDay() - bus.getStartTime().toSecondOfDay());
                    }
                } else {
                    bus.addDistance(actualTime.toSecondOfDay() - oldTime.toSecondOfDay());
                }
            } else if (actualTime.isBefore(oldTime)) {
                // Posun zpět
                bus.resetVehicle();
                if (bus.getStartTime().isBefore(actualTime)) {
                    bus.addDistance(actualTime.toSecondOfDay() - bus.getStartTime().toSecondOfDay());
                }
            }
        }
        main_controller_static = null;
    }

    /**
     * Vytvoření okna pro nastavení času.
     *
     * @throws IOException
     */
    @FXML
    public void runSetTime() throws IOException {
        if (secondStage != null) {
            return;
        }
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/setTime.fxml"));
        loader = new FXMLLoader(getClass().getResource("/setTime.fxml"));
        AnchorPane root = loader.load();
        Scene scene = new Scene(root);
        secondStage = new Stage();
        secondStage.setTitle("Nastavení Času");
        secondStage.setScene(scene);
        secondStage.show();
        secondStage.setAlwaysOnTop(true);
        secondStage.setOnCloseRequest(event -> {
            secondStage = null;
        });
        main_controller_static = this;
    }


    /**
     * Funkce vytvoří zácpu linky podle zadané hodnoty.
     */
    @FXML
    public void setTraficSpinner() {
        if (MyStreet.last_street_clicked != null) {
            MyStreet.last_street_clicked.setTrafic((Integer) this.traficSpinner.getValue());
        }
    }

    /**
     * Nastavení hodnoty pro vybrání zácpy.
     *
     * @param v - hondota která byla vybrána
     */
    public void setTraficSpinnerValue(int v) {
        SpinnerValueFactory<Integer> traficValueFactory = new SpinnerValueFactory.IntegerSpinnerValueFactory(1, 10, v);
        this.traficSpinner.setValueFactory(traficValueFactory);
    }

    /**
     * Nastavení hodnot spinneru ve scéně.
     */
    public void setSpinner() {
        SpinnerValueFactory<Integer> traficValueFactory = new SpinnerValueFactory.IntegerSpinnerValueFactory(1, 10, 1);
        this.traficSpinner.setValueFactory(traficValueFactory);
    }


    @FXML
    public void closeStreetAdd() {
        if (MyStreet.last_street_clicked != null && !detourStreets.contains(MyStreet.last_street_clicked) && !closedStreets.contains(MyStreet.last_street_clicked)) {
            closedStreets.add(MyStreet.last_street_clicked);
            MyStreet.last_street_clicked.line.setStrokeWidth(10);
            MyStreet.last_street_clicked.line.setStroke(Color.ORANGE);
            MyStreet.last_street_clicked = null;
            this.unsetText_pole();
            this.unsetText_stops();
            this.unsetText_lines();
        }
    }

    @FXML
    public void detourStreetAdd() {
        if (MyStreet.last_street_clicked != null && !closedStreets.contains(MyStreet.last_street_clicked)) {
            detourStreets.add(MyStreet.last_street_clicked);
            MyStreet.last_street_clicked.line.setStrokeWidth(10);
            MyStreet.last_street_clicked.line.setStroke(Color.GREEN);
            MyStreet.last_street_clicked = null;
            this.unsetText_pole();
            this.unsetText_stops();
            this.unsetText_lines();
        }
    }

    @FXML
    public void setDetour() {
        if (!closedStreets.isEmpty() && !detourStreets.isEmpty()) {
            for (Time_update update : this.updates) {
                if (update.getClass() == MyStreet.class) {
                    if (closedStreets.contains((MyStreet) update)) {
                        ((MyStreet) update).getLinesOnStreet().clear();
                    }
                } else if (update.getClass() == Vehicle.class) {
                    if (((Vehicle) update).getStreets().contains(closedStreets.get(0))) {
                        if (closedStreets.contains(((Vehicle) update).getStreets().get(((Vehicle) update).getStreetIdx()))) {
                            ((Vehicle) update).setLost(true);
                        }
                        ((Vehicle) update).resetBusStreets(closedStreets, detourStreets);
                    }
                }
            }

            this.cancelDetour();
        }
    }

    @FXML
    public void cancelDetour() {
        for (MyStreet street : closedStreets) {
            street.line.setStrokeWidth(2);
            street.line.setStroke(Color.BLACK);
        }
        for (MyStreet street : detourStreets) {
            street.line.setStrokeWidth(2);
            street.line.setStroke(Color.BLACK);
        }
        closedStreets.clear();
        detourStreets.clear();

    }

    /**
     * Získání uzavřených ulic.
     *
     * @return list uzavřených ulic
     */
    public List<MyStreet> getClosedStreets() {
        return closedStreets;
    }

    public void setClosedStreets(List<MyStreet> closedStreets) {
        this.closedStreets = closedStreets;
    }

    /**
     * Získání objízdných ulic.
     *
     * @return
     */
    public List<MyStreet> getDetourStreets() {
        return detourStreets;
    }

    public void setDetourStreets(List<MyStreet> detourStreets) {
        this.detourStreets = detourStreets;
    }

    /**
     * Funkce zobrazí jízdní řád v samostatném okně do scény.
     * @param mouseEvent
     */
    public void ShowTimeTable(MouseEvent mouseEvent) {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/time_table.fxml"));
        try {
            AnchorPane root1 = loader.load();
            int x = 50;
            int y = 20;
            Stage time_table = new Stage();
            time_table.setTitle("Line time table");
            if (choice1 == null || choice1 == "None") {
                Text text = new Text();
                text.setText("No line selected");
                text.setX(x);
                text.setY(y);
                root1.getChildren().add(text);
            } else {

                for (MyLine linka : liner
                ) {
                    if (linka.getId() == choice1) {
                        int i = 0;
                        for (Vehicle vozidlo : linka.buses) {
                            Text text = new Text();
                            String whole_time_table = vozidlo.getId() + "   ";
                            for (int g = 0; g < vozidlo.getActualTimetable().size(); g++) {
                                whole_time_table = whole_time_table.concat(vozidlo.getStops().get(g).getId());
                                whole_time_table = whole_time_table.concat(" : ");
                                whole_time_table = whole_time_table.concat(String.valueOf(vozidlo.getActualTimetable().get(g)));
                                whole_time_table = whole_time_table.concat("   ");
                            }
                            text.setText(whole_time_table);
                            y += 20;
                            text.setX(x);
                            text.setY(y);
                            root1.getChildren().add(text);
                            i++;
                        }
                    }
                }
            }
            Scene scene1 = new Scene(root1);
            time_table.setScene(scene1);
            time_table.showAndWait();
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    public void ShowBusTimeTable(MouseEvent mouseEvent) {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/bus_table.fxml"));
        try {
            AnchorPane root1 = loader.load();
            int x = 50;
            int y = 20;
            Stage time_table = new Stage();
            time_table.setTitle("Bus time table");
            Text text = new Text();
            text.setX(50);
            text.setY(50);
            if(Vehicle.lastBusClicked != null) {
                String whole_time_table = Vehicle.lastBusClicked.getId() + "   ";
                for (int g = 0; g < Vehicle.lastBusClicked.getActualTimetable().size(); g++) {
                    whole_time_table = whole_time_table.concat(Vehicle.lastBusClicked.getStops().get(g).getId());
                    whole_time_table = whole_time_table.concat(" : ");
                    whole_time_table = whole_time_table.concat(String.valueOf(Vehicle.lastBusClicked.getActualTimetable().get(g)));
                    whole_time_table = whole_time_table.concat("   ");
                }
                text.setText(whole_time_table);
            }else{
                text.setText("No vehicle selected");
            }
            root1.getChildren().add(text);
            Scene scene1 = new Scene(root1);
            time_table.setScene(scene1);
            time_table.showAndWait();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
