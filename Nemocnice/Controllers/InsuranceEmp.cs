/*
 * Soubor kontroleru uživatele s rolí pracovník pojišťovny nebo admin.
 * Autor: Kateřina Kunorzová <xkunor00>
 * Poslední úprava: 20.11.2020
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
using ReflectionIT.Mvc.Paging;
using Microsoft.AspNetCore.Authorization;


namespace Nemocnice.Controllers
{
    /*
     * Třída, která zpracuje požadavky uživatele s rolí pracovník pojišťovny nebo doktor,
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
        public async Task<IActionResult> InRequest(int ID_accept, int ID_reject, string searchString, string SortOrder, string buttonAll, InsuranceModel model, int p = 1)
        {
            //uložení aktuálně vyhledaného řetězce
            ViewData["Search"] = searchString;

            //pokud je různé od 0, byla schválena žádost s daným ID a tu d datbázi změníme z null na "schváleno"
            if (ID_accept != 0)
            {
                var state = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID_accept);
                state.State = "schváleno";
                var date = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID_accept);
                state.DecisionDate = DateTime.Now;
                this.Context.SaveChanges();
            }

            //pokud je různé od 0, byla zamítnuta žádost s daným ID a tu d datbázi změníme z null na "zamítnuto"
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
                searchString = searchString.Split(' ').Last();
                var user = this.Context.UserT.Where(s => s.Name.StartsWith(searchString) || s.Surname.StartsWith(searchString)).Select(x => x.UserId).FirstOrDefault();
                model.medicallBills = this.Context.MedicallBillT.Where(i => i.State==null && i.Doctor.UserId == user).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice).OrderByDescending(o => o.CreateDate).ToList();                
            }
            //pokud je vyhledávací řetězec prázdný, vypisujeme všechny nerozhodnuté žádosti -> ty, co moají stav null
            else
            {
                var query = this.Context.MedicallBillT.AsNoTracking().Where(x => x.State == null).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice).OrderByDescending(s => s.CreateDate);

                //řazení dle následujících kritériíí
                switch (SortOrder)
                {
                    case "cena":
                        query = query.OrderBy(o => o.MedicallActivityPrice.Amount);
                        break;
                    case "diagnoza":
                        query = query.OrderBy(o => o.Diagnosis.Name);
                        break;
                    default:
                        query = query.OrderByDescending(o => o.CreateDate);
                        break;
                }

                //stránkování vybraných dat
                model.medicallBills = await PagingList.CreateAsync(query, 5, p);
                model.Records = this.Context.MedicallBillT.Where(x => x.State == null).Count();
                model.PageNum = p;
                model.PageSize = 5;
                
            }
            
            //pokud je string naplněný, schválí/zamítne se vše na dané stránce
            if (!String.IsNullOrEmpty(buttonAll))
            {
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
                var users = this.Context.UserT.Where(s => s.Name.Contains(searchString) || s.Surname.Contains(searchString)).Select(x => x.UserId).ToList();
                
                //zde se vyberou a uloží vybrané položky (ve vyhledávacím řetězci) databáze
                List<MedicallBill> state = new List<MedicallBill>();
                foreach (var user in users)
                {
                    List<MedicallBill> temp;
                    temp = this.Context.MedicallBillT.Where(a => a.State == null && a.Doctor.UserId == user).ToList();
                    state.AddRange(temp);
                }

                //pokud je string "schvalit-vse" schvalujeme vše
                if (buttonAll == "schvalit-vse")
                {
                    state.ForEach(a => { a.State = "schváleno"; });
                    List<MedicallBill> date = this.Context.MedicallBillT.Where(a => a.State == null).ToList();
                    date.ForEach(a => { a.DecisionDate = DateTime.Now; ; });
                    this.Context.SaveChanges();
                }

                //pokud je string "zamitnnout-vse" zamítneme vše
                if (buttonAll == "zamitnout-vse")
                {
                    state.ForEach(a => { a.State = "zamítnuto"; });
                    List<MedicallBill> date = this.Context.MedicallBillT.Where(a => a.State == null).ToList();
                    date.ForEach(a => { a.DecisionDate = DateTime.Now; ; });
                    this.Context.SaveChanges();
                }
            }



            return View(model);

        }

     /*
     * Akce vypíše tabulku s úkony, které se proplácí
     * ID_delete - ID úkonu, který se z tabulky vymaže
     * sortOrder - typ řazení (dle jména doktora, data nebo diagnózy)
     * buttonUpdate - ID úkonu, který bude upraven
     * new_butt - ID s nově vytvořeným úkonem
     * model - model pro uložení vybraných dat
     * p - proměnná pro stránkování
     */
        public async Task<IActionResult> PaymentDb(int ID_delete, string SortOrder, string buttonUpdate, string new_butt, InsuranceModel model, int p = 1)
        {

            //pokud je ID_delete různé od 0, víme, jaký úkon s daným ID máme vymazat
            if (ID_delete != 0)
            {
                this.Context.Remove(this.Context.MedicallActivityPriceT.Single(a => a.MedicallActivityPriceId == ID_delete));
                this.Context.SaveChanges();
            }

            //nachystání celé tabulky se všemi úkony k vypsání
            var query = this.Context.MedicallActivityPriceT.AsNoTracking().OrderBy(o => o.Name);

            //řazení dle následujících kritériíí
            switch (SortOrder)
            {
                case "cena":
                    query = query.OrderBy(o => o.Amount);
                    break;
                default:
                    query = query.OrderBy(o => o.Name);
                    break;
            }

            //stránkování vybraných dat
            model.medicallActivityPrice = await PagingList.CreateAsync(query, 5, p);
            model.Records = this.Context.MedicallActivityPriceT.Count();
            model.PageNum = p;
            model.PageSize = 5;

        
            //pokud následující string něco obsahuje, jedná se o přidání nového úkonu
            if (!String.IsNullOrEmpty(new_butt))
            {
                var activityPrice = new MedicallActivityPrice { Name = model.nazev, Amount = model.cena };
                this.Context.Add<MedicallActivityPrice>(activityPrice);
                this.Context.SaveChanges();
            }

            //pokud následující string něco obsahuje, jedná se o update daného úkonu
            if (!String.IsNullOrEmpty(buttonUpdate))
            {
                int ID = Int32.Parse(buttonUpdate);
                var pom = this.Context.MedicallActivityPriceT.First(a => a.MedicallActivityPriceId == ID);
                pom.Name = model.nazevZmena; 
                pom.Amount = model.cenaZmena;
                this.Context.SaveChanges();
            }

             return View(model);
            
        }
    }
}
