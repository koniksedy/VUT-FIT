/* SQL skript pro vytvoření databáze Lékárny.
   Zadání: LÉKÁRNA
   Autoři: Michal Šedý <xsedym02>
           Ondřej Pavlacký <xpavla15>
   Poslední úprava: 22.4.2020
 */

-- VYČIŠTĚNÍ TABULEK, PROCEDUR, POHLEDŮ
-- ##########################################################################
-- ##########################################################################
DROP TABLE "rezervace_lek";
DROP TABLE "rezervace";
DROP TABLE "prodej_lek";
DROP TABLE "recept";
DROP TABLE "dodavka";
DROP TABLE "prodej";
DROP TABLE "lek";
DROP TABLE "dodavatel";
DROP TABLE "zamestnanec";
DROP TABLE "pobocka";
DROP SEQUENCE "seq_osobni_cislo";
DROP PROCEDURE "propustit";
DROP PROCEDURE "povyseni";
DROP PROCEDURE "najdi_lek";
DROP MATERIALIZED VIEW "view_lek_s_doplatkem";


-- DEFINICE TABULEK
-- ##########################################################################
-- ##########################################################################

CREATE TABLE "pobocka" (
    "id_pobocky" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "mesto" VARCHAR(78) NOT NULL,
    "ulice" VARCHAR(78) NOT NULL,
    "telefon" VARCHAR(9) NOT NULL CHECK (REGEXP_LIKE("telefon", '^\d+$'))
);

CREATE TABLE "rezervace" (
    "id_rezervace" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "rodne_cislo" VARCHAR(10) NOT NULL CHECK (REGEXP_LIKE("rodne_cislo", '^\d+$') and
                                           ((LENGTH("rodne_cislo") = 9 and SUBSTR("rodne_cislo", 7) != '000')
                                                or MOD(TO_NUMBER("rodne_cislo"), 11) = 0) and
                                     (TO_NUMBER(SUBSTR("rodne_cislo", 3, 2)) BETWEEN 51 and 62 and
                                      TO_NUMBER(SUBSTR("rodne_cislo", 5, 2)) BETWEEN 1 and 71) or
                                     (TO_NUMBER(SUBSTR("rodne_cislo", 3, 2)) BETWEEN 21 and 32 and
                                      TO_NUMBER(SUBSTR("rodne_cislo", 5, 2)) BETWEEN 1 and 31) or
                                     (TO_NUMBER(SUBSTR("rodne_cislo", 3, 2)) BETWEEN 1 and 12 and
                                      TO_NUMBER(SUBSTR("rodne_cislo", 5, 2)) BETWEEN 1 and 71)
                                           ),
    "datum_zrizeni" DATE DEFAULT SYSDATE,
    "datum_expirace" DATE DEFAULT SYSDATE + 12,
    "id_pobocky" INT NOT NULL CHECK ("id_pobocky" IN (1, 2, 3)),

    CONSTRAINT "fk_rezervace_id_pobocky"
        FOREIGN KEY ("id_pobocky")
            REFERENCES "pobocka" ("id_pobocky")
                ON DELETE CASCADE
);

CREATE TABLE "lek" (
    "kod_leku" CHAR(7) NOT NULL PRIMARY KEY CHECK (REGEXP_LIKE("kod_leku", '^\d+$')),
    "nazev" VARCHAR(78) NOT NULL,
    "doplnkovy_nazev" VARCHAR(78) DEFAULT NULL,
    "ucinna_latka" VARCHAR(78) NOT NULL,
    "prodejni_cenna" INT NOT NULL,
    "vyse_prispevku" INT DEFAULT 0 NOT NULL,
    "volne_prodejny" CHAR(1) NOT NULL CHECK ("volne_prodejny" IN ('A', 'N', 'M'))
);

CREATE TABLE "rezervace_lek" (
    "id_rezervace" INT NOT NULL,
    "kod_leku" CHAR(7) NOT NULL,
    "mnozstvi" INT DEFAULT 1 NOT NULL,

    CONSTRAINT "pk_rezervace_lek"
        PRIMARY KEY ("id_rezervace", "kod_leku"),

    CONSTRAINT "fk_rezervace_lek_id_rezervace"
        FOREIGN KEY ("id_rezervace")
            REFERENCES "rezervace" ("id_rezervace")
                ON DELETE CASCADE,

    CONSTRAINT "fk_rezervace_lek_kod_leku"
        FOREIGN KEY ("kod_leku")
            REFERENCES "lek" ("kod_leku")
                ON DELETE CASCADE
);

CREATE TABLE "prodej" (
    "id_prodeje" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "datum" DATE DEFAULT SYSDATE,
    "id_pobocky" INT NOT NULL CHECK ("id_pobocky" IN (1, 2, 3)),

    CONSTRAINT "fk_prodej_id_pobocky"
        FOREIGN KEY ("id_pobocky")
            REFERENCES "pobocka" ("id_pobocky")
                ON DELETE CASCADE
);

CREATE TABLE "prodej_lek" (
    "id_prodeje" INT NOT NULL,
    "kod_leku" CHAR(7) NOT NULL,
    "mnozstvi_na_recept" INT DEFAULT 0 NOT NULL,
    "mnozstvi_bez_receptu" INT DEFAULT 1 NOT NULL,

    CONSTRAINT "pk_prodej_lek"
        PRIMARY KEY ("id_prodeje", "kod_leku"),

    CONSTRAINT "fk_prodej_lek_id_prodeje"
        FOREIGN KEY ("id_prodeje")
            REFERENCES "prodej" ("id_prodeje")
                ON DELETE CASCADE,

    CONSTRAINT "fk_prodej_lek_kod_leku"
        FOREIGN KEY ("kod_leku")
            REFERENCES "lek" ("kod_leku")
                ON DELETE CASCADE
);

