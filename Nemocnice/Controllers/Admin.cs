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
using X.PagedList.Mvc.Core;
using X.PagedList;

namespace Nemocnice.Controllers
{

    [Authorize(Roles = "Admin")]

    public class Admin : Controller
    {
        public IActionResult Index()
        {
            return View();
        }

        /*
         * Informace o právě přihlášeném uživateli.
         */
        private readonly UserManager<NemocniceUser> _userManager;

        /*
         * Proměnná umožňující napříč kontrollerem přistupovat k databázi
         */
        private DatabaseContext db { get; set; } = new DatabaseContext();

        public Admin(UserManager<NemocniceUser> userManager)
        {
            _userManager = userManager;
        }


        /*
         * Akce karotéky. Vypíše všechny pacienty uložené v databázi.
         * sortOrder - typ řazení (podle jnéma, příjmení, rodného čísla)
         * searchString - hledaný řetězec (v případě vyhledávání)
         */
        public IActionResult Card(string sortOrder, string searchString, string ID_delete, CardModel model, int ? p)
        {
            // Uložení přávě vyhledávaného řetězce.
            // Při řazení výsledků budeme už vědět, o jaké výsledky se jedná.
            ViewData["Search"] = searchString;
            ViewData["CurrentSort"] = sortOrder;
            ViewData["CurrentPage"] = p;



            //pokud je ID_delete různé od 0, víme, jakého pacienta chceme odstranit
            if (ID_delete != null)
            {
                db.Remove(db.PatientT.Single(a => a.SocialSecurityNum == ID_delete));
                db.SaveChanges();
            }

            // Model - Seznam všech pacientů v databázi
            List<CardModel> Patients;

            // Získání údajů ke každému pacientovi (Příjmení, Jméno, Titul, R.Č., pojišťovny).
            // Informace jsou získávány ze spojení dvou tabulek: PatientT (R.Č., pojišťovna) a UserT (Příjmení, Jméno, Titul).
            if (String.IsNullOrEmpty(searchString))
            {
                // Není použito vyhledávání (chceme všechny pacienty)
                Patients = db.PatientT.Join(db.UserT,
                                patient => patient.UserId,
                                user => user.UserId,
                                (patient, user) => new CardModel
                                {
                                    PatientFullName = new NameModel
                                    {
                                        Surname = user.Surname,
                                        Name = user.Name,
                                        Title = user.Title,
                                    },
                                    UserId = user.UserId,
                                    SocialSecurityNum = patient.SocialSecurityNum,
                                    Insurance = patient.InsuranceCompany
                                }
                                ).ToList();
            }
            else
            {
                // Je potřeba vyhledat konkrétní pacienty odpovídající hledanému výrazu.
                // Hledání probíhá skrz položky (Jméno, Příjmení, R.Č.).
                // Rodné číslo je převáděno na string. Hledání probíhá na základě metody StartsWith.
                Patients = db.PatientT.Join(db.UserT,
                                patient => patient.UserId,
                                user => user.UserId,
                                (patient, user) => new CardModel
                                {
                                    PatientFullName = new NameModel
                                    {
                                        Surname = user.Surname,
                                        Name = user.Name,
                                        Title = user.Title,
                                    },
                                    SocialSecurityNum = patient.SocialSecurityNum,
                                    Insurance = patient.InsuranceCompany
                                }
                                ).Where(s => s.SocialSecurityNum.Contains(searchString) ||
                                             s.PatientFullName.Name.Contains(searchString) ||
                                             s.PatientFullName.Surname.Contains(searchString)).ToList();
            }
            model.patients = Patients;

            // Řazení dle jednotlivých kritérií nastaveních v sortOrder.
            switch (sortOrder)
            {
                case "byName":
                    model.patients = model.patients.OrderBy(o => o.PatientFullName.Name).ToList();
                    break;
                case "byNumber":
                    model.patients = model.patients.OrderBy(o => o.SocialSecurityNum).ToList();
                    break;
                default:
                    model.patients = model.patients.OrderBy(o => o.PatientFullName.Surname).ToList();
                    break;
            }

            //stránkování vybraných dat
            model.PageNum = (p ?? 1);
            int pageSize = 5;
            IPagedList<CardModel> lide = model.patients.ToPagedList(model.PageNum, pageSize);
            model.patientsPage = lide;

            return View(model);
        }


