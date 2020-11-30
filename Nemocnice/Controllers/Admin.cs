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
using Microsoft.EntityFrameworkCore.Internal;
using System.Text.RegularExpressions;

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
        public async Task<IActionResult> Card(string sortOrder, string searchString, string ID_delete, Admin_patient model, int ? p)
        {
            // Uložení přávě vyhledávaného řetězce.
            // Při řazení výsledků budeme už vědět, o jaké výsledky se jedná.
            ViewData["Search"] = searchString;
            ViewData["CurrentSort"] = sortOrder;
            ViewData["CurrentPage"] = p;



            //pokud je ID_delete různé od 0, víme, jakého pacienta chceme odstranit
            if (ID_delete != null)
            {
                var PatientId = db.PatientT.Where(s => s.SocialSecurityNum == ID_delete).Select(s => s.PatientID).FirstOrDefault();

                List<CheckupTicket> tmp = new List<CheckupTicket>();
                tmp = db.CheckupTicketT.Where(a => a.Patient.PatientID == PatientId).Include(i => i.Patient).ToList();
                foreach (var pom in tmp) {
                    db.Remove(db.CheckupTicketT.FirstOrDefault(a => a.Patient.PatientID == pom.Patient.PatientID));
                }

                List<MedicallReport> tmp1 = new List<MedicallReport>();
                tmp1 = db.MedicallReportT.Where(a => a.Patient.PatientID == PatientId).Include(i => i.Patient).ToList();
                foreach (var pom in tmp1)
                {
                    db.Remove(db.MedicallReportT.FirstOrDefault(a => a.Patient.PatientID == pom.Patient.PatientID));
                }

                List<PatientTreatmentLog> tmp2 = new List<PatientTreatmentLog>();
                tmp2 = db.PatientTreatmentLogT.Where(a => a.Patient.PatientID == PatientId).Include(i => i.Patient).ToList();
                foreach (var pom in tmp2)
                {
                    db.Remove(db.PatientTreatmentLogT.FirstOrDefault(a => a.Patient.PatientID == pom.Patient.PatientID));
                }
               
                var PictureId = db.PictureT.Where(s => s.SocialSecurityNum == ID_delete).Select(s => s.PictureId).FirstOrDefault();
                List<PictureOnReport> tmp3 = new List<PictureOnReport>();
                tmp3 = db.PictureOnReportT.Where(a => a.Picture.PictureId == PictureId).Include(i => i.Picture).ToList();
                foreach (var pom in tmp3)
                {
                    db.Remove(db.PictureOnReportT.FirstOrDefault(a => a.Picture.PictureId == pom.Picture.PictureId));
                }

                List<PictureOnTicket> tmp4 = new List<PictureOnTicket>();
                tmp4 = db.PictureOnTicketsT.Where(a => a.Picture.PictureId == PictureId).Include(i => i.Picture).ToList();
                foreach (var pom in tmp4)
                {
                    db.Remove(db.PictureOnTicketsT.FirstOrDefault(a => a.Picture.PictureId == pom.Picture.PictureId));
                }

                List<Picture> tmp5 = new List<Picture>();
                tmp5 = db.PictureT.Where(a => a.PictureId == PictureId).ToList();
                foreach (var pom in tmp5)
                {
                    db.Remove(db.PictureT.FirstOrDefault(a => a.SocialSecurityNum == ID_delete));
                }

                var health = db.HealthConditionT.Where(s => s.SocialSecurityNum == ID_delete).Select(s => s.HealthConditionId).FirstOrDefault();
                if (health != 0)
                {
                    db.Remove(db.HealthConditionT.FirstOrDefault(a => a.HealthConditionId == health));
                }

                var ID = db.PatientT.Where(s => s.SocialSecurityNum == ID_delete).Select(s => s.UserId).FirstOrDefault();
                db.Remove(db.PatientT.FirstOrDefault(a => a.SocialSecurityNum == ID_delete));

                db.Remove(db.UserT.FirstOrDefault(a => a.UserId == ID));

                var Patient = db.PatientT.Where(x => x.SocialSecurityNum == ID_delete).Select(s => s.UserId).First();
                var userLogin = db.UserT.Where(x => x.UserId == Patient).Select(x => x.Login).FirstOrDefault();
                var user = await _userManager.FindByNameAsync(userLogin);
                if (user != null)
                {
                    await _userManager.DeleteAsync(user);
                }
                db.SaveChanges();


            }

            // Model - Seznam všech pacientů v databázi
            List<Admin_patient> Patients;

            // Získání údajů ke každému pacientovi (Příjmení, Jméno, Titul, R.Č., pojišťovny).
            // Informace jsou získávány ze spojení dvou tabulek: PatientT (R.Č., pojišťovna) a UserT (Příjmení, Jméno, Titul).
            if (String.IsNullOrEmpty(searchString))
            {
                // Není použito vyhledávání (chceme všechny pacienty)
                Patients = db.PatientT.Join(db.UserT,
                                patient => patient.UserId,
                                user => user.UserId,
                                (patient, user) => new Admin_patient
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
                                (patient, user) => new Admin_patient
                                {
                                    PatientFullName = new NameModel
                                    {
                                        Surname = user.Surname,
                                        Name = user.Name,
                                        Title = user.Title,
                                    },
                                    UserId = patient.UserId,
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
            IPagedList<Admin_patient> lide = model.patients.ToPagedList(model.PageNum, pageSize);
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
        public async Task<IActionResult> EditDb_PatientAsync()
        {

            string edit_oldRC = Request.Form["edit_ID"];
            string edit_name = Request.Form["edit_name"];
            string edit_surname = Request.Form["edit_surname"];
            string edit_rc = Request.Form["edit_rc"];
            int edit_insurance = int.Parse(Request.Form["edit_insurance"]);
            string edit_tel = Request.Form["edit_tel"];
            string edit_mail = Request.Form["edit_mail"];
            string edit_title = Request.Form["edit_title"];
            string edit_password = Request.Form["edit_password"];
            string edit_confirmPassword = Request.Form["edit_confirmPassword"];

            var PatientId = db.PatientT.Where(x => x.SocialSecurityNum == edit_oldRC).Select(x => x.UserId).First();
            
            var Patient = db.UserT.Include(x => x.WorkAddress).Where(x => x.UserId == PatientId).First();
            
            if (Patient.WorkAddress != null)
            {

                string edit_street = Request.Form["edit_street"];
                
                string edit_town = Request.Form["edit_town"];
                int edit_psc = int.Parse(Request.Form["edit_psc"]);
                var changePicSSN = db.PictureT.Where(x => x.SocialSecurityNum == edit_oldRC).ToList();
                foreach (var fotka in changePicSSN)
                {
                    fotka.SocialSecurityNum = edit_rc;
                }
                db.SaveChanges();
                int pom2 = Convert.ToInt32(Patient.WorkAddress.AddressId);
                var pom3 = db.AddressT.First(a => a.AddressId == pom2);

                if (Request.Form["edit_cp"] == "")
                {
                    int edit_cp = int.Parse(Request.Form["edit_cp"]);
                    pom3.HouseNumber = edit_cp;
                }
                pom3.StreetName = edit_street;
                
                pom3.City = edit_town;
                pom3.ZIP = edit_psc;
            }

            var pom = db.UserT.First(a => a.Login == Patient.Login);
            pom.Name = edit_name;
            pom.Surname = edit_surname;
            pom.Title = edit_title;
            pom.Phone = edit_tel;
            pom.Email = edit_mail;
            pom.Login = edit_rc;

            var pat = db.PatientT.First(a => a.SocialSecurityNum == edit_oldRC);
            pat.SocialSecurityNum = edit_rc;
            pat.InsuranceCompany = edit_insurance;


            var userChangeLogin = await _userManager.FindByNameAsync(edit_oldRC);
            userChangeLogin.UserName = edit_rc;
            //user.UserName = edit_rc;
            //user.NormalizedUserName = edit_rc.ToUpper();
            await _userManager.UpdateAsync(userChangeLogin);

            /****************************/


            var newPasswordHashed = _userManager.PasswordHasher.HashPassword(userChangeLogin,edit_password);
            var token = _userManager.GeneratePasswordResetTokenAsync(userChangeLogin).Result;
            await _userManager.ResetPasswordAsync(userChangeLogin, token, edit_password);
            await _userManager.UpdateAsync(userChangeLogin);
            
            /****************************/

            await db.SaveChangesAsync();

            return RedirectToAction("Card", new { SortOrder = Request.Form["SortOrder"], p = Request.Form["p"], Search = Request.Form["Search"] });
        }

        

        [HttpPost]
        public IActionResult EditDb_Insurance()
        {

            int edit_ID = int.Parse(Request.Form["edit_ID"]);
            string edit_name = Request.Form["edit_name"];
            string edit_surname = Request.Form["edit_surname"];
            string edit_rc = Request.Form["edit_rc"];
            string edit_tel = Request.Form["edit_tel"];
            string edit_mail = Request.Form["edit_mail"];
            string edit_title = Request.Form["edit_title"];
            string edit_work = Request.Form["edit_work"];
            string edit_position = Request.Form["edit_position"];
            string edit_login = Request.Form["edit_login"];
            string old = Request.Form["old"];
            string edit_password = Request.Form["edit_password"];
            string edit_confirmPassword = Request.Form["edit_confirmPassword"];

            var ID = db.InsureEmpT.Where(a => a.PersonalId == edit_ID).Select(s => s.UserId).FirstOrDefault();
            var pom = db.UserT.First(a => a.UserId == ID);

            pom.Name = edit_name;
            pom.Surname = edit_surname;
            pom.Title = edit_title;
            pom.Phone = edit_tel;
            pom.Email = edit_mail;
            pom.Login = edit_login;

            var ins = db.InsureEmpT.First(s => s.UserId == ID);
            ins.Possition = edit_position;
            ins.WorkPhone = edit_work;

            var user = db.Users.First(s => s.UserName == old);
            user.UserName = edit_login;
            user.NormalizedUserName = edit_login;



            var work = db.UserT.Where(a => a.UserId == ID).Select(s => s.WorkAddress).FirstOrDefault();

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

            return RedirectToAction("CardInsurance", new { SortOrder = Request.Form["SortOrder"], p = Request.Form["p"], Search = Request.Form["Search"] });
        }
    
        public async Task<IActionResult> CardInsurance(string sortOrder, string searchString, int ID_delete, Admin_insurance model, int? p)
        {
            // Uložení přávě vyhledávaného řetězce.
            // Při řazení výsledků budeme už vědět, o jaké výsledky se jedná.
            ViewData["Search"] = searchString;
            ViewData["CurrentSort"] = sortOrder;
            ViewData["CurrentPage"] = p;



            //pokud je ID_delete různé od 0, víme, jakého pacienta chceme odstranit
            if (ID_delete != 0)
            {
                var ID = db.InsureEmpT.Where(s => s.PersonalId == ID_delete).Select(s => s.UserId).FirstOrDefault();
                db.Remove(db.InsureEmpT.Single(a => a.PersonalId == ID_delete));
                db.Remove(db.UserT.Single(a => a.UserId == ID));

                var userLogin = db.UserT.Where(x => x.UserId == ID).Select(x => x.Login).FirstOrDefault();
                var user = await _userManager.FindByNameAsync(userLogin);
                if (user != null)
                {
                    await _userManager.DeleteAsync(user);
                }
                db.SaveChanges();
            }

            // Model - Seznam všech pacientů v databázi
            List<Admin_insurance> Insurance;

            // Získání údajů ke každému pacientovi (Příjmení, Jméno, Titul, R.Č., pojišťovny).
            // Informace jsou získávány ze spojení dvou tabulek: PatientT (R.Č., pojišťovna) a UserT (Příjmení, Jméno, Titul).
            if (String.IsNullOrEmpty(searchString))
            {
                // Není použito vyhledávání (chceme všechny pacienty)
                Insurance = db.InsureEmpT.Join(db.UserT,
                                insurance => insurance.UserId,
                                user => user.UserId,
                                (insurance, user) => new Admin_insurance
                                {
                                    InsuranceFullName = new NameModel
                                    {
                                        Surname = user.Surname,
                                        Name = user.Name,
                                        Title = user.Title,
                                    },
                                    UserId = user.UserId,
                                    Position = insurance.Possition,
                                    PersonalID = insurance.PersonalId,
                                    WorkPhone = insurance.WorkPhone,
                                    Login = user.Login
                                }
                                ).ToList();
            }
            else
            {
                // Je potřeba vyhledat konkrétní pacienty odpovídající hledanému výrazu.
                // Hledání probíhá skrz položky (Jméno, Příjmení, R.Č.).
                // Rodné číslo je převáděno na string. Hledání probíhá na základě metody StartsWith.
                Insurance = db.InsureEmpT.Join(db.UserT,
                                insurance => insurance.UserId,
                                user => user.UserId,
                                (insurance, user) => new Admin_insurance
                                {
                                    InsuranceFullName = new NameModel
                                    {
                                        Surname = user.Surname,
                                        Name = user.Name,
                                        Title = user.Title,
                                    },
                                    UserId = user.UserId,
                                    Position = insurance.Possition,
                                    PersonalID = insurance.PersonalId,
                                    WorkPhone = insurance.WorkPhone,
                                    Login = user.Login
                                }
                                   ).Where(s => s.InsuranceFullName.Name.Contains(searchString) || s.InsuranceFullName.Surname.Contains(searchString)).ToList();
            }
            model.insurance = Insurance;

            // Řazení dle jednotlivých kritérií nastaveních v sortOrder.
            switch (sortOrder)
            {
                case "byName":
                    model.insurance = model.insurance.OrderBy(o => o.InsuranceFullName.Name).ToList();
                    break;
                default:
                    model.insurance = model.insurance.OrderBy(o => o.InsuranceFullName.Surname).ToList();
                    break;
            }

            //stránkování vybraných dat
            model.PageNum1 = (p ?? 1);
            int pageSize = 5;
            IPagedList<Admin_insurance> lide1 = model.insurance.ToPagedList(model.PageNum1, pageSize);
            model.insurancePage = lide1;

            return View(model);
        }

        /*
         * Akce vytvoří nového pacienta v databázi.
         * Zahrnuje vytvoření Adresy, Uživatele, Pacienta, Uživatele pro Identity FW a přidělení práv.
         */
        [HttpPost]
        public async Task<IActionResult> CreateInsuranceAsync()
        {
            // Hodnoty pro nového pacienta získané přes POST
            string name = Request.Form["NewName"];
            string surname = Request.Form["NewSurname"];
            string login = Request.Form["NewLogin"];
            string title = Request.Form["NewTitle"];
            string tel = Request.Form["NewTel"];
            string email = Request.Form["NewEmail"];
            string street = Request.Form["NewStreet"];
            // houseNum může být nezadáno, nejde převést prázdnou hodnotu na int, proto kontroluji.
            int houseNum = int.Parse(String.IsNullOrEmpty(Request.Form["NewHouseNum"].ToString()) ? "0" : Request.Form["NewHouseNum"].ToString());
            string city = Request.Form["NewCity"];
            int zip = int.Parse(Request.Form["NewZip"].ToString().Replace(" ", ""));    // Může být ve tvaru "739 11"
            string position = Request.Form["NewPosition"];
            string workphone = Request.Form["NewWorkPhone"];

            // Deklarace proměnných pro budoucí vložení do databáze
            Address address;
            User user;


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

            string loginsurname = "";
            if (surname.Length > 7)
            {
                loginsurname = surname.Substring(0, 7);
            }
            else
                loginsurname = surname;

            loginsurname = Regex.Replace(loginsurname, "[éèëêð]", "e");
            loginsurname = Regex.Replace(loginsurname, "[ÉÈËÊ]", "E");
            loginsurname = Regex.Replace(loginsurname, "[àâä]", "a");
            loginsurname = Regex.Replace(loginsurname, "[ÀÁÂÃÄÅ]", "A");
            loginsurname = Regex.Replace(loginsurname, "[àáâãäå]", "a");
            loginsurname = Regex.Replace(loginsurname, "[ÙÚÛÜ]", "U");
            loginsurname = Regex.Replace(loginsurname, "[ùúûüµ]", "u");
            loginsurname = Regex.Replace(loginsurname, "[òóôõöø]", "o");
            loginsurname = Regex.Replace(loginsurname, "[ÒÓÔÕÖØ]", "O");
            loginsurname = Regex.Replace(loginsurname, "[ìíîï]", "i");
            loginsurname = Regex.Replace(loginsurname, "[ÌÍÎÏ]", "I");
            loginsurname = Regex.Replace(loginsurname, "[č]", "c");
            loginsurname = Regex.Replace(loginsurname, "[ř]", "r");
            loginsurname = Regex.Replace(loginsurname, "[ž]", "z");
            loginsurname = Regex.Replace(loginsurname, "[ý]", "y");
            loginsurname = Regex.Replace(loginsurname, "[í]", "i");
            loginsurname = Regex.Replace(loginsurname, "[ů]", "u");
            loginsurname = Regex.Replace(loginsurname, "[ú]", "u");
            loginsurname = Regex.Replace(loginsurname, "[ě]", "e");
            loginsurname = Regex.Replace(loginsurname, "[ť]", "t");
            loginsurname = Regex.Replace(loginsurname, "[ň]", "n");
            loginsurname = Regex.Replace(loginsurname, "[ď]", "d");
            loginsurname = Regex.Replace(loginsurname, "[š]", "s");
            loginsurname = Regex.Replace(loginsurname, "[Š]", "S");
            loginsurname = Regex.Replace(loginsurname, "[ñ]", "n");
            loginsurname = Regex.Replace(loginsurname, "[Ñ]", "N");
            loginsurname = Regex.Replace(loginsurname, "[ç]", "c");
            loginsurname = Regex.Replace(loginsurname, "[Ç]", "C");
            loginsurname = Regex.Replace(loginsurname, "[ÿý]", "y");
            loginsurname = Regex.Replace(loginsurname, "[Ÿ]", "Y");
            loginsurname = Regex.Replace(loginsurname, "[ž]", "z");
            loginsurname = Regex.Replace(loginsurname, "[Ž]", "Z");
            loginsurname = Regex.Replace(loginsurname, "[Ð]", "D");
            loginsurname = Regex.Replace(loginsurname, "[œ]", "oe");
            loginsurname = Regex.Replace(loginsurname, "[Œ]", "Oe");
            loginsurname = Regex.Replace(loginsurname, "[«»\u201C\u201D\u201E\u201F\u2033\u2036]", "\"");
            loginsurname = Regex.Replace(loginsurname, "[\u2026]", "...");
            loginsurname = loginsurname.ToLower();
            int suffix = 0;

            string loginCreator = "x" + loginsurname + String.Format("{0:D2}", suffix);

            List<string> logins = db.UserT.Select(x => x.Login).ToList();
            if (logins.Contains(loginCreator) == true)
            {
                suffix++;
                loginCreator = "x" + surname + String.Format("{0:D2}", suffix);
            }

            // Vytvoření uživatele (Jméno, Příjmení, Titul, ...)
            // Uživatelským jménem pacientů je jejich rodné číslo.
            user = new User
            {
                Login = loginCreator,
                Name = name,
                Surname = surname,
                Title = title,
                Phone = tel,
                Email = email,
                WorkAddress = new Address
                {
                    City = city,
                    StreetName = street,
                    HouseNumber = houseNum,
                    ZIP = zip
                }
            };
            db.UserT.Add(user);
            db.SaveChanges();

            // Vytvoření pracovníka.
            // Napojení Address a User
            db.InsureEmpT.Add(new Data.InsureEmp
            {
                UserId = db.UserT.Where(o => o.Login == user.Login).Select(s => s.UserId).ToList().First(),
                Possition = position,
                WorkPhone = workphone
            }); ;
            db.SaveChanges();

            // Vytvoření nového uživatele pro Identity Framework.
            // Základní heslo je 1234567890, uživatelské jméno je shodné s rodným číslem.
            // Udělení oprávnění Patient.
            var userIdentity = new NemocniceUser { UserName = user.Login };
            var result = await _userManager.CreateAsync(userIdentity, "1234567890");
            await _userManager.AddToRoleAsync(userIdentity, "Insurance");

            // Návrat zpět do kartotéky.
            return RedirectToAction("CardInsurance", new { SortOrder = Request.Form["SortOrder"], p = Request.Form["p"], Search = Request.Form["Search"] });
        }



        public async Task<IActionResult> DoctorEditAsync(string sortOrder, string searchString, string ID_delete, DoctorEditModel model, int? p)
        {
            // Uložení přávě vyhledávaného řetězce.
            // Při řazení výsledků budeme už vědět, o jaké výsledky se jedná.
            ViewData["Search"] = searchString;
            ViewData["CurrentSort"] = sortOrder;
            ViewData["CurrentPage"] = p;

            //pokud je ID_delete různé od 0, víme, jakého doktora chceme odstranit
            if (ID_delete != null)
            {
                var userId = db.DoctorT.Where(x => x.ICZ == int.Parse(ID_delete)).Select(x => x.UserId).FirstOrDefault();
                var userLogin = db.UserT.Where(x => x.UserId == userId).Select(x => x.Login).FirstOrDefault();
                db.UserT.Where(x => x.UserId == userId).First().Deactivated = true;
                var user = await _userManager.FindByNameAsync(userLogin);
                if (user != null)
                {
                    await _userManager.DeleteAsync(user);
                }
                db.SaveChanges();

            }

            // Model - Seznam všech pacientů v databázi
            List<DoctorJoined1> Doctors;

            // Získání údajů ke každému pacientovi (Příjmení, Jméno, Titul, R.Č., pojišťovny).
            // Informace jsou získávány ze spojení dvou tabulek: PatientT (R.Č., pojišťovna) a UserT (Příjmení, Jméno, Titul).
            if (String.IsNullOrEmpty(searchString))
            {
                // Není použito vyhledávání (chceme všechny pacienty)
                Doctors = db.UserT.Include(x => x.WorkAddress).Join(db.DoctorT, user => user.UserId, doctor => doctor.UserId,
                            (user, doctor) => new DoctorJoined1
                            {
                                Doctor = doctor,
                                User = user      
                            }).ToList();
            }
            else
            {
                // Je potřeba vyhledat konkrétní pacienty odpovídající hledanému výrazu.
                // Hledání probíhá skrz položky (Jméno, Příjmení, R.Č.).
                // Rodné číslo je převáděno na string. Hledání probíhá na základě metody StartsWith.
                Doctors = db.UserT.Include(x => x.WorkAddress).Join(db.DoctorT, user => user.UserId, doctor => doctor.UserId,
                            (user, doctor) => new DoctorJoined1
                            {
                                Doctor = doctor,
                                User = user
                            }).Where(s => s.User.Title.Contains(searchString) ||
                                             s.User.Surname.Contains(searchString) ||
                                             s.User.Name.Contains(searchString)).ToList();
            }
            model.doctors = Doctors;

            // Řazení dle jednotlivých kritérií nastaveních v sortOrder.
            switch (sortOrder)
            {
                case "byName":
                    model.doctors = model.doctors.OrderBy(o => o.User.Name).ToList();
                    break;
                case "byICZ":
                    model.doctors = model.doctors.OrderBy(o => o.Doctor.ICZ).ToList();
                    break;
                default:
                    model.doctors = model.doctors.OrderBy(o => o.User.Surname).ToList();
                    break;
            }

            //stránkování vybraných dat
            model.PageNum = (p ?? 1);
            int pageSize = 5;
            IPagedList<DoctorJoined1> lide = model.doctors.ToPagedList(model.PageNum, pageSize);
            model.DoctorJoined = lide;

            return View(model);
        }

        [HttpPost]
        public async Task<IActionResult> CreateDoctor()
        {
            // Hodnoty pro nového pacienta získané přes POST
            string name = Request.Form["NewName"];
            string surname = Request.Form["NewSurname"];
            string title = Request.Form["NewTitle"];
            int socialICZ = int.Parse(Request.Form["NewICZ"]);
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
            if (db.DoctorT.Where(o => o.ICZ == socialICZ).ToList().Count != 0)
            {
                return RedirectToAction("DoctorEdit");
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

            string loginsurname = "";
            if(surname.Length > 7)
                loginsurname = surname.Substring(0, 7);
            else
                loginsurname = surname;
            
            loginsurname = Regex.Replace(loginsurname, "[éèëêð]", "e");
            loginsurname = Regex.Replace(loginsurname, "[ÉÈËÊ]", "E");
            loginsurname = Regex.Replace(loginsurname, "[àâä]", "a");
            loginsurname = Regex.Replace(loginsurname, "[ÀÁÂÃÄÅ]", "A");
            loginsurname = Regex.Replace(loginsurname, "[àáâãäå]", "a");
            loginsurname = Regex.Replace(loginsurname, "[ÙÚÛÜ]", "U");
            loginsurname = Regex.Replace(loginsurname, "[ùúûüµ]", "u");
            loginsurname = Regex.Replace(loginsurname, "[òóôõöø]", "o");
            loginsurname = Regex.Replace(loginsurname, "[ÒÓÔÕÖØ]", "O");
            loginsurname = Regex.Replace(loginsurname, "[ìíîï]", "i");
            loginsurname = Regex.Replace(loginsurname, "[ÌÍÎÏ]", "I");
            loginsurname = Regex.Replace(loginsurname, "[č]", "c");
            loginsurname = Regex.Replace(loginsurname, "[ř]", "r");
            loginsurname = Regex.Replace(loginsurname, "[ž]", "z");
            loginsurname = Regex.Replace(loginsurname, "[ý]", "y");
            loginsurname = Regex.Replace(loginsurname, "[í]", "i");
            loginsurname = Regex.Replace(loginsurname, "[ů]", "u");
            loginsurname = Regex.Replace(loginsurname, "[ú]", "u");
            loginsurname = Regex.Replace(loginsurname, "[ě]", "e");
            loginsurname = Regex.Replace(loginsurname, "[ť]", "t");
            loginsurname = Regex.Replace(loginsurname, "[ň]", "n");
            loginsurname = Regex.Replace(loginsurname, "[ď]", "d");
            loginsurname = Regex.Replace(loginsurname, "[š]", "s");
            loginsurname = Regex.Replace(loginsurname, "[Š]", "S");
            loginsurname = Regex.Replace(loginsurname, "[ñ]", "n");
            loginsurname = Regex.Replace(loginsurname, "[Ñ]", "N");
            loginsurname = Regex.Replace(loginsurname, "[ç]", "c");
            loginsurname = Regex.Replace(loginsurname, "[Ç]", "C");
            loginsurname = Regex.Replace(loginsurname, "[ÿý]", "y");
            loginsurname = Regex.Replace(loginsurname, "[Ÿ]", "Y");
            loginsurname = Regex.Replace(loginsurname, "[ž]", "z");
            loginsurname = Regex.Replace(loginsurname, "[Ž]", "Z");
            loginsurname = Regex.Replace(loginsurname, "[Ð]", "D");
            loginsurname = Regex.Replace(loginsurname, "[œ]", "oe");
            loginsurname = Regex.Replace(loginsurname, "[Œ]", "Oe");
            loginsurname = Regex.Replace(loginsurname, "[«»\u201C\u201D\u201E\u201F\u2033\u2036]", "\"");
            loginsurname = Regex.Replace(loginsurname, "[\u2026]", "...");
            loginsurname = loginsurname.ToLower();
            int suffix = 0;

            string loginCreator = "x" + loginsurname + String.Format("{0:D2}",suffix);

            List<string> logins = db.UserT.Select(x => x.Login).ToList();
            if (logins.Contains(loginCreator) == true)
            {
                suffix++;
                loginCreator = "x" + surname + String.Format("{0:D2}", suffix);
            }



            // Vytvoření uživatele (Jméno, Příjmení, Titul, ...)
            // Uživatelským jménem pacientů je jejich rodné číslo.
            user = new User
            {
                Login = loginCreator,
                Name = name,
                Phone = tel,
                Surname = surname,
                Title = title,
                Email = email,
                WorkAddress = new Address
                {
                    City = city,
                    StreetName = street,
                    HouseNumber = houseNum,
                    ZIP = zip
                }
            };
            db.UserT.Add(user);
            db.SaveChanges();

            // Vytvoření pacienta.
            // Napojení Address a User
            db.DoctorT.Add(new Data.Doctor
            {
                UserId = db.UserT.Where(o => o.Login == user.Login).Select(s => s.UserId).ToList().First(),
                ICZ = socialICZ,
                WorkPhone = tel
            });
            db.SaveChanges();

            // Vytvoření nového uživatele pro Identity Framework.
            // Základní heslo je 1234567890, uživatelské jméno je shodné s rodným číslem.
            // Udělení oprávnění Patient.
            var userIdentity = new NemocniceUser { UserName = user.Login };
            var result = await _userManager.CreateAsync(userIdentity, "1234567890");
            await _userManager.AddToRoleAsync(userIdentity, "Doctor");
            db.SaveChanges();
            // Návrat zpět do kartotéky.
            return RedirectToAction("DoctorEdit", new { SortOrder = Request.Form["SortOrder"], p = Request.Form["p"], Search = Request.Form["Search"] });
        }

        [HttpPost]
        public IActionResult EditDb_Doctor()
        {

            string edit_ID = Request.Form["edit_ID"];
            string edit_name = Request.Form["edit_name"];
            string edit_surname = Request.Form["edit_surname"];
            int edit_ICZ = int.Parse(Request.Form["edit_icz"]);
            string edit_tel = Request.Form["edit_tel"];
            string edit_mail = Request.Form["edit_mail"];
            string edit_title = Request.Form["edit_title"];

            var pom = db.UserT.Include(x => x.WorkAddress).First(a => a.Login == edit_ID);
            pom.Name = edit_name;
            pom.Surname = edit_surname;
            pom.Title = edit_title;
            pom.Phone = edit_tel;
            pom.Email = edit_mail;

            var DoctorInfo = db.DoctorT.Where(x => x.UserId == pom.UserId).First();
            DoctorInfo.ICZ = edit_ICZ;
            DoctorInfo.WorkPhone = edit_tel;

            var work = db.UserT.Include(x => x.WorkAddress).Where(a => a.Login == edit_ID).Select(s => s.WorkAddress).FirstOrDefault();

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

            return RedirectToAction("DoctorEdit", new { SortOrder = Request.Form["SortOrder"], p = Request.Form["p"], Search = Request.Form["Search"] });
        }

        public JsonResult TestSocialSecurityNumUnique(string num)
        {
            bool result = true;

            if (db.PatientT.Select(s => s.SocialSecurityNum).Where(o => o == num).Any())
            {
                result = false;
            }

            return new JsonResult(result);
        }


    }

}