CREATE TABLE "dodavatel" (
    "jmeno_firmy" VARCHAR(78) NOT NULL PRIMARY KEY CHECK ("jmeno_firmy" IN ('AaL', 'LLLeky', 'Léky pro Vás')),
    "mesto" VARCHAR(78) NOT NULL,
    "ulice" VARCHAR(78) NOT NULL,
    "telefon" VARCHAR(9) NOT NULL CHECK (REGEXP_LIKE("telefon", '^\d+$'))
);

CREATE TABLE "dodavka" (
    "id_dodavky" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "datum_prijeti" DATE DEFAULT SYSDATE,
    "datum_expirace" DATE NOT NULL,
    "mnozstvi_nakupni" INT NOT NULL,
    "mnozstvi_aktualni" INT NOT NULL,
    "nakupni_cena" INT NOT NULL,
    "kod_leku" CHAR(7) NOT NULL CHECK (REGEXP_LIKE("kod_leku", '^\d+$')),
    "id_pobocky" INT NOT NULL CHECK ("id_pobocky" IN (1, 2, 3)),
    "jmeno_firmy" VARCHAR(78) NOT NULL CHECK ("jmeno_firmy" IN ('AaL', 'LLLeky', 'Léky pro Vás')),

    CONSTRAINT "fr_dodavka_kod_leku"
        FOREIGN KEY ("kod_leku")
            REFERENCES "lek" ("kod_leku")
                ON DELETE CASCADE,

    CONSTRAINT "fr_dodavka_jmeno_firmy"
        FOREIGN KEY ("jmeno_firmy")
            REFERENCES "dodavatel" ("jmeno_firmy")
                ON DELETE CASCADE,

    CONSTRAINT "fr_dodavka_id_pobocky"
        FOREIGN KEY ("id_pobocky")
            REFERENCES "pobocka" ("id_pobocky")
                ON DELETE CASCADE
);

CREATE TABLE "zamestnanec" (
    "osobni_cislo" INT NOT NULL PRIMARY KEY,
    "jmeno" VARCHAR(78) NOT NULL,
    "prijmeni" VARCHAR(78) NOT NULL,
    "plat" INT NOT NULL,
    "rodne_cislo" VARCHAR(10) NOT NULL CHECK (REGEXP_LIKE("rodne_cislo", '^\d+$') and
                                           ((LENGTH("rodne_cislo") = 9 and SUBSTR("rodne_cislo", 7) != '000')
                                                or MOD(TO_NUMBER("rodne_cislo"), 11) = 0) and
                                     (TO_NUMBER(SUBSTR("rodne_cislo", 3, 2)) BETWEEN 51 and 62 and
                                      TO_NUMBER(SUBSTR("rodne_cislo", 5, 2)) BETWEEN 1 and 71) or
                                     (TO_NUMBER(SUBSTR("rodne_cislo", 3, 2)) BETWEEN 21 and 32 and
                                      TO_NUMBER(SUBSTR("rodne_cislo", 5, 2)) BETWEEN 1 and 31) or
                                     (TO_NUMBER(SUBSTR("rodne_cislo", 3, 2)) BETWEEN 1 and 12 and
                                      TO_NUMBER(SUBSTR("rodne_cislo", 5, 2)) BETWEEN 1 and 71)
                                           ),
    "status" CHAR(1) DEFAULT 'L' CHECK ("status" IN ('V', 'L')),
    "id_pobocky" INT NOT NULL CHECK ("id_pobocky" IN (1, 2, 3)),

    CONSTRAINT "fr_zamestnanec_id_pobocky"
        FOREIGN KEY ("id_pobocky")
            REFERENCES "pobocka" ("id_pobocky")
                ON DELETE CASCADE
);

CREATE TABLE "recept" (
    "kod_receptu" CHAR(12) NOT NULL PRIMARY KEY CHECK (LENGTH("kod_receptu") = 12),
    "pojistovna" CHAR(3) NOT NULL CHECK ("pojistovna" IN ('111', '201', '205', '207', '209', '211', '213')),
    "rodne_cislo" VARCHAR(10) NOT NULL CHECK (REGEXP_LIKE("rodne_cislo", '^\d+$') and
                                           ((LENGTH("rodne_cislo") = 9 and SUBSTR("rodne_cislo", 7) != '000')
                                                or MOD(TO_NUMBER("rodne_cislo"), 11) = 0) and
                                     (TO_NUMBER(SUBSTR("rodne_cislo", 3, 2)) BETWEEN 51 and 62 and
                                      TO_NUMBER(SUBSTR("rodne_cislo", 5, 2)) BETWEEN 1 and 71) or
                                     (TO_NUMBER(SUBSTR("rodne_cislo", 3, 2)) BETWEEN 21 and 32 and
                                      TO_NUMBER(SUBSTR("rodne_cislo", 5, 2)) BETWEEN 1 and 31) or
                                     (TO_NUMBER(SUBSTR("rodne_cislo", 3, 2)) BETWEEN 1 and 12 and
                                      TO_NUMBER(SUBSTR("rodne_cislo", 5, 2)) BETWEEN 1 and 71)
                                           ),
    "id_prodeje" INT NOT NULL,
    "osobni_cislo" INT DEFAULT NULL,

    CONSTRAINT "fr_recept_id_prodeje"
        FOREIGN KEY ("id_prodeje")
            REFERENCES "prodej" ("id_prodeje")
                ON DELETE CASCADE,

    CONSTRAINT "fr_recept_osobni_cislo"
        FOREIGN KEY ("osobni_cislo")
            REFERENCES "zamestnanec" ("osobni_cislo")
                ON DELETE SET NULL
);