        /*
         * Akce vytvoří nového pacienta v databázi.
         * Zahrnuje vytvoření Adresy, Uživatele, Pacienta, Uživatele pro Identity FW a přidělení práv.
         */
        [HttpPost]
        public async Task<IActionResult> CreatePatientAsync()
        {
            // Hodnoty pro nového pacienta získané přes POST
            string name = Request.Form["NewName"];
            string surname = Request.Form["NewSurname"];
            string title = Request.Form["NewTitle"];
            string socialNum = Request.Form["NewNum"];
            int insurance = int.Parse(Request.Form["NewInsurance"]);
            string tel = Request.Form["NewTel"];
            string email = Request.Form["NewEmail"];
            string street = Request.Form["NewStreet"];
            // houseNum může být nezadáno, nejde převést prázdnou hodnotu na int, proto kontroluji.
            int houseNum = int.Parse(String.IsNullOrEmpty(Request.Form["NewHouseNum"].ToString()) ? "0" : Request.Form["NewHouseNum"].ToString());
            string city = Request.Form["NewCity"];
            int zip = int.Parse(Request.Form["NewZip"].ToString().Replace(" ", ""));    // Může být ve tvaru "739 11"

            // Deklarace proměnných pro budoucí vložení do databáze
            Address address;
            User user;

            // Kontrola, zda pacient již existuje, pokud ano, nic neděláme.
            // TODO - vypsat upozornění
            if (db.PatientT.Where(o => o.SocialSecurityNum == socialNum).ToList().Count != 0)
            {
                return RedirectToAction("Card");
            }

            // Vytváření jednotlivých tabulek pro nového pacienta
            // Po každé vložené tabulce pro jistotu provedu SaveChanges,
            // protože se hned v zápětí na tuto tabulku dotazuji. (Změny se musí uložit.)
            // Vytvoření adresy
            if (houseNum == 0)
            {
                address = new Address
                {
                    StreetName = street,
                    City = city,
                    ZIP = zip
                };
            }
            else
            {
                address = new Address
                {
                    HouseNumber = houseNum,
                    StreetName = street,
                    City = city,
                    ZIP = zip
                };
            }
            db.AddressT.Add(address);
            db.SaveChanges();

            // Vytvoření uživatele (Jméno, Příjmení, Titul, ...)
            // Uživatelským jménem pacientů je jejich rodné číslo.
            user = new User
            {
                Login = socialNum,
                Name = name,
                Surname = surname,
                Title = title,
                Phone = tel,
                Email = email,
                WorkAddress = null
            };
            db.UserT.Add(user);
            db.SaveChanges();

            // Vytvoření pacienta.
            // Napojení Address a User
            db.PatientT.Add(new Data.Patient
            {
                UserId = db.UserT.Where(o => o.Login == user.Login).Select(s => s.UserId).ToList().First(),
                SocialSecurityNum = socialNum,
                InsuranceCompany = insurance,
                HomeAddress = address,
                HealthCondition = new HealthCondition { SocialSecurityNum = socialNum }
            });
            db.SaveChanges();

            // Vytvoření nového uživatele pro Identity Framework.
            // Základní heslo je 1234567890, uživatelské jméno je shodné s rodným číslem.
            // Udělení oprávnění Patient.
            var userIdentity = new NemocniceUser { UserName = user.Login };
            var result = await _userManager.CreateAsync(userIdentity, "1234567890");
            await _userManager.AddToRoleAsync(userIdentity, "Patient");

            // Návrat zpět do kartotéky.
            return RedirectToAction("Card", new { SortOrder = Request.Form["SortOrder"], p = Request.Form["p"], Search = Request.Form["Search"] });
        }


        [HttpPost]
        public IActionResult EditDb_Patient()
        {

            string edit_ID = Request.Form["edit_ID"];
            string edit_name = Request.Form["edit_name"];
            string edit_surname = Request.Form["edit_surname"];
            string edit_rc = Request.Form["edit_rc"];
            int edit_insurance = int.Parse(Request.Form["edit_insurance"]);
            string edit_tel = Request.Form["edit_tel"];
            string edit_mail = Request.Form["edit_mail"];
            string edit_title = Request.Form["edit_title"];

            var pom = db.UserT.First(a => a.Login == edit_ID);
            pom.Name = edit_name;
            pom.Surname = edit_surname;
            pom.Title = edit_title;
            pom.Phone = edit_tel;
            pom.Email = edit_mail;

            var work = db.UserT.Where(a => a.Login == edit_ID).Select(s => s.WorkAddress).FirstOrDefault();

            if (work != null)
            {
                string edit_street = Request.Form["edit_street"];
                int edit_cp = int.Parse(Request.Form["edit_cp"]);
                string edit_town = Request.Form["edit_town"];
                int edit_psc = int.Parse(Request.Form["edit_psc"]);

                int pom2 = Convert.ToInt32(work.AddressId);
                var pom3 = db.AddressT.First(a => a.AddressId == pom2);

                pom3.StreetName = edit_street;
                pom3.HouseNumber = edit_cp;
                pom3.City = edit_town;
                pom3.ZIP = edit_psc;
            }

       
            db.SaveChanges();

            return RedirectToAction("Card", new { SortOrder = Request.Form["SortOrder"], p = Request.Form["p"], Search = Request.Form["Search"] });
        }


