using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography.X509Certificates;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Razor;
using Microsoft.EntityFrameworkCore;
using Nemocnice.Data;
using Nemocnice.Models;

namespace Nemocnice.Controllers
{
    public class InsuranceEmp : Controller
    {

        private DatabaseContext Context { get; }
        public InsuranceEmp(DatabaseContext _context)
        {
            this.Context = _context;
        }
        

        public IActionResult InRequest(string searchString, string button, string buttonAll)
        {

            if (!String.IsNullOrEmpty(button))
            {
                String[] words = button.Split(" ");
                int ID = Int32.Parse(words[1]);

                if (words[0] == "schvaleno")
                {
                    var state = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID);
                    state.State = "schváleno";
                    var date = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID);
                    state.DecisionDate = DateTime.Now;
                    this.Context.SaveChanges();
                }
                else if (words[0] == "zamitnuto")
                {

                    var state = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID);
                    state.State = "zamítnuto";
                    var date = this.Context.MedicallBillT.First(a => a.MedicallBillId == ID);
                    state.DecisionDate = DateTime.Now;
                    this.Context.SaveChanges();
                }
            }

            if (!String.IsNullOrEmpty(buttonAll))
            {
                if (buttonAll == "schvalit-vse")
                {
                    List<MedicallBill> state = this.Context.MedicallBillT.Where(a => a.State == null).ToList();
                    state.ForEach(a => { a.State = "schváleno"; });
                    List<MedicallBill> date = this.Context.MedicallBillT.Where(a => a.State == null).ToList();
                    date.ForEach(a => { a.DecisionDate = DateTime.Now; ; });
                    this.Context.SaveChanges();
                }
                else if (buttonAll == "zamitnout-vse")
                {
                    List<MedicallBill> state = this.Context.MedicallBillT.Where(a => a.State == null).ToList();
                    state.ForEach(a => { a.State = "zamítnuto"; });
                    List<MedicallBill> date = this.Context.MedicallBillT.Where(a => a.State == null).ToList();
                    date.ForEach(a => { a.DecisionDate = DateTime.Now; ; });
                    this.Context.SaveChanges();
                }
            }

            if (!String.IsNullOrEmpty(searchString))
            {
                searchString = searchString.Split(' ').Last();
                var user = this.Context.UserT.Where(s => s.Name.Contains(searchString) || s.Surname.Contains(searchString)).Select(x => x.UserId).FirstOrDefault();
                var medicallBills = this.Context.MedicallBillT.Where(x => x.Doctor.UserId == user).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice);
                return View(medicallBills.ToList());

            }
            else
            {
                var medicallBills = this.Context.MedicallBillT.Where(x => x.State == null).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice);
                return View(medicallBills.ToList());
            }

        }

        public IActionResult PaymentDb(string button, string buttonUpdate, string new_butt, InsuranceModel model)
        {



            if (!String.IsNullOrEmpty(button))
            {
                String[] words = button.Split(" ");
                int ID = Int32.Parse(words[1]);

                if (words[0] == "upravit")
                {
                    
                }
                else if (words[0] == "odstranit")
                {
                    this.Context.Remove(this.Context.MedicallActivityPriceT.Single(a => a.MedicallActivityPriceId == ID));
                    this.Context.SaveChanges();
                }
            }

            var db = new DatabaseContext();
            model.medicallActivityPrice = this.Context.MedicallActivityPriceT.ToList();


            if (!ModelState.IsValid)
            {
                return View();
            }
            else
            {

                if (!String.IsNullOrEmpty(new_butt))
                {
                    var activityPrice = new MedicallActivityPrice { Name = model.nazev, Amount = model.cena };
                    db.Add<MedicallActivityPrice>(activityPrice);
                    db.SaveChanges();
                }


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
}
