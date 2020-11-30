using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Nemocnice.Data;
using Nemocnice.Models;
using System.Linq;
using X.PagedList.Mvc.Core;

using System;
using System.Collections.Generic;
using X.PagedList;

namespace Nemocnice.Controllers
{
    [Authorize(Roles = "Patient,Admin")]
    public class Patient : Controller
    {
        public IActionResult Index()
        {
            return View();
        }
        public static IPagedList<T> Empty<T>()
        {
            return new List<T>().ToPagedList(0, 1);
        }

        public IActionResult PatientCard(PatientCardModel PatientCardModel, int? pageReport, int? pageCheckup, int? pageAllergies, int? pageBill, int? pageTreatmentLogs, int? pageCure, int? select)
        {
            var db = new DatabaseContext();

            var user = User.Identity.Name;




            var query = db.MedicallBillT.AsNoTracking().Where(x => x.State == null).Include(s => s.Doctor).Include(s => s.Diagnosis).Include(s => s.MedicallActivityPrice).OrderByDescending(s => s.CreateDate);
            PatientCardModel.User = db.UserT.Where(x => x.Login == user).Include(s => s.WorkAddress).FirstOrDefault();
            PatientCardModel.Patient = db.PatientT.Where(x => x.UserId == PatientCardModel.User.UserId).Include(s => s.HomeAddress).Include(s => s.HealthCondition).FirstOrDefault();
            // PatientCardModel.medicallActivityPrice = db.MedicallActivityPriceT.ToList();

            // PatientCardModel.Records1 = db.MedicallBillT.Where(x => x.State == null).Count();

            int pageSize = 5;
            PatientCardModel.PageNum1 = (pageReport ?? 1);
            PatientCardModel.PageNum2 = (pageCheckup ?? 1);
            PatientCardModel.PageNum3 = (pageAllergies ?? 1);
            PatientCardModel.PageNum4 = (pageBill ?? 1);
            PatientCardModel.PageNum5 = (pageTreatmentLogs ?? 1);
            PatientCardModel.PageNum6 = (pageCure ?? 1);
            PatientCardModel.TabNumber = (select ?? 1);

            PatientCardModel.Pictures = db.PictureT.ToPagedList(PatientCardModel.PageNum3, pageSize);//.Where(x => x.SocialSecurityNum == PatientCardModel.Patient.SocialSecurityNum).ToPagedList();
            PatientCardModel.medicallReports = db.MedicallReportT.Include(x => x.Author).Include(x => x.Patient).Include(x => x.Owner).ToPagedList(PatientCardModel.PageNum1, pageSize);//Where(x => x.Patient.UserId == PatientCardModel.User.UserId).ToPagedList();
            PatientCardModel.checkupTickets = db.CheckupTicketT.Include(x => x.ToDoctor).Include(x => x.Patient).Include(x => x.CreatedBy).ToPagedList(PatientCardModel.PageNum2, pageSize);//.Where(x => x.Patient.UserId == PatientCardModel.User.UserId).ToPagedList();
            try
            {
                IPagedList<string> hotovo;
                var list = db.HealthConditionT.Where(x => x.SocialSecurityNum == PatientCardModel.Patient.SocialSecurityNum).Select(x => x.Allergys).DefaultIfEmpty("").ToPagedList();


                
                PatientCardModel.Allergies = list;
            }
            catch (Exception)
            {

            }

            //Where(x => x.SocialSecurityNum == PatientCardModel.Patient.SocialSecurityNum).ToPagedList();
            PatientCardModel.MedicallBills = db.MedicallBillT.Include(x => x.Doctor).Include(x => x.MedicallActivityPrice).Include(x => x.Diagnosis).ToPagedList(PatientCardModel.PageNum4, pageSize);//Where(x => x.SocialSecurityNum == PatientCardModel.Patient.SocialSecurityNum).ToPagedList();
            PatientCardModel.PatientTreatmentLogs = db.PatientTreatmentLogT.Include(x => x.Patient).Include(x => x.Diagnosis).ToPagedList(PatientCardModel.PageNum5, pageSize);//Where(x => x.Patient.UserId == PatientCardModel.Patient.UserId).ToPagedList();
            PatientCardModel.CureProgresses = db.CureProgressT.Include(x => x.MedicallReport).Include(x => x.MedicallReport.Owner).Include(x => x.Diagnosis).ToPagedList(PatientCardModel.PageNum6, pageSize); //Where(x => x.MedicallReport.Patient.UserId == PatientCardModel.Patient.UserId).ToPagedList();
            return View(PatientCardModel);
        }

        public IActionResult Checkup(PatientCheckupModel PatientCheckupModel, int? tab1, int? tab2)
        {
            var db = new DatabaseContext();

            var user = User.Identity.Name;

            var query = db.CheckupTicketT.Where(x => x.State == "dokončeno").OrderByDescending(s => s.CreateDate);

            PatientCheckupModel.User = db.UserT.Where(x => x.Login == user).Include(s => s.WorkAddress).FirstOrDefault();
            PatientCheckupModel.Patient = db.PatientT.Where(x => x.UserId == PatientCheckupModel.User.UserId).Include(s => s.HomeAddress).Include(s => s.HealthCondition).FirstOrDefault();

            PatientCheckupModel.tab1 = (tab1 ?? 1);
            PatientCheckupModel.tab2 = (tab2 ?? 1);
            int pageSize = 4;

            PatientCheckupModel.CheckupTicketsRunning = db.CheckupTicketT.Include(x => x.ToDoctor).Include(x => x.Patient).Include(x => x.CreatedBy).Where(x => x.State != "dokončeno").ToPagedList(PatientCheckupModel.tab1, pageSize);
            PatientCheckupModel.CheckupTicketsDone = db.CheckupTicketT.Include(x => x.ToDoctor).Include(x => x.Patient).Include(x => x.CreatedBy).Where(x => x.State == "dokončeno").ToPagedList(PatientCheckupModel.tab2, pageSize);



            return View(PatientCheckupModel);
        }

        public IActionResult Detail(int currentTicketID)
        {
            var db = new DatabaseContext();
            var ticket = db.CheckupTicketT.Include(x => x.CreatedBy).Include(x => x.ToDoctor).Include(x => x.Patient).Where(x => x.CheckupTicketId == currentTicketID).FirstOrDefault();
            return View(ticket);
        }
        public IActionResult DetailMedRep(int currentTicketID)
        {
            var db = new DatabaseContext();
            var ticket = db.MedicallReportT.Include(x => x.Author).Include(x => x.Patient).Include(x => x.Owner).Where(x => x.MedicallReportId == currentTicketID).FirstOrDefault();
            return View(ticket);
        }

        public IActionResult CheckupTicket(int CheckupTicketId)
        {
            var db = new DatabaseContext();
            var CureProg = db.CheckupTicketT.Include(x => x.CreatedBy).Include(x => x.Patient).Where(x => x.CheckupTicketId == CheckupTicketId).First();
            return View(CureProg);
        }
    }
}
