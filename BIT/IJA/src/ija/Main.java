package ija;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.dataformat.yaml.YAMLFactory;
import com.fasterxml.jackson.dataformat.yaml.YAMLGenerator;
import ija.Vehicle.Vehicle;
import ija.myDraw.Drawable;
import ija.myMaps.MyMap;
import ija.myMaps.MyStreet;
import javafx.application.Application;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ButtonType;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.BorderPane;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import ija.myMaps.MyStop;


import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Implementace hlavího modulu pro spouštění.
 * V tomto modulu se inicialuzijí základní části našeho projektu
 * jako je scéna, základní objekty, které jsou uloženy v listu a rozlišovány
 * na ty které se budou překreslovat a ty které jsou staicté. Také se
 * spouští funkce z modulů pro aktualizaci scény a výpočet času a
 * aktualizace jednotlivých prvků scény.
 *  @author Michal Šedý
 *  @author Ondřej Pavlacký
 */
public class Main extends Application {

    /** List objektů */
    public List<Drawable> elements = new ArrayList<>();
    /** Nové okno pro uvítací obrazovku */
    public Stage welcome_window = new Stage();
    @FXML
    private javafx.scene.control.Button closeButton;
    /** Mapa objektů */
    static public MyMap mapa;


    /** Nahrávání ze souboru */
    public YAMLFactory factory = new YAMLFactory().disable(YAMLGenerator.Feature.WRITE_DOC_START_MARKER);
    public ObjectMapper mapper = new ObjectMapper(factory);


    public static void main(String[] args) {
        launch(args);
    }


    /** Funkce která začne celý chod programu a naváže na JavaFX.
     * @param primaryStage Stage která je FX zobrazována.
     * */
    @Override
    public void start(Stage primaryStage) throws Exception {

        /** Po spuštění se zobrazí uvítací obrazovka */
        primaryStage.setOnShowing((event) -> {
            try {
                FXMLLoader loader = new FXMLLoader(getClass().getResource("/welcome_scene.fxml"));
                AnchorPane root1 = loader.load();
                Scene scene1 = new Scene(root1);
                welcome_window.setTitle("Welcome window");
                welcome_window.setScene(scene1);
                welcome_window.setOnCloseRequest(eventos -> {
                    System.exit(0);
                });
                welcome_window.showAndWait();
            }catch (IOException e) {
                e.printStackTrace();
            }
        });

        /** Načtení hlavního okna které obsluhuje celou scénu */
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/layout.fxml"));
        BorderPane root = loader.load();
        Scene scene = new Scene(root);

        primaryStage.setTitle("IJA Project - Bus simulator");
        primaryStage.setScene(scene);
        primaryStage.show();
        primaryStage.setOnCloseRequest(e -> {        Alert alert = new Alert(Alert.AlertType.CONFIRMATION, "Sure you want to exit ?", ButtonType.YES, ButtonType.NO);
            alert.showAndWait();
            if(alert.getResult() == ButtonType.NO){
            e.consume();
            }
            if(alert.getResult() == ButtonType.YES){
                primaryStage.close();
                System.exit(0);
            }
        });
        Main_controller controller =  loader.getController();


        /** Přidání controller do ulic, zastávek a autobusů */
        for(MyStreet street: mapa.getStreets()) {
            street.addControler(controller);
        }
        for(MyStop stop: mapa.getStops()) {
            stop.addControler(controller);
        }
        for(Vehicle bus: mapa.getBuses()) {
            bus.setController(controller);
        }

        /** Nastavení drawable elementů */
        elements.addAll(mapa.getStreets());
        elements.addAll(mapa.getStops());
        elements.addAll(mapa.getBuses());

        controller.setLine(mapa.getLines());

        controller.getPercentage();
        controller.setElements(elements);
        controller.startTime(1);
    }

    /** Pokud vybereme referenční mapu */
    public void pressed_ref(MouseEvent mouseEvent) {
        Stage stage = (Stage) closeButton.getScene().getWindow();

        try {
            // Pozor antem se zpouští z úrovně projektu, kliknutím ve složce z úrovně podložky. nutnost ../
        this.mapa = mapper.readValue(new File("data/parse.yaml"), MyMap.class);

        } catch (com.fasterxml.jackson.databind.exc.MismatchedInputException e) {
            Alert exitAlert = new Alert(Alert.AlertType.ERROR, "YAML structure problem occurred.");
            exitAlert.showAndWait();
            System.exit(0);
        } catch (java.io.FileNotFoundException e){
            Alert exitAlert = new Alert(Alert.AlertType.ERROR, "No YAML with name \"data/parse.yaml\" found.");
            exitAlert.showAndWait();
            System.exit(0);
        } catch (IOException e) {
            e.printStackTrace();
        }
        stage.close();
    }

    /** Pokud chceme načíst mapu ze souboru */
    public void pressed_browse(MouseEvent mouseEvent) {
        FileChooser chooser = new FileChooser();
        Stage stage = (Stage) closeButton.getScene().getWindow();
        chooser.getExtensionFilters().addAll(new FileChooser.ExtensionFilter("yaml", "*.yaml"));
        File selectedFile = chooser.showOpenDialog(welcome_window);
        if (selectedFile != null) {
            try {
                this.mapa = mapper.readValue(selectedFile, MyMap.class);

            } catch (com.fasterxml.jackson.databind.exc.MismatchedInputException e){
                Alert exitAlert = new Alert(Alert.AlertType.ERROR, "YAML structure problem occurred.");
                exitAlert.showAndWait();
                System.exit(0);
            } catch (IOException e) {
                e.printStackTrace();
            }

            stage.close();
        }
    }

}