-- Implementace TRIGGERů
-- ##########################################################################
-- ##########################################################################

CREATE SEQUENCE "seq_osobni_cislo";

-- (1) Vytvoří osobní číslo zaměstnance, pokud mu není přiděleno.
-- Generování primárního klíče tabulky zaměstnanec s využitím SEKVENCE.
CREATE OR REPLACE TRIGGER "osobni_cislo_zamestnance"
    BEFORE INSERT OR UPDATE ON "zamestnanec"
    FOR EACH ROW
    BEGIN
        IF :NEW."osobni_cislo" IS NULL THEN
           :NEW."osobni_cislo" := "seq_osobni_cislo".nextval;
        END IF;
    END;
/

-- (2) V případě vložení nového zaměstnance, se mu automaticky nastaví platové zařazení podle jeho pozice.
CREATE OR REPLACE TRIGGER "prijmuti_zamestnance"
    BEFORE INSERT OR UPDATE ON "zamestnanec"
    FOR EACH ROW
    BEGIN
        IF :NEW."plat" IS NULL THEN
           IF :NEW."status" = 'L' THEN
               :NEW."plat" := 27000;
           ELSE
               :NEW."plat" := 39500;
            END IF;
        END IF;
    END;
/

-- (3) V případě vložení léku s cenou menší než příspěvek, navýší cenu na úroveň příspěvku.
-- Nekalá praktika prodejce, ale když to pojištovna zaplatí a lidé budou mít lék zdarma, tak proč si nenahrabat. :-)
CREATE OR REPLACE TRIGGER "zaokrouhleni_ceny"
    BEFORE INSERT OR UPDATE ON "lek"
    FOR EACH ROW
    BEGIN
       IF :NEW."vyse_prispevku" > :NEW."prodejni_cenna" THEN
           :NEW."prodejni_cenna" := :NEW."vyse_prispevku";
       end if;
    END;
/

-- PLNĚNÍ TABULEK DATY
-- ##########################################################################
-- ##########################################################################
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "volne_prodejny")
VALUES ('0032020', 'IBUPROFEN AL 400', '400MG TBL FLM 100 ', 'IBUPROFEN', 120, 'A');
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "volne_prodejny")
VALUES ('0208990', 'IBUPROFEN B. BRAUN', '600MG INF SOL 10X100ML', 'IBUPROFEN', 350, 'A');
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "volne_prodejny")
VALUES ('0207893', 'IBALGIN', '400MG TBL FLM 100 ', 'IBUPROFEN', 143, 'A');
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "vyse_prispevku", "volne_prodejny")
VALUES ('0216914', 'ZOFRAN', '16MG SUP 1', 'ONDANSETRON', 560, 28, 'N');
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "volne_prodejny")
VALUES ('0029752', 'VIAGRA', '50MG TBL FLM 2 I ', 'SILDENAFIL', 320, 'A');
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "vyse_prispevku", "volne_prodejny")
VALUES ('0176954', 'ALGIFEN NEO', '500MG/ML+5MG/ML POR GTT SOL 1X50ML', 'PITOFENON-HYDROCHLORID', 180, 170, 'N');
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "volne_prodejny")
VALUES ('0097375', 'ADIPEX RETARD', '15MG CPS RML 30', 'FENTERMIN', 874, 'M');
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "vyse_prispevku", "volne_prodejny")
VALUES ('0011955', 'DUROGESIC', '12MCG/H TDR EMP 5X2,1MG', 'FENTERMIN', 845, 230, 'M');
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "vyse_prispevku", "volne_prodejny")
VALUES ('0202090', 'ANALERGIN', '10MG TBL FLM 90', '10MG TBL FLM 90', 412, 170, 'A');

INSERT INTO "pobocka" ("mesto", "ulice", "telefon")
VALUES ('Brno', 'Cimburkova 13', '545469812');
INSERT INTO "pobocka" ("mesto", "ulice", "telefon")
VALUES ('Brno', 'Okrajová 532', '548752145');
INSERT INTO "pobocka" ("mesto", "ulice", "telefon")
VALUES ('Praha 6', 'Nikoly Testy 1232', '325874658');


INSERT INTO "rezervace" ("rodne_cislo", "id_pobocky")
VALUES ('7558015355', 1);
INSERT INTO "rezervace" ("rodne_cislo", "id_pobocky")
VALUES ('471115566', 1);
INSERT INTO "rezervace" ("rodne_cislo", "id_pobocky")
VALUES ('9709055609', 1);
INSERT INTO "rezervace" ("rodne_cislo", "id_pobocky")
VALUES ('9501217792', 2);
INSERT INTO "rezervace" ("rodne_cislo", "id_pobocky")
VALUES ('9510189040', 3);
INSERT INTO "rezervace" ("rodne_cislo", "id_pobocky")
VALUES ('420620198', 3);
INSERT INTO "rezervace" ("rodne_cislo", "id_pobocky")
VALUES ('425213025', 3);

