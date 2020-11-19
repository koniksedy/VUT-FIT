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
    [Authorize(Roles = "Insurance")]
    public class InsuranceEmp : Controller
    {

        private DatabaseContext Context { get; }
        public InsuranceEmp(DatabaseContext _context)
        {
            this.Context = _context;
        }


        public async Task<IActionResult> InRequest(string searchString, string SortOrder, string button, string buttonAll, InsuranceModel model, int p = 1)
        {
            /* OBNOVA DAT */
            /*
            List<MedicallBill> a = this.Context.MedicallBillT.Where(a => a.State == "schváleno").ToList();
            a.ForEach(a => { a.State =null; });
            List<MedicallBill> b = this.Context.MedicallBillT.Where(a => a.State == null).ToList();
            b.ForEach(a => { a.DecisionDate = DateTime.Now; ; });
            this.Context.SaveChanges();
            */

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
                model.medicallBills = this.Context.MedicallBillT.Where(i => i.State==null && i.Doctor.UserId == user).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice).OrderByDescending(o => o.CreateDate).ToList();
              

            }
            else
            {
                var query = this.Context.MedicallBillT.AsNoTracking().Where(x => x.State == null).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice).OrderByDescending(s => s.CreateDate);

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

                model.medicallBills = await PagingList.CreateAsync(query, 5, p);
                model.Records = this.Context.MedicallBillT.Where(x => x.State == null).Count();
                model.PageNum = p;
                model.PageSize = 5;
                
            }

            return View(model);

        }

        public async Task<IActionResult> PaymentDb(string button, string SortOrder, string buttonUpdate, string new_butt, InsuranceModel model, int p = 1)
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

        

            var query = this.Context.MedicallActivityPriceT.AsNoTracking().OrderBy(o => o.Name);

            switch (SortOrder)
            {
                case "cena":
                    query = query.OrderBy(o => o.Amount);
                    break;
                default:
                    query = query.OrderBy(o => o.Name);
                    break;
            }

            model.medicallActivityPrice = await PagingList.CreateAsync(query, 5, p);
            model.Records = this.Context.MedicallActivityPriceT.Count();
            model.PageNum = p;
            model.PageSize = 5;

        

            if (!ModelState.IsValid)
            {
                return View();
            }
            else
            {

                if (!String.IsNullOrEmpty(new_butt))
                {
                    var activityPrice = new MedicallActivityPrice { Name = model.nazev, Amount = model.cena };
                    this.Context.Add<MedicallActivityPrice>(activityPrice);
                    this.Context.SaveChanges();
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
