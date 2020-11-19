#define TEST

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
using Nemocnice.DatabaseDataCreator;
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
                        patient => patient.UserId,
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
                        patient => patient.UserId,
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
                            patient => patient.UserId,
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
            


            return View(new CardModel { OldPatients = Patients });
        }

        public IActionResult Activity(string userName)
        {
            var db = new DatabaseContext();

            string user = User.FindFirstValue(ClaimTypes.Name);
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();

            List<ActivityModel> Activities = new List<ActivityModel>();
#if (!TEST)
            if (id_list.Any())
            {
                Activities = db.MedicallBillT.Where(o => o.Doctor.UserId == id_list.First())
#else
                Activities = db.MedicallBillT
#endif
                                                    .Select(s => new ActivityModel
                                                        {
                                                            ActivityName = s.MedicallActivityPrice.Name,
                                                            Value = s.MedicallActivityPrice.Amount,
                                                            PatientNum = s.SocialSecurityNum,
                                                            CreateDate = s.CreateDate,
                                                            State = s.State,
                                                        }).ToList();
#if (!TEST)
        }
#endif
                             
            return View(Activities);
        }

        public IActionResult Requests()
        {
            return View();
        }

        public IActionResult InRequest(string sortOrder, string searchString)
        {
            ViewData["Search"] = searchString;
            var db = new DatabaseContext();

            List<CheckupTicketToMe> MyInRequests = new List<CheckupTicketToMe>();

            string user = User.FindFirstValue(ClaimTypes.Name);
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();



#if (!TEST)
            if(id_list.Any())
            {
                MyInRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno").Where(o => o.ToDoctor.UserId == id_list.First())
#else

            if (String.IsNullOrEmpty(searchString))
            {
                MyInRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno")
#endif
                .Select(s => new CheckupTicketToMe
                {
                    PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                    PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                    SocialSecurityNumber = s.Patient.SocialSecurityNum,
                    FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                    CreationDate = s.CreateDate.ToString("dd.MM.yyyy")
                }
                ).ToList();
            }
            else
            {
                Int64 parseRes;
                if (Int64.TryParse(searchString, out parseRes))
                {
                    MyInRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno").Select(s => new CheckupTicketToMe
                    {
                    PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                    PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                    SocialSecurityNumber = s.Patient.SocialSecurityNum,
                    FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                    CreationDate = s.CreateDate.ToString("dd.MM.yyyy")
                    }
                ).Where(s => s.SocialSecurityNumber == parseRes).ToList();
                }
                else
                {
                    MyInRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno").Select(s => new CheckupTicketToMe
                    {
                        PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                        PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                        SocialSecurityNumber = s.Patient.SocialSecurityNum,
                        FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                        CreationDate = s.CreateDate.ToString("dd.MM.yyyy")
                    }
                    ).Where(s => s.PatientName.Contains(searchString) || s.PatientSurname.Contains(searchString) || s.FromDoctor.Contains(searchString)).ToList();
                }
            }

#if (!TEST)
            }