INSERT INTO "rezervace_lek" ("id_rezervace", "kod_leku")
VALUES (1, '0032020');
INSERT INTO "rezervace_lek" ("id_rezervace", "kod_leku")
VALUES (2, '0208990');
INSERT INTO "rezervace_lek" ("id_rezervace", "kod_leku")
VALUES (3, '0176954');
INSERT INTO "rezervace_lek" ("id_rezervace", "kod_leku")
VALUES (4, '0216914');
INSERT INTO "rezervace_lek" ("id_rezervace", "kod_leku")
VALUES (5, '0208990');
INSERT INTO "rezervace_lek" ("id_rezervace", "kod_leku")
VALUES (6, '0216914');
INSERT INTO "rezervace_lek" ("id_rezervace", "kod_leku", "mnozstvi")
VALUES (7, '0216914', 2);

INSERT INTO "prodej" ("id_pobocky")
VALUES (1);
INSERT INTO "prodej" ("id_pobocky")
VALUES (2);
INSERT INTO "prodej" ("id_pobocky")
VALUES (1);
INSERT INTO "prodej" ("id_pobocky")
VALUES (2);
INSERT INTO "prodej" ("id_pobocky")
VALUES (2);
INSERT INTO "prodej" ("id_pobocky")
VALUES (1);
INSERT INTO "prodej" ("id_pobocky")
VALUES (3);
INSERT INTO "prodej" ("id_pobocky")
VALUES (3);
INSERT INTO "prodej" ("id_pobocky")
VALUES (3);

INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku", "mnozstvi_bez_receptu")
VALUES (1, '0032020', '2');
INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku", "mnozstvi_bez_receptu")
VALUES (2, '0207893', '2');
INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku", "mnozstvi_bez_receptu")
VALUES (3, '0029752', '10');
INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku", "mnozstvi_bez_receptu", "mnozstvi_na_recept")
VALUES (4, '0216914', '0', '1');
INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku", "mnozstvi_bez_receptu", "mnozstvi_na_recept")
VALUES (5, '0176954', '0', '1');
INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku")
VALUES (6, '0208990');
INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku", "mnozstvi_bez_receptu", "mnozstvi_na_recept")
VALUES (7, '0011955', '0', '1');
INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku", "mnozstvi_bez_receptu", "mnozstvi_na_recept")
VALUES (8, '0097375', '0', '2');
INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku")
VALUES (9, '0032020');
INSERT INTO "prodej_lek" ("id_prodeje", "kod_leku")
VALUES (9, '0208990');

INSERT INTO "dodavatel" ("jmeno_firmy", "mesto", "ulice", "telefon")
VALUES ('AaL', 'Praha', 'Markupova 231', '365874587');
INSERT INTO "dodavatel" ("jmeno_firmy", "mesto", "ulice", "telefon")
VALUES ('Léky pro Vás', 'Praha', 'Nad Okrouhlíkem 1874', '712458596');
INSERT INTO "dodavatel" ("jmeno_firmy", "mesto", "ulice", "telefon")
VALUES ('LLLeky', 'Brno', 'Chodská 65', '123456781');

INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/05/02', 'yyyy/mm/dd'), 56, 54, 75, '0032020', 1, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/11/12', 'yyyy/mm/dd'), 61, 61, 192, '0208990', 1, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2021/11/12', 'yyyy/mm/dd'), 60, 59, 220, '0208990', 1, 'LLLeky');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/08/12', 'yyyy/mm/dd'), 13, 13, 345, '0216914', 1, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/08/12', 'yyyy/mm/dd'), 79, 69, 160, '0029752', 1, 'Léky pro Vás');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2021/12/20', 'yyyy/mm/dd'), 59, 59, 250, '0176954', 1, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/12/20', 'yyyy/mm/dd'), 65, 65, 750, '0097375', 1, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2021/12/28', 'yyyy/mm/dd'), 40, 40, 384, '0202090', 1, 'Léky pro Vás');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2021/12/28', 'yyyy/mm/dd'), 67, 67, 354, '0202090', 1, 'LLLeky');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/05/02', 'yyyy/mm/dd'), 34, 34, 75, '0032020', 2, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/07/01', 'yyyy/mm/dd'), 86, 84, 86, '0207893', 2, 'Léky pro Vás');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/08/12', 'yyyy/mm/dd'), 3, 2, 345, '0216914', 2, 'AaL');
INSERT INTO "dodavka" ("datum_prijeti", "datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2018/09/12', 'yyyy/mm/dd'), TO_DATE('2019/12/01', 'yyyy/mm/dd'), 5, 5, 345, '0216914', 2, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2021/12/28', 'yyyy/mm/dd'), 35, 35, 384, '0202090', 2, 'Léky pro Vás');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/08/12', 'yyyy/mm/dd'), 45, 45, 172, '0029752', 2, 'LLLeky');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2021/12/20', 'yyyy/mm/dd'), 36, 36, 250, '0176954', 2, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/05/02', 'yyyy/mm/dd'), 252, 251, 75, '0032020', 3, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/07/01', 'yyyy/mm/dd'), 251, 251, 70, '0207893', 3, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/08/12', 'yyyy/mm/dd'), 87, 87, 345, '0216914', 3, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/08/12', 'yyyy/mm/dd'), 187, 187, 172, '0029752', 3, 'LLLeky');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/01/25', 'yyyy/mm/dd'), 90, 90, 250, '0176954', 3, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2020/12/20', 'yyyy/mm/dd'), 97, 97, 248, '0176954', 3, 'LLLeky');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/11/12', 'yyyy/mm/dd'), 321, 320, 220, '0208990', 3, 'LLLeky');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2022/07/16', 'yyyy/mm/dd'), 90, 89, 687, '0011955', 3, 'AaL');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2021/10/01', 'yyyy/mm/dd'), 66, 64, 701, '0097375', 3, 'LLLeky');
INSERT INTO "dodavka" ("datum_expirace", "mnozstvi_nakupni", "mnozstvi_aktualni", "nakupni_cena", "kod_leku", "id_pobocky", "jmeno_firmy")
VALUES (TO_DATE('2023/01/01', 'yyyy/mm/dd'), 214, 214, 384, '0202090', 2, 'AaL');

INSERT INTO "zamestnanec" ("jmeno", "prijmeni", "rodne_cislo", "id_pobocky")
VALUES ('Roman', 'Trávníček', '7902065666', 1);
INSERT INTO "zamestnanec" ("jmeno", "prijmeni", "rodne_cislo", "id_pobocky")
VALUES ('Jindřich', 'Toman', '7409041904', 1);
INSERT INTO "zamestnanec" ("jmeno", "prijmeni", "plat", "rodne_cislo",  "status", "id_pobocky")
VALUES ('Zuzana', 'Hotaříková', 45000, '7459041909', 'V', 1);
INSERT INTO "zamestnanec" ("jmeno", "prijmeni", "rodne_cislo", "id_pobocky")
VALUES ('Věra', 'Machalová', '9358140495', 2);
INSERT INTO "zamestnanec" ("jmeno", "prijmeni", "rodne_cislo", "status", "id_pobocky")
VALUES ('Jaroslav', 'Balog', '8107119999', 'V', 2);
INSERT INTO "zamestnanec" ("jmeno", "prijmeni", "rodne_cislo", "id_pobocky")
VALUES ('Rudolf', 'Skála', '8105069709', 3);
INSERT INTO "zamestnanec" ("jmeno", "prijmeni", "rodne_cislo", "id_pobocky")
VALUES ('Josef', 'Valchář', '9005120883', 3);
INSERT INTO "zamestnanec" ("jmeno", "prijmeni", "rodne_cislo", "id_pobocky")
VALUES ('Pavlína', 'Střílková', '8658049356', 3);
INSERT INTO "zamestnanec" ("jmeno", "prijmeni", "rodne_cislo", "status", "id_pobocky")
VALUES ('Denisa', 'Vondráková', '8704147309', 'V', 3);

INSERT INTO "recept" ("kod_receptu", "pojistovna", "rodne_cislo", "id_prodeje")
VALUES ('A365D785R47E', '213', '6857267395', 4);
INSERT INTO "recept" ("kod_receptu", "pojistovna", "rodne_cislo", "id_prodeje")
VALUES ('A98Z75R69D14', '111', '6505052862', 5);
INSERT INTO "recept" ("kod_receptu", "pojistovna", "rodne_cislo", "id_prodeje", "osobni_cislo")
VALUES  ('A58964F8751D', '205', '470924614', 7, 7);
INSERT INTO "recept" ("kod_receptu", "pojistovna", "rodne_cislo", "id_prodeje", "osobni_cislo")
VALUES  ('AD758F471R2C', '111', '9861077589', 8, 6);
INSERT INTO "recept" ("kod_receptu", "pojistovna", "rodne_cislo", "id_prodeje", "osobni_cislo")
VALUES  ('A5987FTR54V2', '111', '9861077589', 8, 6);

-- Provedení několika dotazů SELECT
-- ##########################################################################
-- ##########################################################################

-- 1)
-- "lek" + "prodej_lek" (SELECT 2 TABULKY)
-- Které z prodaných léků byly prodány na modrý recept?
SELECT DISTINCT "lek"."kod_leku", "lek"."nazev", "lek"."doplnkovy_nazev"
FROM "prodej_lek" JOIN "lek" ON "prodej_lek"."kod_leku" = "lek"."kod_leku"
WHERE "lek"."volne_prodejny" = 'M';

-- 2)
-- "lek" + "prodej_lek" (SELECT 2 TABULKY + COUNT)
-- Kolik zákazniků si koupilo lék dražší než 200 kč (počítá se jeden na jeden nákup)?
SELECT COUNT(*) pocet_zakazniku_50let_200kc
FROM "prodej_lek" JOIN "lek" ON "prodej_lek"."kod_leku" = "lek"."kod_leku"
WHERE "lek"."prodejni_cenna" > 200;

-- 3)
-- "lek" + "dodavka" + "dodavatel" (SELECT 3 TABULKY)
-- Jaký maji telefon firmy dodávají opiátové léky (modrý recept)?
SELECT DISTINCT "dodavatel"."jmeno_firmy", "dodavatel"."telefon"
FROM "lek" JOIN "dodavka" ON "lek"."kod_leku" = "dodavka"."kod_leku"
JOIN "dodavatel" ON "dodavka"."jmeno_firmy" = "dodavatel"."jmeno_firmy"
WHERE "lek"."volne_prodejny" = 'M';

