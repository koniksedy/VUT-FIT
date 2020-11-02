using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography.X509Certificates;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Nemocnice.Data;

namespace Nemocnice.Controllers
{
    public class InsuranceEmp : Controller
    {

        private DatabaseContext Context { get; }
        public InsuranceEmp(DatabaseContext _context)
        {
            this.Context = _context;
        }
        

        public IActionResult InRequest(string searchString, string button)
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

        public IActionResult PaymentDb(string button)
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

            var medicallActivityPrice = this.Context.MedicallActivityPriceT.ToList();
            return View(medicallActivityPrice);
        }
    }
}