#endif
                switch (sortOrder)
            {
                case "bySurname":
                    MyInRequests = MyInRequests.OrderBy(s => s.PatientSurname).ToList();
                    break;
                case "byName":
                    MyInRequests = MyInRequests.OrderBy(s => s.PatientName).ToList();
                    break;
                case "byNumber":
                    MyInRequests = MyInRequests.OrderBy(s => s.SocialSecurityNumber).ToList();
                    break;
                case "byDoctor":
                    MyInRequests = MyInRequests.OrderBy(s => s.FromDoctor).ToList();
                    break;
                default:
                    MyInRequests = MyInRequests.OrderByDescending(o => DateTime.Parse(o.CreationDate)).ToList();
                    break;
            }

            return View(MyInRequests);
        }

        public IActionResult OutRequest(string sortOrder, string searchString)
        {
            var db = new DatabaseContext();
            ViewData["Search"] = searchString;
            List<CheckupTicketToOtherModel> MyOutRequests = new List<CheckupTicketToOtherModel>();

            string user = User.FindFirstValue(ClaimTypes.Name);
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();

            if (String.IsNullOrEmpty(searchString))
            {
                MyOutRequests = db.CheckupTicketT
                .Select(s => new CheckupTicketToOtherModel
                {
                    PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                    PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                    SocialSecurityNumber = s.Patient.SocialSecurityNum,
                    ToDoctor = db.UserT.Where(o => o.UserId == s.ToDoctor.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                    CreationDate = s.CreateDate.ToString("dd.MM.yyyy"),
                    State = s.State == "dokončeno"
                }
                               ).ToList().OrderByDescending(p => DateTime.Parse(p.CreationDate)).ToList();
                MyOutRequests.Sort((x, y) => (y.State == x.State) ? DateTime.Compare(DateTime.Parse(y.CreationDate), DateTime.Parse(x.CreationDate)) : (x.State ? 1 : -1));
            }
            else
            {
                Int64 parseRes;
                if (Int64.TryParse(searchString, out parseRes))
                {
                    MyOutRequests = db.CheckupTicketT
                    .Select(s => new CheckupTicketToOtherModel
                    {
                        PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                        PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                        SocialSecurityNumber = s.Patient.SocialSecurityNum,
                        ToDoctor = db.UserT.Where(o => o.UserId == s.ToDoctor.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                        CreationDate = s.CreateDate.ToString("dd.MM.yyyy"),
                        State = s.State == "dokončeno"
                    }
               ).Where(s => s.SocialSecurityNumber == parseRes).ToList();
                    MyOutRequests.Sort((x, y) => (y.State == x.State) ? DateTime.Compare(DateTime.Parse(y.CreationDate), DateTime.Parse(x.CreationDate)) : (x.State ? 1 : -1));
                }
                else
                {
                    MyOutRequests = db.CheckupTicketT
                    .Select(s => new CheckupTicketToOtherModel
                    {
                        PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                        PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                        SocialSecurityNumber = s.Patient.SocialSecurityNum,
                        ToDoctor = db.UserT.Where(o => o.UserId == s.ToDoctor.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                        CreationDate = s.CreateDate.ToString("dd.MM.yyyy"),
                        State = s.State == "dokončeno"
                    }
               ).Where(s => s.PatientName.Contains(searchString) || s.PatientSurname.Contains(searchString) || s.ToDoctor.Contains(searchString)).ToList();
                    MyOutRequests.Sort((x, y) => (y.State == x.State) ? DateTime.Compare(DateTime.Parse(y.CreationDate), DateTime.Parse(x.CreationDate)) : (x.State ? 1 : -1));
                }
            }

                switch (sortOrder)
            {
                case "bySurname":
                    MyOutRequests = MyOutRequests.OrderBy(s => s.PatientSurname).ToList();
                    break;
                case "byName":
                    MyOutRequests = MyOutRequests.OrderBy(s => s.PatientName).ToList();
                    break;
                case "byNumber":
                    MyOutRequests = MyOutRequests.OrderBy(s => s.SocialSecurityNumber).ToList();
                    break;
                case "byDoctor":
                    MyOutRequests = MyOutRequests.OrderBy(s => s.ToDoctor).ToList();
                    break;
                case "byState":
                    MyOutRequests = MyOutRequests.OrderBy(s => s.State).ToList();
                    break;
                default:
                    MyOutRequests = MyOutRequests.OrderByDescending(o => DateTime.Parse(o.CreationDate)).ToList();
                    break;
            }


            return View(MyOutRequests);
        }

        public IActionResult InFinishRequest(string sortOrder, string searchString)
        {
            ViewData["Search"] = searchString;
            var db = new DatabaseContext();

            List<CheckupTicketToMeFinish> MyFinishRequests = new List<CheckupTicketToMeFinish>();

            string user = User.FindFirstValue(ClaimTypes.Name);
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();

#if (!TEST)
            if(id_list.Any())
            {
                MyFinishRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno").Where(o => o.ToDoctor.UserId == id_list.First())
#else

#if (!TEST)
            }
#endif
#endif

            if (String.IsNullOrEmpty(searchString))
            {
                MyFinishRequests = db.CheckupTicketT.Where(o => o.State == "dokončeno")
                .Select(s => new CheckupTicketToMeFinish
                {
                    PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                    PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                    SocialSecurityNumber = s.Patient.SocialSecurityNum,
                    FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                    CreationDate = s.CreateDate.ToString("dd.MM.yyyy"),
                    FinishDate = s.FinishDate.ToString("dd.MM.yyyy")
                }
                           ).ToList().OrderByDescending(o => DateTime.Parse(o.CreationDate)).ToList();
            }
            else
            {
                {
                    Int64 parseRes;
                    if (Int64.TryParse(searchString, out parseRes))
                    {
                        MyFinishRequests = db.CheckupTicketT.Where(o => o.State == "dokončeno")
                        .Select(s => new CheckupTicketToMeFinish
                        {
                            PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                            PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                            SocialSecurityNumber = s.Patient.SocialSecurityNum,
                            FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                            CreationDate = s.CreateDate.ToString("dd.MM.yyyy"),
                            FinishDate = s.FinishDate.ToString("dd.MM.yyyy")
                        }
                        ).Where(s => s.SocialSecurityNumber == parseRes).ToList();
                    }
                    else
                    {
                        MyFinishRequests = db.CheckupTicketT.Where(o => o.State == "dokončeno")
                        .Select(s => new CheckupTicketToMeFinish
                        {
                            PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                            PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                            SocialSecurityNumber = s.Patient.SocialSecurityNum,
                            FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.Title + ((String.IsNullOrEmpty(p.Title)) ? "" : " ") + p.Name + " " + p.Surname).First(),
                            CreationDate = s.CreateDate.ToString("dd.MM.yyyy"),
                            FinishDate = s.FinishDate.ToString("dd.MM.yyyy")
                        }
                        ).Where(s => s.PatientName.Contains(searchString) || s.PatientSurname.Contains(searchString) || s.FromDoctor.Contains(searchString)).ToList();
                    }
                }
            }

                switch (sortOrder)
            {
                case "bySurname":
                    MyFinishRequests = MyFinishRequests.OrderBy(s => s.PatientSurname).ToList();
                    break;
                case "byName":
                    MyFinishRequests = MyFinishRequests.OrderBy(s => s.PatientName).ToList();
                    break;
                case "byNumber":
                    MyFinishRequests = MyFinishRequests.OrderBy(s => s.SocialSecurityNumber).ToList();
                    break;
                case "byDoctor":
                    MyFinishRequests = MyFinishRequests.OrderBy(s => s.FromDoctor).ToList();
                    break;
                case "byOsetreniDate":
                    MyFinishRequests = MyFinishRequests.OrderByDescending(s => s.FinishDate).ToList();
                    break;
                default:
                    MyFinishRequests = MyFinishRequests.OrderByDescending(o => DateTime.Parse(o.CreationDate)).ToList();
                    break;
            }
            return View(MyFinishRequests);
        }


        public async Task<IActionResult> CreatePatientAsync(CardModel cardModel)
        {
            NewPatientModel newPatient = cardModel.NewPatient;
            var db = new DatabaseContext();

            if(ModelState.IsValid && db.PatientT.Where(o => o.SocialSecurityNum == newPatient.SocialSecurityNumber).ToList().Count == 0)
            {
                // Make address
                Address address = new Address
                {
                    HouseNumber = newPatient.HouseNumber,
                    StreetName = newPatient.Street,
                    City = newPatient.City,
                    ZIP = newPatient.Zip
                };
                db.AddressT.Add(address);
                db.SaveChanges();

                // Make user
                User user = new User
                {
                    Login = newPatient.SocialSecurityNumber.ToString(),
                    Name = newPatient.Name,
                    Surname = newPatient.Surname,
                    Title = newPatient.Title,
                    Phone = newPatient.Tel,
                    Email = newPatient.Email,
                    WorkAddress = null
                };
                db.UserT.Add(user);
                db.SaveChanges();

                // Make patient
                db.PatientT.Add(new Data.Patient
                {
                    UserId = db.UserT.Where(o => o.Login == user.Login).Select(s => s.UserId).ToList().First(),
                    SocialSecurityNum = newPatient.SocialSecurityNumber,
                    InsuranceCompany = newPatient.InsuranceCompany,
                    HomeAddress = address,
                    HealthCondition = null
                });
                db.SaveChanges();

                var userIdentity = new NemocniceUser { UserName = user.Login };
                var result = await _userManager.CreateAsync(userIdentity, "1234567890");
                await _userManager.AddToRoleAsync(userIdentity, "Patient");

                // Make new patient in identity framework
                return RedirectToAction("Card");
            }
            else
            {
                return RedirectToAction("Card");
            }
        }
    }
}