-- 4)
-- "lek" + "prodej_lek" + "prodej" + "recept" + "zamestnanec" (SELECT 5 TABULEK)
-- Které léky, kdy a komu prodal na modrý recept "Rudolf Skála"?
SELECT "lek"."kod_leku", "lek"."nazev", "lek"."doplnkovy_nazev", "prodej"."datum", "recept"."rodne_cislo"
FROM "prodej_lek" JOIN "lek" ON "prodej_lek"."kod_leku" = "lek"."kod_leku"
JOIN "prodej" ON "prodej_lek"."id_prodeje" = "prodej"."id_prodeje"
JOIN "recept" ON "recept"."id_prodeje" = "prodej"."id_prodeje"
JOIN "zamestnanec" ON "recept"."osobni_cislo" = "zamestnanec"."osobni_cislo"
WHERE "lek"."volne_prodejny" = 'M' and "zamestnanec"."jmeno" = 'Rudolf' and "zamestnanec"."prijmeni" = 'Skála';

-- 5)
-- "dodavka" + "pobocka" (GROUP BY + agregace)
-- Kolik léků maji celkem na jednotlivých pobočkách?
SELECT "pobocka"."mesto", "pobocka"."ulice", SUM("dodavka"."mnozstvi_aktualni") celkem
FROM "dodavka" JOIN "pobocka" ON "dodavka"."id_pobocky" = "pobocka"."id_pobocky"
GROUP BY "pobocka"."mesto", "pobocka"."ulice"
ORDER BY "pobocka"."mesto", "pobocka"."ulice";

-- 6)
-- "pobocka" + "rezervace_lek" + "rezervace" + "lek" (GROUP BY + agregace)
-- Jakou tržbu můžeme očekávat na Brněnských a Pražských pobočkách po vydání všech rezervací?
SELECT "pobocka"."mesto", SUM("rezervace_lek"."mnozstvi" * "lek"."prodejni_cenna")
FROM "rezervace" JOIN "rezervace_lek" ON "rezervace"."id_rezervace" = "rezervace_lek"."id_rezervace"
JOIN "lek" ON "rezervace_lek"."kod_leku" = "lek"."kod_leku"
JOIN "pobocka" ON "rezervace"."id_pobocky" = "pobocka"."id_pobocky"
GROUP BY "pobocka"."mesto"
ORDER BY "pobocka"."mesto";

-- 7)
-- "pobocka" + "lek" + "dodavka" (IN vnoreny SELECT)
-- Které léky by se měly naskladnit na jednotlivé pobočky?
SELECT "pobocka"."mesto", "pobocka"."ulice", "lek"."kod_leku", "lek"."nazev", "lek"."doplnkovy_nazev"
FROM "lek", "pobocka"
WHERE ("lek"."kod_leku", "pobocka"."id_pobocky")  NOT IN (
    SELECT "dodavka"."kod_leku", "dodavka"."id_pobocky"
    FROM "dodavka")
ORDER BY "pobocka"."mesto", "pobocka"."ulice";

-- 8)
-- "dodavatel" + "dodavka" (NOT EXISTS)
-- Která z nasmlouvaných firem nedodává léky do Prahy?
SELECT DISTINCT "dodavatel"."jmeno_firmy"
FROM "dodavatel" JOIN "dodavka" ON "dodavatel"."jmeno_firmy" = "dodavka"."jmeno_firmy"
WHERE NOT EXISTS(
    SELECT "dodavka"."jmeno_firmy"
    FROM "dodavka"
    WHERE "dodavka"."id_pobocky" = 3 and "dodavka"."jmeno_firmy" = "dodavatel"."jmeno_firmy"
    );

-- 9)
-- "pobocka" + "dodavka" (EXISTS)
-- Na které pobočce jsou prošlé léky?
SELECT DISTINCT "pobocka"."mesto", "pobocka"."ulice"
FROM "pobocka" JOIN "dodavka" ON "pobocka"."id_pobocky" = "dodavka"."id_pobocky"
WHERE EXISTS(
    SELECT *
    FROM "dodavka"
    WHERE "dodavka"."datum_expirace" < SYSDATE and "dodavka"."id_pobocky" = "pobocka"."id_pobocky"
          );

-- Předvedení TRIGGERů
-- ##########################################################################
-- ##########################################################################
-- 1) všichni zaměstnanci, různých poboček, by měli mít vzestupně přidělené osobní čísla.
SELECT "zamestnanec"."id_pobocky", "zamestnanec"."osobni_cislo" FROM "zamestnanec";

-- 2) Přiřazení platu příjmutým zaměstnancům. Vedoucí poboček maní standartně více.
--    Zuzaně Hotaříkové ale byl stanoven plat ručně na 45 tisíc.
SELECT "zamestnanec"."jmeno", "zamestnanec"."prijmeni", "zamestnanec"."status", "zamestnanec"."plat"
FROM "zamestnanec";

-- 3) Výše příspěvku je větší než cena. Řešení, zvýšit prodejní cenu.
INSERT INTO "lek" ("kod_leku", "nazev", "doplnkovy_nazev", "ucinna_latka", "prodejni_cenna", "vyse_prispevku", "volne_prodejny")
VALUES ('6666666', 'ŠKUBALIN', '1000MG PODRAZU ', 'CHAMTIVOST', 120, 320, 'A');
SELECT "lek"."nazev", "lek"."vyse_prispevku", "lek"."prodejni_cenna" FROM "lek" WHERE "lek"."nazev" = 'ŠKUBALIN';

-- Procedury
-- ##########################################################################
-- ##########################################################################
-- 1) Procedura k povýšení lékárníka na vedoucího a přidání platu, pokud již je vedoucím,
--    zvedne se mírně plat.
CREATE OR REPLACE PROCEDURE "povyseni" ("prjm" IN VARCHAR) AS
    "stat" VARCHAR(1);
    "plt" INT;
