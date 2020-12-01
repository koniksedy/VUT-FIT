/*
 * Soubor kontroleru uživatele s rolí pracovník pojišťovny nebo admin.
 * Autor: Kateřina Kunorzová <xkunor00>
 * Poslední úprava: 1.12.2020
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography.X509Certificates;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Razor;
using Microsoft.EntityFrameworkCore;
using Nemocnice.Data;
using Nemocnice.Models;
using Microsoft.AspNetCore.Authorization;
using X.PagedList.Mvc.Core;
using X.PagedList;


namespace Nemocnice.Controllers
{
    /*
     * Třída, která zpracuje požadavky uživatele s rolí pracovník pojišťovny nebo admin,
     * k jednotlivým akcím může přistoupit jen uživatel s těmito rolemi.
     */
    [Authorize(Roles = "Insurance,Admin")]
    public class InsuranceEmp : Controller
    {

        /*
         * Proměnná, která umožňuje přístup do databáze
         */
        private DatabaseContext Context { get; }
        public InsuranceEmp(DatabaseContext _context)
        {
            this.Context = _context;
        }

        /*
         * Akce vypíše všechny nerozhodnuté žádosti o proplacení
         * ID_accept - ID žádosti, která byla schválena
         * ID_reject - ID žádosti, která byla zamítnuta
         * searchString - vyhledávací řetězec
         * sortOrder - typ řazení (dle jména doktora, data nebo diagnózy)
         * buttonAll - značí, jestli uživatel schválil/zamítl všechny položky na dané stránce
         * model - model pro uložení vybraných dat
         * p - proměnná pro stránkování
         */
        public IActionResult InRequest(int ID_accept, int ID_reject, string searchString, string SortOrder, string buttonAll, InsuranceModel model, int? p)
        {
  
            //pokud je string naplněný, schválí/zamítne se vše na dané stránce
            if (!String.IsNullOrEmpty(buttonAll))
            {
                //uložení aktuálně vyhledaného řetězce, vyhledávání a stránky
                ViewData["Search"] = "";
                ViewData["CurrentSort"] = "";
                ViewData["CurrentPage"] = 1;

                //searchString je prázdný, pracujeme se vším
                if (String.IsNullOrEmpty(searchString))
                {
                    searchString = "";
                }
                //searchString něco obsahujeme, pracujeme jen s tím, co bylo vyhledáno
                else
                {
                    searchString = (searchString.Split(' ').Last() ?? "");
                }

                List<MedicallBill> medicalBills;
                try
                {
                    medicalBills = this.Context.MedicallBillT.Where(s => s.State == null && (s.Doctor.ICZ == int.Parse(searchString))).ToList();
                }
                catch
                {
                    medicalBills = this.Context.MedicallBillT.Where(s => s.State == null && (s.MedicallActivityPrice.Name.Contains(searchString) || s.Diagnosis.Name.Contains(searchString))).ToList();
                }

                //pokud je string "schvalit-vse" schvalujeme vše
                if (buttonAll == "schvalit-vse")
                {
                    medicalBills.ForEach(a => { a.State = "schváleno"; });
                    List<MedicallBill> date = this.Context.MedicallBillT.Where(a => a.State == null).ToList();
                    date.ForEach(a => { a.DecisionDate = DateTime.Now; ; });
                    this.Context.SaveChanges();
                }

                //pokud je string "zamitnnout-vse" zamítneme vše
                if (buttonAll == "zamitnout-vse")
                {
                    medicalBills.ForEach(a => { a.State = "zamítnuto"; });
                    List<MedicallBill> date = this.Context.MedicallBillT.Where(a => a.State == null).ToList();
                    date.ForEach(a => { a.DecisionDate = DateTime.Now; ; });
                    this.Context.SaveChanges();
                }
                p = 1;
                searchString = "";
                SortOrder = "";
            }
            else
            {
                //uložení aktuálně vyhledaného řetězce, vyhledávání a stránky
                ViewData["Search"] = searchString;
                ViewData["CurrentSort"] = SortOrder;
                ViewData["CurrentPage"] = p;
            }

            //pokud je různé od 0, byla schválena žádost s daným ID a tu v datbázi změníme z null na "schváleno"
            if (ID_accept != 0)
            {
                var state = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID_accept);
                state.State = "schváleno";
                var date = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID_accept);
                state.DecisionDate = DateTime.Now;
                this.Context.SaveChanges();
            }

            //pokud je různé od 0, byla zamítnuta žádost s daným ID a tu v datbázi změníme z null na "zamítnuto"
            if (ID_reject != 0)
            {
                var state = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID_reject);
                state.State = "zamítnuto";
                var date = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID_reject);
                state.DecisionDate = DateTime.Now;
                this.Context.SaveChanges();
            }
            
            //pokud vyhledávací řetězec něco obsahuje, podle jeho obsahu provedeme vyhledání v databázi
            if (!String.IsNullOrEmpty(searchString))
            {
                try {
                    model.medicallBills = this.Context.MedicallBillT.Where(s => s.State == null && (s.Doctor.ICZ == int.Parse(searchString))).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice).ToList();
                }
                catch
                {
                    model.medicallBills = this.Context.MedicallBillT.Where(s => s.State == null && (s.MedicallActivityPrice.Name.Contains(searchString) || s.Diagnosis.Name.Contains(searchString))).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice).ToList();
                }
            }
            //pokud je vyhledávací řetězec prázdný, vypisujeme všechny nerozhodnuté žádosti -> ty, co mají stav null
            else
            {
                model.medicallBills = this.Context.MedicallBillT.Where(x => x.State == null).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice).ToList();
            }

            //řazení dle následujících kritériíí
            switch (SortOrder)
            {
                case "cena":
                    model.medicallBills = model.medicallBills.OrderBy(o => o.MedicallActivityPrice.Amount).ToList();
                    break;
                case "typ":
                    model.medicallBills = model.medicallBills.OrderBy(o => o.MedicallActivityPrice.Name).ToList();
                    break;
                case "diagnoza":
                    model.medicallBills = model.medicallBills.OrderBy(o => o.Diagnosis.Name).ToList();
                    break;
                default:
                    model.medicallBills = model.medicallBills.OrderByDescending(o => o.CreateDate).ToList();
                    break;
            }


            //stránkování vybraných dat
            model.PageNum = (p ?? 1);
            int pageSize = 5;
            IPagedList<MedicallBill> lide = model.medicallBills.ToPagedList(model.PageNum, pageSize);
            model.medicallBillsPage = lide;



            return View(model);

        }


         /*
         * Akce upravuje položky v databázi ceníku úkonů
         */
        [HttpPost]
        public IActionResult EditDb()
        {
            //vytáhneme z formuláře zadaná data a těmi přepíšeme databázi
            int edit_ID = int.Parse(Request.Form["edit_ID"]);
            string edit_name = Request.Form["edit_name"];
            decimal edit_amount = decimal.Parse(Request.Form["edit_amount"].ToString().Replace(".", ","));

            var pom = this.Context.MedicallActivityPriceT.First(a => a.MedicallActivityPriceId == edit_ID);
            pom.Name = edit_name;
            pom.Amount = edit_amount;
            this.Context.SaveChanges();

            return RedirectToAction("PaymentDb", new { SortOrder = Request.Form["SortOrder"], p1 = Request.Form["p1"], searchString = Request.Form["searchString"] });
        }


        /*
        * Akce přidává položky do ceníku úkonů
        */
        [HttpPost]
        public IActionResult NewDb()
        {

            //vytáhneme z formuláře zadaná data a ty vložíme do databáze
            string new_name = Request.Form["new_name"];
            decimal new_amount = decimal.Parse(Request.Form["new_amount"].ToString().Replace(".", ","));

            var activityPrice = new MedicallActivityPrice { Name = new_name, Amount = new_amount };
            this.Context.Add<MedicallActivityPrice>(activityPrice);
            this.Context.SaveChanges();

            return RedirectToAction("PaymentDb", new { SortOrder = Request.Form["SortOrder"], p1 = Request.Form["p1"], searchString = Request.Form["searchString"] });
        }



        /*
        * Akce vypíše tabulku s úkony, které se proplácí
        * ID_delete - ID úkonu, který se z tabulky vymaže
        * sortOrder - typ řazení (dle jména doktora, data nebo diagnózy)
        * searchString - vyhledávací řetězec
        * model - model pro uložení vybraných dat
        * p - proměnná pro stránkování
        */
        public IActionResult PaymentDb(int ID_delete, string SortOrder, string searchString, InsuranceModel model, int ? p1)
        {

            //uložení aktuálně vyhledaného řetězce, vyhledávání a stránky
            ViewData["CurrentSort"] = SortOrder;
            ViewData["CurrentPage"] = p1;
            ViewData["Search"] = searchString;

            //pokud je ID_delete různé od 0, víme, jaký úkon s daným ID máme vymazat
            if (ID_delete != 0)
            {
                this.Context.Remove(this.Context.MedicallActivityPriceT.Single(a => a.MedicallActivityPriceId == ID_delete));
                this.Context.SaveChanges();
            }

            //pokud vyhledávací řetězec něco obsahuje, provedeme vyhledávání
            if (!String.IsNullOrEmpty(searchString))
            {
                model.medicallActivityPrice = this.Context.MedicallActivityPriceT.Where(s => s.Name.Contains(searchString)).OrderBy(o => o.Name).ToList();
            }
            //v opačném případě proběhně nachystání celé tabulky se všemi úkony k vypsání
            else
            {   
                model.medicallActivityPrice = this.Context.MedicallActivityPriceT.OrderBy(o => o.Name).ToList();
            }

            //řazení dle následujících kritériíí
            switch (SortOrder)
            {
                case "cena":
                    model.medicallActivityPrice = model.medicallActivityPrice.OrderBy(o => o.Amount).ToList();
                    break;
                default:
                    model.medicallActivityPrice = model.medicallActivityPrice.OrderBy(o => o.Name).ToList();
                    break;
            }

            //stránkování vybraných dat
            model.PageNum1 = (p1 ?? 1);
            int pageSize = 5;
            IPagedList<MedicallActivityPrice> lide = model.medicallActivityPrice.ToPagedList(model.PageNum1, pageSize);
            model.medicallActivityPricePage = lide;

             return View(model);
            
        }



       /*
       * Akce upravuje položky v databázi diagnóz
       */
        [HttpPost]
        public IActionResult EditDb_Dia()
        {

            //vytáhneme z formuláře zadaná data a těmi přepíšeme databázi
            int edit_ID = int.Parse(Request.Form["edit_ID"]);
            string edit_name = Request.Form["edit_name"];

            var pom = this.Context.DiagnosisT.First(a => a.DiagnosisId == edit_ID);
            pom.Name = edit_name;
            this.Context.SaveChanges();

            return RedirectToAction("Diagnosis", new { p2 = Request.Form["p2"] });
        }

        /*
        * Akce přídává nové položky v databázi diagnóz
        */
        [HttpPost]
        public IActionResult NewDb_Dia()
        {
            //vytáhneme z formuláře zadaná data a ty vložíme do databáze
            string new_name = Request.Form["new_name"];
            var diagnosis = new Diagnosis { Name = new_name };
            this.Context.Add<Diagnosis>(diagnosis);
            this.Context.SaveChanges();

            return RedirectToAction("Diagnosis", new { p2 = Request.Form["p2"] });
        }


        /*
        * Akce vypíše tabulku diagnóz
        * ID_delete - ID úkonu, který se z tabulky vymaže
        * model - model pro uložení vybraných dat
        * p - proměnná pro stránkování
        */
        public IActionResult Diagnosis(int ID_delete, InsuranceModel model, int? p2)
        {
            //uložení aktuálně vyhledaného řetězce
            ViewData["CurrentPage"] = p2;

            //pokud je ID_delete různé od 0, víme, jaký úkon s daným ID máme vymazat
            if (ID_delete != 0)
            {
                this.Context.Remove(this.Context.DiagnosisT.Single(a => a.DiagnosisId == ID_delete));
                this.Context.SaveChanges();
            }

            //nachystání celé tabulky se všemi diagnózami k vypsání
            model.diagnosis = this.Context.DiagnosisT.OrderBy(o => o.Name).ToList();


            //stránkování vybraných dat
            model.PageNum2 = (p2 ?? 1);
            int pageSize = 5;
            IPagedList<Diagnosis> lide = model.diagnosis.ToPagedList(model.PageNum2, pageSize);
            model.diagnosisPage = lide;

            return View(model);

        }
    }
}