        public IActionResult DoctorEdit(string sortOrder, string searchString, string ID_delete, DoctorEditModel model, int? p)
        {
            // Uložení přávě vyhledávaného řetězce.
            // Při řazení výsledků budeme už vědět, o jaké výsledky se jedná.
            ViewData["Search"] = searchString;
            ViewData["CurrentSort"] = sortOrder;
            ViewData["CurrentPage"] = p;

            //pokud je ID_delete různé od 0, víme, jakého doktora chceme odstranit
            if (ID_delete != null)
            {
                try
                {
                    var userId = db.DoctorT.Where(x => x.ICZ == int.Parse(ID_delete)).Select(x => x.UserId).FirstOrDefault();
                    db.Remove(db.DoctorT.Single(a => a.ICZ == int.Parse(ID_delete)));
                    db.Remove(db.PatientT.Single(a => a.UserId == userId));
                    db.SaveChanges();
                } catch (Exception)
                {

                }
            }

            // Model - Seznam všech pacientů v databázi
            List<DoctorJoined> Doctors;

            // Získání údajů ke každému pacientovi (Příjmení, Jméno, Titul, R.Č., pojišťovny).
            // Informace jsou získávány ze spojení dvou tabulek: PatientT (R.Č., pojišťovna) a UserT (Příjmení, Jméno, Titul).
            if (String.IsNullOrEmpty(searchString))
            {
                // Není použito vyhledávání (chceme všechny pacienty)
                Doctors = db.DoctorT.Join(db.UserT.Include(x => x.WorkAddress), doctor => doctor.UserId, user => user.UserId,
                (doctor, user) => new DoctorJoined
                {
                    Surname = user.Surname,
                    Name = user.Name,
                    Title = user.Title,
                    UserId = user.UserId,
                    ICZ = doctor.ICZ,
                    Email = user.Email,
                    Phone = doctor.WorkPhone,
                    StreetName = user.WorkAddress.StreetName,
                    City = user.WorkAddress.City,
                    ZIP = user.WorkAddress.ZIP,
                    HouseNumber = user.WorkAddress.HouseNumber
                }).ToList();
            }
            else
            {
                // Je potřeba vyhledat konkrétní pacienty odpovídající hledanému výrazu.
                // Hledání probíhá skrz položky (Jméno, Příjmení, R.Č.).
                // Rodné číslo je převáděno na string. Hledání probíhá na základě metody StartsWith.
                Doctors = db.DoctorT.Join(db.UserT.Include(x => x.WorkAddress), doctor => doctor.UserId, user => user.UserId,
                            (doctor, user) => new DoctorJoined
                            {
                                Surname = user.Surname,
                                Name = user.Name,
                                Title = user.Title,
                                UserId = user.UserId,
                                ICZ = doctor.ICZ,
                                Email = user.Email,
                                Phone = doctor.WorkPhone,
                                StreetName = user.WorkAddress.StreetName,
                                City = user.WorkAddress.City,
                                ZIP = user.WorkAddress.ZIP,
                                HouseNumber = user.WorkAddress.HouseNumber
                            }).Where(s => s.Title.Contains(searchString) ||
                                             s.Surname.Contains(searchString) ||
                                             s.Name.Contains(searchString)).ToList();
            }
            model.doctors = Doctors;

            // Řazení dle jednotlivých kritérií nastaveních v sortOrder.
            switch (sortOrder)
            {
                case "byName":
                    model.doctors = model.doctors.OrderBy(o => o.Name).ToList();
                    break;
                case "byICZ":
                    model.doctors = model.doctors.OrderBy(o => o.ICZ).ToList();
                    break;
                default:
                    model.doctors = model.doctors.OrderBy(o => o.Surname).ToList();
                    break;
            }

            //stránkování vybraných dat
            model.PageNum = (p ?? 1);
            int pageSize = 15;
            IPagedList<DoctorJoined> lide = model.doctors.ToPagedList(model.PageNum, pageSize);
            model.DoctorJoined = lide;

            return View(model);
        }
    }
}