BEGIN
    SELECT "zamestnanec"."plat", "zamestnanec"."status" INTO "plt", "stat" FROM "zamestnanec" WHERE "prijmeni" = "prjm";
    IF "stat" = 'L' THEN
        UPDATE "zamestnanec" SET "plat" = "plt" + 7000, "status" = 'V' WHERE "prijmeni" = "prjm";
    ELSE
        UPDATE "zamestnanec" SET "plat" = "plt" + 2000 WHERE "prijmeni" = "prjm";
    END IF;
END;
/
-- Paní STŘÍLKOVÁ bude povýšena a dostave přidáno.
-- Před povýšením
SELECT "zamestnanec"."jmeno", "zamestnanec"."prijmeni", "zamestnanec"."status", "zamestnanec"."plat"
FROM "zamestnanec" WHERE "prijmeni" = 'Střílková';
BEGIN "povyseni" ('Střílková'); END;
-- Po povýšení
SELECT "zamestnanec"."jmeno", "zamestnanec"."prijmeni", "zamestnanec"."status", "zamestnanec"."plat"
FROM "zamestnanec" WHERE "prijmeni" = 'Střílková';


-- 2) Procedůza k propuštění zaměstnance, pokud je propouštěným vedoucí, pak bude služebně
--    nejstarší lékárníki (nejnižší osobní číslo) povýšen do této pozice a to i s přídáním platu.
--    Pokud již není, kdo by na pobočce pracoval, dostaneme výpis upozornění.
--    EXCEPTION, %TYPE, CURSOR
CREATE OR REPLACE PROCEDURE "propustit" ("os_c" IN INT) AS
    "min_id" "zamestnanec"."osobni_cislo"%TYPE;
    "osobni_id" "zamestnanec"."osobni_cislo"%TYPE;
    "stat" "zamestnanec"."status"%TYPE;
    "stat_propousteneho" "zamestnanec"."status"%TYPE;
    "prijmeni_propousteneho" "zamestnanec"."prijmeni"%TYPE;
    "nahradnik" "zamestnanec"."prijmeni"%TYPE;
    CURSOR "zam" IS  SELECT "osobni_cislo", "status" FROM "zamestnanec" WHERE "id_pobocky" = (SELECT "zamestnanec"."id_pobocky" FROM "zamestnanec" WHERE "osobni_cislo" = "os_c");
    BEGIN
        "min_id" := 0;
        SELECT "zamestnanec"."status" INTO "stat_propousteneho" FROM "zamestnanec" WHERE "osobni_cislo" = "os_c";
        SELECT "zamestnanec"."prijmeni" INTO "prijmeni_propousteneho" FROM "zamestnanec" WHERE "osobni_cislo" = "os_c";

        "min_id" := -1;
        IF "stat_propousteneho" = 'V' THEN
            -- Potřeba stanovit nového vedoucího
            OPEN "zam";
            LOOP
                FETCH  "zam" INTO "osobni_id", "stat";
                EXIT WHEN "zam"%NOTFOUND;
                IF "osobni_id" < "min_id" OR "min_id" = -1 and "osobni_id" <> "os_c" THEN
                    "min_id" := "osobni_id";
                END IF;
            END LOOP;
            DELETE FROM "zamestnanec" WHERE "osobni_cislo" = "os_c";
            CLOSE "zam";
            SELECT "zamestnanec"."prijmeni" INTO "nahradnik" FROM "zamestnanec" WHERE "osobni_cislo" = "min_id";
            BEGIN "povyseni" ("nahradnik"); END;
        ELSE
            DELETE FROM "zamestnanec" WHERE "osobni_cislo" = "os_c";
        END IF;

    EXCEPTION WHEN NO_DATA_FOUND THEN
        BEGIN
            IF "min_id" = 0 THEN
                DBMS_OUTPUT.put_line('Neznámé ID pracovníka: ' || "os_c");
            ELSE
                DBMS_OUTPUT.put_line('Nebyla nalezena náhrada za vedoucího: ' || "prijmeni_propousteneho");
            END IF;
        END;
    END;
/
-- Vedoucí na pobočce č. 1 (osobní číslo = 3) bude propuštěn,
-- novým vedoucím by se měl stát pracovník s osobním číslem 1.
SELECT "zamestnanec"."osobni_cislo", "zamestnanec"."prijmeni", "zamestnanec"."status", "zamestnanec"."plat"
FROM "zamestnanec" WHERE "id_pobocky" = 1;
BEGIN "propustit" (3); END;
SELECT "zamestnanec"."osobni_cislo", "zamestnanec"."prijmeni", "zamestnanec"."status", "zamestnanec"."plat"
FROM "zamestnanec" WHERE "id_pobocky" = 1;


