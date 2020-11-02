using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using Nemocnice.Data;
using Nemocnice.Models;

namespace Nemocnice.Controllers
{
    [Authorize(Roles = "Doctor")]
    public class Doctor : Controller
    {
        private readonly UserManager<NemocniceUser> _userManager;

        public Doctor(UserManager<NemocniceUser> userManager)
        {
            _userManager = userManager;
        }


        public IActionResult Index()
        {
            return View();
        }

        public IActionResult CreatePatient()
        {
            return View();
        }

        public IActionResult Card(string sortOrder, string searchString)
        {
            ViewData["Search"] = searchString;

            var db = new DatabaseContext();
            List<CardPatientModel> Patients;

            if (String.IsNullOrEmpty(searchString))
            {
                Patients =
                    db.PatientT.Join(
                        db.UserT,
                        patient => patient.PatientID,
                        user => user.UserId,
                        (patient, user) => new CardPatientModel
                        {
                            Surname = user.Surname,
                            Name = user.Name,
                            SocialSecurityNum = patient.SocialSecurityNum
                        }
                        ).ToList();
            }
            else
            {
                Int64 parseRes;
                if(Int64.TryParse(searchString, out parseRes))
                {
                    Patients =
                    db.PatientT.Join(
                        db.UserT,
                        patient => patient.PatientID,
                        user => user.UserId,
                        (patient, user) => new CardPatientModel
                        {
                            Surname = user.Surname,
                            Name = user.Name,
                            SocialSecurityNum = patient.SocialSecurityNum
                        }
                        ).Where(s => s.SocialSecurityNum == parseRes).ToList();
                }
                else
                {
                    Patients =
                        db.PatientT.Join(
                            db.UserT,
                            patient => patient.PatientID,
                            user => user.UserId,
                            (patient, user) => new CardPatientModel
                            {
                                Surname = user.Surname,
                                Name = user.Name,
                                SocialSecurityNum = patient.SocialSecurityNum
                            }
                            ).Where(s => s.Name == searchString || s.Surname == searchString).ToList();
                }
            }

            switch (sortOrder)
            {
                case "byName":
                    Patients = Patients.OrderBy(o => o.Name).ToList();
                    break;
                case "byNumber":
                    Patients = Patients.OrderBy(o => o.SocialSecurityNum).ToList();
                    break;
                default:
                    Patients = Patients.OrderBy(o => o.Surname).ToList();
                    break;
            }
 
            return View(Patients);
        }

        public IActionResult Activity(string userName)
        {
            var db = new DatabaseContext();

            string user = User.FindFirstValue(ClaimTypes.Name);
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();

            List<ActivityModel> Activities = new List<ActivityModel>();

            if (id_list.Any())
            {


                Activities = db.MedicallBillT.Where(o => o.Doctor.UserId == id_list.First())
                                                        .Select(s => new ActivityModel
                                                        {
                                                            ActivityName = s.MedicallActivityPrice.Name,
                                                            Value = s.MedicallActivityPrice.Amount,
                                                            PatientNum = s.SocialSecurityNum,
                                                            CreateDate = s.CreateDate,
                                                            State = s.State,
                                                        }).ToList();
            }
                             
            return View(Activities);
        }

        public IActionResult Requests()
        {
            return View();
        }

        public IActionResult InRequest()
        {
            var db = new DatabaseContext();

            List<CheckupTicketToMe> MyInRequests = new List<CheckupTicketToMe>();

            string user = User.FindFirstValue(ClaimTypes.Name);
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();


            
            if(id_list.Any())
            {
                MyInRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno").Where(o => o.ToDoctor.UserId == id_list.First())
                .Select(s => new CheckupTicketToMe
                {
                    PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.PatientID).Select(p => p.Surname).First(),
                    PatientName = db.UserT.Where(o => o.UserId == s.Patient.PatientID).Select(p => p.Name).First(),
                    SocialSecurityNumber = s.Patient.SocialSecurityNum,
                    FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                    CreationDate = s.CreateDate
                }
                ).ToList().OrderByDescending(o => o.CreationDate).ToList();  
            }
            

            return View(MyInRequests);
        }

        public IActionResult OutRequest()
        {
            var db = new DatabaseContext();

            List<CheckupTicketToOtherModel> MyOutRequests = new List<CheckupTicketToOtherModel>();

            string user = User.FindFirstValue(ClaimTypes.Name);
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();


            //if (id_list.Any())
            //{
            MyOutRequests = db.CheckupTicketT//.Where(o => o.CreatedBy.UserId == id_list.First())
            .Select(s => new CheckupTicketToOtherModel
            {
                PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.PatientID).Select(p => p.Surname).First(),
                PatientName = db.UserT.Where(o => o.UserId == s.Patient.PatientID).Select(p => p.Name).First(),
                SocialSecurityNumber = s.Patient.SocialSecurityNum,
                ToDoctor = db.UserT.Where(o => o.UserId == s.ToDoctor.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                CreationDate = s.CreateDate,
                State = s.State == "dokončeno"
            }
            ).ToList();
            MyOutRequests.Sort((x, y) => (y.State == x.State) ? DateTime.Compare(y.CreationDate, x.CreationDate) : (x.State ? 1 : -1));
            //}

            return View(MyOutRequests);
        }
    }
}
