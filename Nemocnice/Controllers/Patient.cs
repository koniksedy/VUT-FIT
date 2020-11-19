using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Nemocnice.Data;
using Nemocnice.Models;
using System.Linq;
using X.PagedList.Mvc.Core;
using cloudscribe.Pagination.Models;
using X.PagedList;

namespace Nemocnice.Controllers
{
    [Authorize(Roles = "Patient")]
    public class Patient : Controller
    {
        public IActionResult Index()
        {
            return View();
        }

        public async System.Threading.Tasks.Task<IActionResult> PatientCardAsync(PatientCardModel PatientCardModel, int? page1, int? page2, int? page3, int? select)
        {
            var db = new DatabaseContext();

            var user = User.Identity.Name;



            
            var query = db.MedicallBillT.AsNoTracking().Where(x => x.State == null).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice).OrderByDescending(s => s.CreateDate);
            PatientCardModel.User = db.UserT.Where(x => x.Login == user).Include(s => s.WorkAddress).FirstOrDefault();
            PatientCardModel.Patient = db.PatientT.Where(x => x.UserId == PatientCardModel.User.UserId).Include(s => s.HomeAddress).Include(s => s.HealthCondition).FirstOrDefault();
            PatientCardModel.medicallActivityPrice = db.MedicallActivityPriceT.ToList();

            PatientCardModel.Records1 = db.MedicallBillT.Where(x => x.State == null).Count();

            
            PatientCardModel.PageNum1 = (page1 ?? 1);
            int pageSize = 3;
            PatientCardModel.PageNum2 = (page2 ?? 1);
            PatientCardModel.PageNum3 = (page3 ?? 1);
            PatientCardModel.TabNumber = (select ?? 1);
            IPagedList<MedicallActivityPrice> lide1 = PatientCardModel.medicallActivityPrice.ToPagedList(PatientCardModel.PageNum1, pageSize);
            IPagedList<MedicallActivityPrice> lide2 = PatientCardModel.medicallActivityPrice.ToPagedList(PatientCardModel.PageNum2, pageSize);
            IPagedList<MedicallActivityPrice> lide3 = PatientCardModel.medicallActivityPrice.ToPagedList(PatientCardModel.PageNum3, pageSize);
            PatientCardModel.medicallBills1 = lide1;
            PatientCardModel.medicallBills2 = lide2;
            PatientCardModel.medicallBills3 = lide3;
            return View(PatientCardModel);
        }

        public async System.Threading.Tasks.Task<IActionResult> CheckupAsync(PatientCheckupModel PatientCheckupModel, int? tab1, int? tab2)
        {
            var db = new DatabaseContext();

            var user = User.Identity.Name;

            var query = db.CheckupTicketT.Where(x => x.State == "dokončeno").OrderByDescending(s => s.CreateDate);

            PatientCheckupModel.User = db.UserT.Where(x => x.Login == user).Include(s => s.WorkAddress).FirstOrDefault();
            PatientCheckupModel.Patient = db.PatientT.Where(x => x.UserId == PatientCheckupModel.User.UserId).Include(s => s.HomeAddress).Include(s => s.HealthCondition).FirstOrDefault();
            //PatientCheckupModel.CheckupTicketsDone = db.CheckupTicketT.Where(x => x.State == "dokončeno" ).ToList();

            PatientCheckupModel.tab1 = (tab1 ?? 1);
            PatientCheckupModel.tab2 = (tab2 ?? 1);
            int pageSize = 3;

            PatientCheckupModel.CheckupTicketsRunning = db.CheckupTicketT.Where(x => x.State != "dokončeno").ToPagedList(PatientCheckupModel.tab1, pageSize);
            PatientCheckupModel.CheckupTicketsDone = db.CheckupTicketT.Where(x => x.State == "dokončeno").ToPagedList(PatientCheckupModel.tab2, pageSize);



            return View(PatientCheckupModel);
        }

        public IActionResult Detail(int currentTicketID)
        {
            var db = new DatabaseContext();
            var ticket = db.CheckupTicketT.Where(x => x.CheckupTicketId == currentTicketID).Include(x => x.CreatedBy).Include(x => x.ToDoctor).Include(x => x.Patient).FirstOrDefault();
            return View(ticket);
        }
    }


}