-- 3) Procedůra vyhledá pobočky, na kterých se lék s hledaným názvem nachází a vypíše jeho počet.
--    V případě nenalezení, nebo neexistence léku vypíše upozornění.
--    EXCEPTION, %TYPE, CURSOR
CREATE OR REPLACE PROCEDURE "najdi_lek" ("jmeno" IN VARCHAR) AS
    "nalezeno" NUMBER(1);
    "pocet" "dodavka"."mnozstvi_aktualni"%TYPE;
    "aktualni_kod" "lek"."nazev"%TYPE;
    "hledany_kod" "dodavka"."kod_leku"%TYPE;
    "aktualni_pobocka" "dodavka"."id_pobocky"%TYPE;
    CURSOR "dodavka_na_pobocce" IS  SELECT "kod_leku", "id_pobocky", "mnozstvi_aktualni" FROM "dodavka";
    BEGIN
        "nalezeno" := 0;
       SELECT "lek"."kod_leku" INTO "hledany_kod" FROM "lek" WHERE "nazev" = "jmeno";
       OPEN "dodavka_na_pobocce";
       LOOP
           FETCH "dodavka_na_pobocce" INTO "aktualni_kod", "aktualni_pobocka", "pocet";
           EXIT WHEN "dodavka_na_pobocce"%NOTFOUND;
           IF "aktualni_kod" = "hledany_kod" THEN
               "nalezeno" := 1;
               DBMS_OUTPUT.put_line('Lék ' || "jmeno" || ' se nachází na pobočce ' || "aktualni_pobocka"
                   || ' v počtu ' || "pocet");
           END IF;
       END LOOP;
       CLOSE "dodavka_na_pobocce";
       IF "nalezeno" = 0 THEN
           DBMS_OUTPUT.put_line('Lék ' || "jmeno" || ' nebyl nalezen na žádné pobočce');
       END IF;

       EXCEPTION WHEN NO_DATA_FOUND THEN
       BEGIN
           DBMS_OUTPUT.put_line('Lék ' || "jmeno" || ' neexistuje v databázi.');
       END;
    END;
/
-- Dotaz na nalezení léku.
BEGIN "najdi_lek"('IBALGIN'); END;
-- Vypíše varování, že daný lék neexistuje.
BEGIN "najdi_lek"('VYMYSLIN'); END;


-- EXPLAIN PLAN - INDEX
-- ##########################################################################
-- ##########################################################################
-- Vypíše kolik volně prodejných léků se stejnou účinnou látkou se nachází na jaké pobočce.
-- EXPLAIN PLAN, lek + dodavka, GROUP BY
EXPLAIN PLAN FOR
SELECT "dodavka"."id_pobocky", "lek"."ucinna_latka", SUM("dodavka"."mnozstvi_aktualni") AS "celkem"
FROM "dodavka" JOIN "lek" ON "dodavka"."kod_leku" = "lek"."kod_leku" WHERE "volne_prodejny" = 'A'
GROUP BY "dodavka"."id_pobocky", "lek"."ucinna_latka";
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

-- Přidáme index k dostupnisti (volně prodejný/na předpis) léku, protože hledáma
-- volně prodejné.
CREATE INDEX "volnost" ON "lek" ("volne_prodejny");

-- Rychlejší vyhledávání volně prodejných léků.
EXPLAIN PLAN FOR
SELECT "dodavka"."id_pobocky", "lek"."ucinna_latka", SUM("dodavka"."mnozstvi_aktualni") AS "celkem"
FROM "dodavka" JOIN "lek" ON "dodavka"."kod_leku" = "lek"."kod_leku" WHERE "volne_prodejny" = 'A'
GROUP BY "dodavka"."id_pobocky", "lek"."ucinna_latka";
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

-- PRÁVA PŘÍSTUPU
-- ##########################################################################
-- ##########################################################################
GRANT ALL ON "dodavatel" TO xpavla15;
GRANT ALL ON "dodavka" TO xpavla15;
GRANT ALL ON "lek" TO xpavla15;
GRANT ALL ON "pobocka" TO xpavla15;
GRANT ALL ON "prodej" TO xpavla15;
GRANT ALL ON "prodej_lek" TO xpavla15;
GRANT ALL ON "recept" TO xpavla15;
GRANT ALL ON "rezervace" TO xpavla15;
GRANT ALL ON "rezervace_lek" TO xpavla15;
GRANT ALL ON "zamestnanec" TO xpavla15;
GRANT ALL ON "seq_osobni_cislo" TO xpavla15;
GRANT ALL ON "user_id" TO xpavla15;

-- Povolím pouze procedůru vyhledávání léku.
-- Přijmat a propouštět zaměstnance můžu pouze JÁ.
GRANT EXECUTE ON "najdi_lek" TO xpavla15;

-- MATERIALIZOVANÝ POHLED pro xpavla15
-- ##########################################################################
-- ##########################################################################
-- Demonstrace VIEW a MATERIAL WIEW
-- Vytvoří pohled jmény volně prodejného léku, na které se vztahuje přízpěvek.
CREATE MATERIALIZED VIEW "view_lek_s_doplatkem" REFRESH ON COMMIT AS
    SELECT  "lek"."nazev",  "lek"."doplnkovy_nazev", "lek"."vyse_prispevku", "lek"."prodejni_cenna"
    FROM "lek" WHERE "volne_prodejny" = 'A' and "vyse_prispevku" > 0;
GRANT ALL ON "view_lek_s_doplatkem" TO xpavla15;

-- První výpis pohledu
SELECT * FROM "view_lek_s_doplatkem";

-- Změna v tabulce lek.
UPDATE "lek" SET "volne_prodejny" = 'N' WHERE "nazev" = 'ANALERGIN';
-- Po změně vybrané části takulky lek, se pohled nezmění, i když se jej měněná data týkají.
-- Pořád stejný pohled
SELECT * FROM "view_lek_s_doplatkem";

-- Je potřeba provést COMMIT jinak se změna neprojeví.
COMMIT;
-- Nyní se již změněná data projeví. Z tabulky ubude ANALERGIN, protože již je na předpis.
-- Aktualizovaný pohled
SELECT * FROM "view_lek_s_doplatkem";
