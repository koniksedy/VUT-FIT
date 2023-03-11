﻿/*
 * Soubor kontroleru uživatele s rolí Doctor, nebo Admin.
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15>, Kateřina Kunorzová <xkunor00>
 */

/*
 * Testovací makro, když je nastaveno nejsou pacienti filtrování dle doktorů.
 * Všichni doktori vydí všechno.
 * TODO - odstranit
 */
#define TEST    

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Http;
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
    /*
     * Třída kontroleru zpracovávající požadavky uživatele s oprávněním Doktor, nebo Admin.
     * K jednotlivým akcím a následně zobrazením může přistoupit pouze oprávněný uživatel.
     */
    [Authorize(Roles = "Doctor,Admin")]
    public class Doctor : Controller
    {
        /*
         * Informace o právě přihlášeném uživateli.
         */
        private readonly UserManager<NemocniceUser> _userManager;
        private readonly IWebHostEnvironment webHostEnvironment;

        /*
         * Proměnná umožňující napříč kontrollerem přistupovat k databázi
         */
        private DatabaseContext db { get; set; } = new DatabaseContext();

        public Doctor(UserManager<NemocniceUser> userManager, IWebHostEnvironment hostEnvironment)
        {
            _userManager = userManager;
            webHostEnvironment = hostEnvironment;
        }

        /*
         * Akce index přesměruje na hlavní stránku.
         */
        public IActionResult Index()
        {
            return RedirectToAction("Index", "Home");
        }

        /*
         * Akce karotéky. Vypíše všechny pacienty uložené v databázi.
         * sortOrder - typ řazení (podle jnéma, příjmení, rodného čísla)
         * searchString - hledaný řetězec (v případě vyhledávání)
         */
        public IActionResult Card(string sortOrder, string searchString, CardModel model, int ? p)
        {
            // Uložení přávě vyhledávaného řetězce.
            // Při řazení výsledků budeme už vědět, o jaké výsledky se jedná.
            ViewData["Search"] = searchString;
            ViewData["CurrentSort"] = sortOrder;
            ViewData["CurrentPage"] = p;

            // Model - Seznam všech pacientů v databázi
            List<CardModel> Patients;
            
            // Získání údajů ke každému pacientovi (Příjmení, Jméno, Titul, R.Č., pojišťovny).
            // Informace jsou získávány ze spojení dvou tabulek: PatientT (R.Č., pojišťovna) a UserT (Příjmení, Jméno, Titul).
            if (String.IsNullOrEmpty(searchString))
            {
                // Není použito vyhledávání (chceme všechny pacienty)
                Patients =  db.PatientT.Join(db.UserT,
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

            // Řazení dle jednotlivých krytérií nastaveních v sortOrder.
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
            int pageSize = 8;
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
            return RedirectToAction("Card");
        }


        /*
         * Akce zobrazí zprávy (ne žádanek a jejich výsledků) pojících se k pacientovi.
         * V základu slouží k vytvoření nové zprávy.
         * Umožňuje zobrazit a upravoval libovolné stare zprávy.
         * Netoda zpřístupní pouze ty zápisy, jejichž správcem je přihlášený Doktor.
         * Pokud se jedná o Administrátora, jsou mi zpřístupněny zprávy všech dokutorů
         * socialNum - rodné číslo zobrazovaného pacienta
         * date - datum zobrazovaného zápisu (pro nový zápis se rovná default)
         */
        public IActionResult Report(string socialNum, DateTime date)
        {
            // Získání ID přihlášeného doktora pro kontrolu přístupu ke zprávám.
            string userDoctor = User.Identity.Name;
            // HACK - pokud bude databáze nekonzistentní a přihlášený uživatel nebude mít data v tabulce UserT, dojde k chybě.
            int doctorId = db.UserT.Where(s => s.Login == userDoctor).Select(o => o.UserId).ToList().First();
            PatientReportsModel reportsModel = new PatientReportsModel();

            // Kontrola modelu
            if (!ModelState.IsValid)
            {
                return View(new PatientReportsModel());
            }

            IQueryable<MedicallReport> patientDoctorReports;
            // Pokud je uživatl v roli admin, pak vidí zprávy od všech lékařů,
            // v opačném případě pouze zprávy, kterých je zprávcem.
            if(User.IsInRole("Admin"))
            {
                patientDoctorReports = db.MedicallReportT.Where(o => o.Patient.SocialSecurityNum == socialNum);
            }
            else
            {
                patientDoctorReports = db.MedicallReportT.Where(o => (o.Patient.SocialSecurityNum == socialNum) && o.Owner.UserId == doctorId);
            }

            // Pokud je datum default, pak se vytváří nová zpráva, jinak se zobrazuje stará.
            reportsModel.ActualReportDate = (date == default) ? DateTime.Now : date;
            // Soubor všech zpráv, pouze Data(datum). (to stačí)
            reportsModel.AllReports = patientDoctorReports.Select(s => s.CreateDate).ToList();

            // Pokud má pacient již nějakou starší zprávu, vybereme tu o jednu jednotku starší a zobrazíme.
            if (reportsModel.AllReports.Any())
            {
                // Seřazení časů všech pacientových zpráv.
                reportsModel.AllReports = reportsModel.AllReports.OrderByDescending(x => x).ToList();

                // Kontrola, zda je vytvářená nová zpráva.
                if (date == default)
                {
                    // Pokud je vytvářená nová zpráva zpbrazíme také poslední zprávu.
                    reportsModel.PreviousReportDate = reportsModel.AllReports.First();
                    reportsModel.PreviousReport = patientDoctorReports.Where(o => o.CreateDate == reportsModel.PreviousReportDate)
                                                                      .Select(s => s.Description).ToList().First();
                }
                else
                {
                    // Zobrazujeme starší zprávu. Jako aktuální (k případné úpravě) bude hledaná zpráva.
                    reportsModel.ActualReport = patientDoctorReports.Where(o => o.CreateDate == date).Select(s => s.Description).ToList().First();

                    // Získání zprávy o jednu jednotku starší, pokud taková existuje.
                    var oldCreateDateIdx = reportsModel.AllReports.FindIndex(o => o == date) + 1;
                    if (oldCreateDateIdx < reportsModel.AllReports.Count)
                    {
                        reportsModel.PreviousReportDate = reportsModel.AllReports.ElementAt(oldCreateDateIdx);
                        reportsModel.PreviousReport = patientDoctorReports.Where(o => o.CreateDate == reportsModel.PreviousReportDate)
                                                                          .Select(s => s.Description).ToList().First();
                    }
                    else
                    {
                        reportsModel.PreviousReportDate = default;
                        reportsModel.PreviousReport = "";
                    }
                }
            }
            else
            {
                // Píšeme novou zprávu a pacient nemá žádnou předchozí.
                reportsModel.PreviousReportDate = default;
                reportsModel.PreviousReport = "";
            }

            // Získání informací o pacientovi.
            Data.Patient patient = db.PatientT.Where(o => o.SocialSecurityNum == socialNum).ToList().First();
            Data.User userPatient = db.UserT.Where(o => o.UserId == patient.UserId).ToList().First();

            // Uložení informací o pacientovi do modelu.
            reportsModel.PatientFullName = new NameModel
            {
                Name = userPatient.Name,
                Surname = userPatient.Surname,
                Title = userPatient.Title
            };
            reportsModel.Insurance = patient.InsuranceCompany;
            reportsModel.SocialSecurityNumber = socialNum;

            // Získání všech obrázků pacienta ke zprávě
            var test = db.PictureOnReportT.Where(o => o.Report.CreateDate == reportsModel.ActualReportDate &&
                                                                   o.Picture.SocialSecurityNum == socialNum)
                                                       .ToList();
            reportsModel.Pictures = db.PictureOnReportT.Where(o => o.Report.CreateDate == reportsModel.ActualReportDate &&
                                                                   o.Picture.SocialSecurityNum == socialNum)
                                                       .Select(s => new PictureJsonModel
                                                       {
                                                           id = s.Picture.NameInt,
                                                           name = s.Picture.Description,
                                                           date = s.Picture.CreateDate.ToString(),
                                                           type = s.Picture.Type
                                                       }).ToList();

            // Přidání zpráv z vyšetření
            if(date == default)
            {
                reportsModel.AllTickets = db.CheckupTicketT.Where(o => o.Patient.SocialSecurityNum == patient.SocialSecurityNum &&
                                                                       (o.CreatedBy.UserId == doctorId || User.IsInRole("Admin")) &&
                                                                       o.FinishDate != default)
                                                            .Select(s => s.FinishDate).ToList();
            }

            return View(reportsModel);
        }


        /*
         * Akce vytvoří novou zprávu. S tím souvisí aktualizace/vytvoření
         * zdravotního stavu, vytvoření výkazu pro pojišťovnu a aktualizace stavu pacienta.
         */
        [HttpPost]
        public IActionResult CreateReport()
        {
            // Získání potřebných hodnot o pacientovi, zprávě, diagnozách a výkonu pomocí POST.
            string patientNum = Request.Form["PatientNum"];
            string reportText = Request.Form["ReportText"];
            string billString = Request.Form["Bill"];
            int billCode = int.Parse(Request.Form["Bill"]);
            // HACK - Získáním data z POSTU se oříznou hodnoty menší než sekundy,
            // datum se uniformuje pouze na rok,měsíc,den,hodinu,minutu,sekudy.
            DateTime reportDate = DateTime.Parse(Request.Form["ReportDate"]);

            // Získání kódů všech diagnóz a jejich stavů přidělených ke zprávě.
            List<int> diagnosis = new List<int>();
            List<string> diagnosisState = new List<string>();
            diagnosis.Add(int.Parse(Request.Form["Diagnosis1"]));
            diagnosisState.Add(Request.Form["Cured1"]);
            if (!String.IsNullOrEmpty(Request.Form["Diagnosis2"]))
            {
                diagnosis.Add(int.Parse(Request.Form["Diagnosis2"]));
                diagnosisState.Add(Request.Form["Cured2"]);
            }
            if (!String.IsNullOrEmpty(Request.Form["Diagnosis3"]))
            {
                diagnosis.Add(int.Parse(Request.Form["Diagnosis3"]));
                diagnosisState.Add(Request.Form["Cured3"]);
            }
            if (!String.IsNullOrEmpty(Request.Form["Diagnosis4"]))
            {
                diagnosis.Add(int.Parse(Request.Form["Diagnosis4"]));
                diagnosisState.Add(Request.Form["Cured4"]);
            }

            // Zíkání informací o doktorovi, který zprávu napsal.
            // HACK - pokud nebude přihášený uživatl mít zíznam v tabulce UserT, dojde k erroru.
            string userDoctor = User.Identity.Name;
            var doctorId = db.UserT.Where(s => s.Login == userDoctor).Select(o => o.UserId).ToList().First();

            // Pokud je přihlášeným uživatelem admin, a ještě nemá přiřezený
            // svůj pracovní doktorský profil, vytvoříme ho.
            Data.Doctor doctor;
            if (User.IsInRole("Admin"))
            {
                var tmp = db.DoctorT.Where(o => o.UserId == doctorId).ToList();
                if (!tmp.Any())
                {
                    // Vytvoření pracovního doktorského profilu pro administrátora
                    var tmpTel = db.AdminT.Where(o => o.UserId == doctorId).Select(s => s.WorkPhone).ToList().First();
                    db.DoctorT.Add(new Data.Doctor
                    {
                        UserId = doctorId,
                        WorkPhone = tmpTel
                    });
                    db.SaveChanges();

                    doctor = db.DoctorT.Where(o => o.UserId == doctorId).ToList().First();
                }
                else
                {
                    // Pracovní profil již existuje.
                    doctor = tmp.First();
                }
            }
            else
            {
                doctor = db.DoctorT.Where(o => o.UserId == doctorId).ToList().First();
            }
            
            // Vytvoření zprávy
            MedicallReport report = new MedicallReport
            {
                Author = doctor,
                Patient = db.PatientT.Where(o => o.SocialSecurityNum == patientNum).ToList().First(),
                Description = reportText,
                Owner = doctor,
                CreateDate = reportDate
            };
            db.MedicallReportT.Add(report);
            db.SaveChanges();

            // Registrace výkonu s hlavní diagnozou
            // HACK - Kód výkonu musí v databázi výkonú existovat.
            MedicallBill bill = new MedicallBill()
            {
                Doctor = doctor,
                SocialSecurityNum = patientNum,
                MedicallActivityPrice = db.MedicallActivityPriceT.Where(o => o.MedicallActivityPriceId == billCode).ToList().First(),
                Diagnosis = db.DiagnosisT.Where(o => o.DiagnosisId == diagnosis.First()).ToList().First(),
                CreateDate = reportDate
            };
            db.MedicallBillT.Add(bill);
            db.SaveChanges();

            // Aktualizace TreatementLogu a CureProgressu pacienta
            // TreatementLog - přehled záčátku a konce léčby dané diagnozy
            // CureProgress - propojení každé lékařské zprávy, a diagnozy se zhodnocením léčby (zahájení, kontrola, ukončení)
            var oldTreatementLog = db.PatientTreatmentLogT.Where(o => o.Patient.SocialSecurityNum == patientNum);
            for(int i = 0; i < diagnosis.Count; i++)
            {
                // Všechny neukonžené léčby pacienta.
                List<PatientTreatmentLog> openDiagnosis = oldTreatementLog.Any() ?
                    oldTreatementLog.Where(o => o.Diagnosis.DiagnosisId == diagnosis.ElementAt(i) && o.EndOfTreatment == default).ToList() :
                    new List<PatientTreatmentLog>();

                // Různá funkciolalita pro ukončované a zahajované léčby.
                if (diagnosisState.ElementAt(i) == "cured")
                {
                    // Kontrola, zda existuje nějaká otevřená léčba dané diagnózy,
                    // kterou je možné ukončit.
                    if(openDiagnosis.Any())
                    {
                        // Ukončení existující léčby
                        openDiagnosis.First().EndOfTreatment = reportDate;
                    }
                    else
                    {
                        // Vytvoření a okamžité ukončení léčby
                        db.PatientTreatmentLogT.Add(new PatientTreatmentLog
                        {
                            Patient = db.PatientT.Where(o => o.SocialSecurityNum == patientNum).ToList().First(),
                            Diagnosis = db.DiagnosisT.Where(o => o.DiagnosisId == diagnosis.ElementAt(i)).ToList().First(),
                            StartOfTreatment = reportDate,
                            EndOfTreatment = reportDate
                        });
                    }
                    db.SaveChanges();
                }
                else
                {
                    // Kontrola, zda existuje už nějaká otevřená léčba s touto diagnozou.
                    // Pokud ne, pak se vytvoří nová léčba, v opačném případě se jedná pouze o kontrolu.
                    if(!openDiagnosis.Any())
                    {
                        // Vytvoření nové léčby pacienta
                        db.PatientTreatmentLogT.Add(new PatientTreatmentLog
                        {
                            Patient = db.PatientT.Where(o => o.SocialSecurityNum == patientNum).ToList().First(),
                            Diagnosis = db.DiagnosisT.Where(o => o.DiagnosisId == diagnosis.ElementAt(i)).ToList().First(),
                            StartOfTreatment = reportDate
                        });
                        db.SaveChanges();
                    }
                }
                
                // Určení CureProgress pro každou diagnozu vrámci zprávy. (Zahájení léčby, kontrola, konec)
                db.CureProgressT.Add(new CureProgress
                {
                    MedicallReport = report,
                    Diagnosis = db.DiagnosisT.Where(o => o.DiagnosisId == diagnosis.ElementAt(i)).ToList().First(),
                    StateOfTreatment = (diagnosisState.ElementAt(i) == "cured") ?
                                       (openDiagnosis.Any() ?
                                            "Ukončení léčby" :
                                            "Jednorázová léčba"):
                                       (openDiagnosis.Any() ?
                                            "Kontrola" :
                                            "Zahájení léčby")
                });
                db.SaveChanges();
            }

            // Aktualizace Healthcondition pacienta, konkrétně datum jeho poslední návštěvy.
            // Pokud nemá pacient vytvořen HealthCondition, vytvoříme jej.       
            Data.Patient patient = db.PatientT.Where(o => o.SocialSecurityNum == patientNum).Include(s => s.HealthCondition).ToList().First();
            if (patient.HealthCondition == null)
            {
                patient.HealthCondition = new HealthCondition { SocialSecurityNum = patientNum };
            }
            patient.HealthCondition.LastCheckupDate = reportDate;
            db.SaveChanges();


            // Přilinkování obrázků
            // Existujících
            if (Request.Form.Keys.Contains("FileExist"))
            {
                try
                {
                    foreach (string nameString in Request.Form["FileExist"])
                    {
                        int name = int.Parse(nameString);
                        Picture pic = db.PictureT.Where(o => o.NameInt == name).ToList().First();
                        db.PictureOnReportT.Add(new PictureOnReport
                        {
                            Picture = pic,
                            Report = report
                        });
                    }
                    db.SaveChanges();
                } 
                catch
                {
                    // catched
                }
            }

            // Nových
            int maxName = 0;
            if (db.PictureT.Any())
            {
                maxName = db.PictureT.Max(m => m.PictureId);
            }
            foreach (IFormFile image in Request.Form.Files)
            {
                UploadImage(image, webHostEnvironment, maxName);
                Picture pic = new Picture
                {
                    NameInt = maxName,
                    Description = image.FileName,
                    SocialSecurityNum = patientNum,
                    CreateDate = TimeNowTruncateToSec(),
                    Type = image.FileName.Substring(image.FileName.LastIndexOf("."))
                };
                db.PictureT.Add(pic);
                db.SaveChanges();
                db.PictureOnReportT.Add(new PictureOnReport
                {
                    Picture = pic,
                    Report = report
                });
                db.SaveChanges();
                maxName++;
            }


            return RedirectToAction("PatientProfile", new { patientNum = patientNum});
        }

        /*
         * Akce upraví text staré zprávy.
         * Není zde prováděná kontrola na lékaře.
         * Předpokládá se, že do systému se neuloží ve stejný čas pro pacienta dvě stejné zprávy.
         */
        [HttpPost]
        public IActionResult UpdateReport()
        {
            // Získání dat o konkrétním pacientovi a datu zprávy z POST
            DateTime date = DateTime.Parse(Request.Form["ReportDate"]);
            string patientNum = Request.Form["PatientNum"];
            string reportText = Request.Form["ReportText"];

            // Aktualizace textu.
            db.MedicallReportT.Where(o => o.Patient.SocialSecurityNum == patientNum && o.CreateDate == date)
                              .ToList().First().Description = reportText;
            db.SaveChanges();

            return RedirectToAction("PatientProfile", new { patientNum });
        }

        /*
         * Funkce pro získání věku z rodného čísla.
         * Funkce je převzata z: https://www.c-sharpcorner.com/code/961/how-to-calculate-age-from-date-of-birth-in-c-sharp.aspx
         * patientNum - rodné číslo
         * Funkce vrací textovou podobu věku ve tvaru "10 Let 9 Měsíců".
         * Pokud se výpočet nezdaří bude vrácoeno "DateError"
         */
        static private string getAge(string patientNum)
        {
            // Převod rodného čísla na datum
            int yearInt = int.Parse(patientNum.Substring(0, 2));
            int monInt = int.Parse(patientNum.Substring(2, 2));
            int dayInt = int.Parse(patientNum.Substring(4, 2));
            yearInt = (yearInt < 54 && patientNum.Length > 9) ? 2000 + yearInt : 1900 + yearInt;

            // Získání počtu roků a měsíců ze dnů, které vrátí funkce Subtract
            int Years = 0;
            int Months = 0;
            try
            {
                monInt = (monInt > 50) ? monInt - 50 : monInt;
                DateTime birth = new DateTime(yearInt, monInt, dayInt);
                DateTime Now = DateTime.Now;
                Years = new DateTime(DateTime.Now.Subtract(birth).Ticks).Year - 1;
                DateTime PastYearDate = birth.AddYears(Years);
                Months = 0;
                for (int i = 1; i <= 12; i++)
                {
                    if (PastYearDate.AddMonths(i) == Now)
                    {
                        Months = i;
                        break;
                    }
                    else if (PastYearDate.AddMonths(i) >= Now)
                    {
                        Months = i - 1;
                        break;
                    }
                }
            }
            catch
            {
                return "DateError";
            }

            // Zprávné vyskloňování Roků a Měsíců
            return String.Format("{0} {1} {2} {3}", Years, 
                                                    (Years == 1) ?
                                                    "Rok" :
                                                    (Years < 5 && Years != 0) ?
                                                        "Roky" :
                                                        "Let",
                                                    Months,
                                                    (Months == 1) ?
                                                        "Měsíc" :
                                                        (Months < 5 && Months != 0) ?
                                                            "Měsíce" :
                                                            "Měsíců");
        }


        /*
         * Akce zobrazí hlavní kartu pacienta se všemi informacemi.
         * patientNum - rodné číslo pacienta, jehož karta je zobrazována.
         */
        public IActionResult PatientProfile(string patientNum)
        {
            // Získání informaci o lékaři, který si pacienta zobrazuje.
            // HACK - pokud by uživatel nebyl v UserT pak dojde k chybě.
            string user = User.Identity.Name;
            var doctorId = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList().First();

            // Model s informacemi pro zobrazení v kartě pacienta.
            PatientProfileModel patientProfileModel = new PatientProfileModel();

            // Získání informací o pacientovi.
            // HACK - Může se stát, že bylo rozné číslo pacienta změněno, ale při použití
            // caschování a šipky zpět byl proveden dotaz na již neexistující rodné číslo.
            // V takovém případě bude řízení přesměrováno zpět do kartotéky.
            Data.Patient patient;
            try
            {
                patient = db.PatientT.Where(o => o.SocialSecurityNum == patientNum).Include(i => i.HealthCondition).Include(i => i.HomeAddress).ToList().First();
            }
            catch
            {
                return RedirectToAction("Card");
            }

            // Získání základní informací o pacientovi
            Data.User patientUser = db.UserT.Where(o => o.UserId == patient.UserId).ToList().First();
            patientProfileModel.PatientFullName = new NameModel
            {
                Name = patientUser.Name,
                Surname = patientUser.Surname,
                Title = patientUser.Title
            };
            patientProfileModel.InsuranceCompany = patient.InsuranceCompany;
            patientProfileModel.SocialSecurityNumber = patientNum;

            patientProfileModel.Tel = patientUser.Phone;
            patientProfileModel.Email = patientUser.Email;
            patientProfileModel.Age = getAge(patientNum);
            patientProfileModel.Address = patient.HomeAddress;
            patientProfileModel.HealthCondition = patient.HealthCondition;

            // Pokud neexistuje HealthCondition, vytvoříme jej.
            if(patientProfileModel.HealthCondition == default)
            {
                patient.HealthCondition = new HealthCondition { SocialSecurityNum = patientNum };
                db.SaveChanges();
            }

            // Získání alergií.
            patientProfileModel.Allergys = db.PatientT.Where(o => o.SocialSecurityNum == patientNum)
                                                      .Select(s => s.HealthCondition.Allergys).ToList().First();
            if (String.IsNullOrEmpty(patientProfileModel.Allergys))
            {
                patientProfileModel.Allergys = "Sine";
            }

            // Získání lékařských zpráv, jejichž autorem je přihlášený lékař.
            // Pokud de přihlášen administrátor, vidí všechny zprávy.
            patientProfileModel.AllReports = db.MedicallReportT.Where(o => o.Patient.SocialSecurityNum == patientNum &&
                                                                           (User.IsInRole("Admin") || o.Owner.UserId == doctorId))
                                                                .Select(s => new ReportsModel
                                                                {
                                                                    Date = s.CreateDate,
                                                                    Text = (s.Description.Length < 40) ? s.Description : (s.Description.Substring(0, 40) + "...")
                                                                }).ToList().OrderByDescending(o => o.Date).ToList();

            // Získání příchozích žádostí o vyšetření, které čekají na vyřízení.
            // Pokud je přihlášený administrátor, uvidí žádosti určené všem doktorům.
            patientProfileModel.CheckupToMe = db.CheckupTicketT.Include(i => i.CreatedBy).Where(o => o.Patient.SocialSecurityNum == patientNum && 
                                                                           o.State != "dokončeno" &&
                                                                           (User.IsInRole("Admin") || o.ToDoctor.UserId == doctorId))
                                                               .Join(db.UserT,
                                                                     checkup => checkup.CreatedBy.UserId,
                                                                     user => user.UserId,
                                                                     (checkup, user) => new CheckupToMeLightModel
                                                                     {
                                                                         CreateDate = checkup.CreateDate,
                                                                         FromDoctor = user.getFullName()
                                                                     }).ToList().OrderByDescending(o => o.CreateDate).ToList();

            // Získání všech vytvořených žádostí pro ostatní lékaře.
            // Pokud je přihlášený administrátor, uvidí žádosti vytvořené všemi doktory.
            patientProfileModel.CheckupToOther = db.CheckupTicketT.Where(o => o.Patient.SocialSecurityNum == patientNum &&
                                                                              (User.IsInRole("Admin") || o.CreatedBy.UserId == doctorId))
                                                                  .Join(db.UserT,
                                                                        checkup => checkup.ToDoctor.UserId,
                                                                        user => user.UserId,
                                                                        (checkup, user) => new CheckupToOtherLightModle
                                                                        {
                                                                            CreateDate = checkup.CreateDate,
                                                                            ToDoctor = user.getFullName(),
                                                                            State = checkup.State
                                                                        }).ToList().OrderByDescending(o => o.CreateDate).ToList();

            return View(patientProfileModel);
        }

        // Akce aktualizuje základní informace o pacientovi (Adresa, jméno, r.č., ...)
        [HttpPost]
        public async Task<IActionResult> UpdatePatientInfoAsync()
        {
            // Získání nových dat o uživateli z meotdy POST.
            string oldpatientNumber = Request.Form["OldNum"];
            // User
            string name = Request.Form["UpdateName"];
            string surname = Request.Form["UpdateSurname"];
            string tel = Request.Form["UpdateTel"];
            string email = Request.Form["UpdateEmail"];
            // Patient
            string patientNumber = Request.Form["UpdateNum"];
            int insurance = int.Parse(Request.Form["UpdateInsurance"]);
            // Adress
            string street = Request.Form["UpdateStreet"];
            int houseNum = int.Parse(Request.Form["UpdateHouseNum"]);
            string city = Request.Form["UpdateCity"];
            int zip = int.Parse(Request.Form["UpdateZip"]);


            // Získání instance upravovaného pacietna z databáze.
            // Získání pacienta musí probáhat podle starého rodného čísla,
            // protoře při úpravě mohlo být změněno (a pacient s tímto změněným číslem ještě neexituje).
            Data.Patient patient = db.PatientT.Where(o => o.SocialSecurityNum == oldpatientNumber)
                                              .Include(s => s.HomeAddress)
                                              .Include(s => s.HealthCondition).ToList().First();
            // Aktualizace User
            Data.User user = db.UserT.Where(o => o.UserId == patient.UserId).ToList().First();
            user.Name = name;
            user.Surname = surname;
            user.Phone = tel;
            user.Email = email;

            // Aktualizace Patient
            patient.InsuranceCompany = insurance;
            // Pokud se mění rodné číslo, je potřeba změnit také rodné číslo v HalthCondition.
            if(patientNumber != oldpatientNumber)
            {
                patient.SocialSecurityNum = patientNumber;
                if(patient.HealthCondition.SocialSecurityNum != default)
                {
                    patient.HealthCondition.SocialSecurityNum = patientNumber;
                }
                else
                {
                    // Pokud HealthCondition neexistuje, vytvoříme jej.
                    patient.HealthCondition = new HealthCondition { SocialSecurityNum = patientNumber };
                }

                foreach (Picture image in db.PictureT.Where(o => o.SocialSecurityNum == oldpatientNumber))
                {
                    image.SocialSecurityNum = patient.SocialSecurityNum;
                }

                var patientUser = await _userManager.FindByNameAsync(oldpatientNumber);
                patientUser.UserName = patient.SocialSecurityNum;
                await _userManager.UpdateAsync(patientUser);
            }

            // Aktualizace Address
            patient.HomeAddress.StreetName = street;
            patient.HomeAddress.HouseNumber = houseNum;
            patient.HomeAddress.City = city;
            patient.HomeAddress.ZIP = zip;
            db.SaveChanges();

            // Po aktualizaci pacienta je provedeno přesněrování na kartu pacienta s těmito novými údaji.
            return RedirectToAction("PatientProfile", new { patientNum = patientNumber});
        }

        /*
         * Akce aktualizuje stav pacienta (Typ krve, výška, váha, ...)
         */
        [HttpPost]
        public IActionResult UpdateHealthInfo()
        {
            
            // Získání dat o pacientovi a zdravotním stavu z metody POST
            string patientNumber = Request.Form["PatientNum"];
            
            int patientHeight = int.Parse(String.IsNullOrEmpty(Request.Form["UpdateHeight"]) ? "0" : Request.Form["UpdateHeight"].ToString());
            int patientWeight = int.Parse(String.IsNullOrEmpty(Request.Form["UpdateWeight"]) ? "0" : Request.Form["UpdateWeight"].ToString());
            string patientBlodType = Request.Form["UpdateBlodType"];
            string patientAlergys = Request.Form["UpdateAllergys"];

            // Získání a aktualizace zdravotního stavu.
            HealthCondition healthCondition = db.PatientT.Where(o => o.SocialSecurityNum == patientNumber).Select(s => s.HealthCondition).ToList().First();
            healthCondition.Height = patientHeight;
            healthCondition.Weight = patientWeight;
            healthCondition.BloodType = patientBlodType;
            healthCondition.Allergys = patientAlergys;
            db.SaveChanges();
            
            // Přesměrování na kartu pacienta s upravenými údaji.
            return RedirectToAction("PatientProfile", new { patientNum = patientNumber });
        }


        public IActionResult PrepareForCheckupIn(string patientNum, DateTime date)
        {
            // Získání informaci o lékaři, který si pacienta zobrazuje.
            // HACK - pokud by uživatel nebyl v UserT pak dojde k chybě.
            string user = User.Identity.Name;
            var doctorId = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList().First();

            // Získání lékařských zpráv, jejichž autorem je přihlášený lékař.
            // Pokud de přihlášen administrátor, vidí všechny zprávy.
            List<DateTime> reports = db.MedicallReportT.Where(o => o.Patient.SocialSecurityNum == patientNum
#if (!TEST)
            &&
                                                                           (User.IsInRole("Admin") || o.Owner.UserId == doctorId)
#endif
                                                                           )
                                                                .Select(s => s.CreateDate)
                                                                .ToList().OrderByDescending(o => o).ToList();

            return RedirectToAction("CheckupIn", new { patientNum, date });

        }


        /*
         * Akce zpbrazí příchozí žádost o vyšetření daného pacienta.
         * patientNum - rodné číslo zobrazovaného pacienta
         * date - datum vytvoření konkrétní zobrazované žádosti.
         */
        public IActionResult CheckupIn(string patientNum,  DateTime date, List<DateTime> reports)
        { 
            // Získání dané žádosti o vyšetření
            CheckupTicket checkupTicket = db.CheckupTicketT.Where(o => o.Patient.SocialSecurityNum == patientNum && o.CreateDate == date)
                                                           .Include(i => i.CreatedBy)
                                                           .Include(i => i.Patient).ToList().First();

            // Získání entit pacienta a doktora, který žádost vytvořil
            Data.User patient = db.UserT.Where(o => o.UserId == checkupTicket.Patient.UserId).ToList().First();
            Data.User doctorFrom = db.UserT.Where(o => o.UserId == checkupTicket.CreatedBy.UserId).ToList().First();

            // Vytvoření modelu pro zobrazení příchozí žádosti.
            CheckupTicketModel model = new CheckupTicketModel
            {
                PatientFullName = new NameModel
                {
                    Name = patient.Name,
                    Surname = patient.Surname,
                    Title = patient.Title
                },
                SocialSecurityNumber = patientNum,
                DoctorFullName = new NameModel
                {
                    Name = doctorFrom.Name,
                    Surname = doctorFrom.Surname,
                    Title = doctorFrom.Title
                },
                State = checkupTicket.State,
                CreateDate = date,
                RequestText = checkupTicket.Description,
                AllReports = reports
            };

            // Získání obrázků
            model.Pictures = db.PictureOnTicketsT.Where(o => o.Ticket.Patient.SocialSecurityNum == patientNum &&
                                                               o.Ticket.CreateDate == model.CreateDate)
                                                 .Select(s => new PictureJsonModel
                                                 {
                                                     id = s.Picture.NameInt,
                                                     name = s.Picture.Description,
                                                     date = s.Picture.CreateDate.ToString(),
                                                     type = s.Picture.Type  
                                                 }).ToList();

            return View(model);
        }

        public IActionResult CheckupInFin(string patientNum, DateTime date)
        {

            // Získání dané žádosti o vyšetření
            CheckupTicket checkupTicket = db.CheckupTicketT.Where(o => o.Patient.SocialSecurityNum == patientNum && o.CreateDate == date)
                                                           .Include(i => i.CreatedBy)
                                                           .Include(i => i.Patient).ToList().First();

            // Získání informací o pacientovi a doktorovi
            Data.User patient = db.UserT.Where(o => o.UserId == checkupTicket.Patient.UserId).ToList().First();
            Data.User doctorFrom = db.UserT.Where(o => o.UserId == checkupTicket.CreatedBy.UserId).ToList().First();

            // Naplnění modelu pro zobrazení
            CheckupTicketModel model = new CheckupTicketModel
            {
                PatientFullName = new NameModel
                {
                    Name = patient.Name,
                    Surname = patient.Surname,
                    Title = patient.Title
                },
                SocialSecurityNumber = patientNum,
                DoctorFullName = new NameModel
                {
                    Name = doctorFrom.Name,
                    Surname = doctorFrom.Surname,
                    Title = doctorFrom.Title
                },
                State = checkupTicket.State,
                CreateDate = date,
                FinishDate = checkupTicket.FinishDate,
                RequestText = checkupTicket.Description,
                ReportText = checkupTicket.Result
            };
            // Získání obrázků
            model.Pictures = db.PictureOnTicketsT.Where(o => o.Ticket.Patient.SocialSecurityNum == patientNum &&
                                                               o.Ticket.CreateDate == model.CreateDate)
                                                 .Select(s => new PictureJsonModel
                                                 {
                                                     id = s.Picture.NameInt,
                                                     name = s.Picture.Description,
                                                     date = s.Picture.CreateDate.ToString(),
                                                     type = s.Picture.Type  
                                                 }).ToList();

            return View(model);
        }

        /*
         * Akce aktualizuje stav příchozí žádosti o vyšetření.
         */
        [HttpPost]
        public IActionResult TicketActualizeState()
        {
            // Získání informací o Pcientovi, žádosti a novém stavu z metody POST.
            string patientNumber = Request.Form["patientNumber"];
            DateTime createDate = DateTime.Parse(Request.Form["ticketDate"]);
            string newState = Request.Form["newState"];

            // Aktualizace stavu dané žádosti a aktualizace databáze.
            db.CheckupTicketT.Where(o => o.Patient.SocialSecurityNum == patientNumber && o.CreateDate == createDate)
                .ToList().First().State = newState;
            db.SaveChanges();

            // Přesněrování na stejnou žádost s aktualizovaným stavem.
            return RedirectToAction("CheckupIn", new { patientNum = patientNumber, date = createDate });
        }

        /*
         * Akce ukončí žádost o vyšetření jako vyřešenou s příslušnýnou zprávou z vyšetření.
         */
        [HttpPost]
        public IActionResult FinishTicket()
        {
            // Získání informací o žádosti, pacientovi a zprávě z vyšetření.
            string patientNumber = Request.Form["patientNumber"];
            DateTime createDate = DateTime.Parse(Request.Form["ticketDate"]);
            string reportText = Request.Form["reportText"];

            // Ukončení žádosti a nahrání zprávy z vyšetření.
            CheckupTicket checkupTicket = db.CheckupTicketT.Where(o => o.Patient.SocialSecurityNum == patientNumber && o.CreateDate == createDate).ToList().First();
            checkupTicket.FinishDate = DateTime.Now;
            checkupTicket.State = "dokončeno";
            checkupTicket.Result = reportText;
            db.SaveChanges();

            // Přilinkování obrázků
            // Existujících
            if (Request.Form.Keys.Contains("FileExist"))
            {
                try
                {
                    foreach (string nameString in Request.Form["FileExist"])
                    {
                        int name = int.Parse(nameString);
                        Picture pic = db.PictureT.Where(o => o.NameInt == name).ToList().First();
                        db.PictureOnTicketsT.Add(new PictureOnTicket
                        {
                            Picture = pic,
                            Ticket = checkupTicket
                        });
                    }
                    db.SaveChanges();
                }
                catch
                {
                    // Catched
                }
            }

            // Nových
            int maxName = 0;
            if (db.PictureT.Any())
            {
                maxName = db.PictureT.Max(m => m.PictureId);
            }
            foreach (IFormFile image in Request.Form.Files)
            {
                UploadImage(image, webHostEnvironment, maxName);
                Picture pic = new Picture
                {
                    NameInt = maxName,
                    Description = image.FileName,
                    SocialSecurityNum = patientNumber,
                    CreateDate = TimeNowTruncateToSec(),
                    Type = image.FileName.Substring(image.FileName.LastIndexOf("."))
                };
                db.PictureT.Add(pic);
                db.SaveChanges();
                db.PictureOnTicketsT.Add(new PictureOnTicket
                {
                    Picture = pic,
                    Ticket = checkupTicket
                });
                db.SaveChanges();
                maxName++;
            }

            // Přesměrování zpět na kartu pacienta.
            return RedirectToAction("PatientProfile", new { patientNum = patientNumber });
        }

        /*
         * Akce zpbrazí odchozí (vytvořenou) žádost o vyšetření,
         * kterou má provést jiný lékař.
         * patientNum - rodné číslo pacienta
         * date - datum vytvoření žádosti
         */
        public IActionResult CheckupOut(string patientNum, DateTime date)
        {
            // Získání dané žádosti o vyšetření
            CheckupTicket checkupTicket = db.CheckupTicketT.Where(o => o.Patient.SocialSecurityNum == patientNum && o.CreateDate == date)
                                                           .Include(i => i.ToDoctor)
                                                           .Include(i => i.Patient).ToList().First();

            // Získání informací o pacientovi a doktorovi
            Data.User patient = db.UserT.Where(o => o.UserId == checkupTicket.Patient.UserId).ToList().First();
            Data.User doctorTo = db.UserT.Where(o => o.UserId == checkupTicket.ToDoctor.UserId).ToList().First();

            // Naplnění modelu pro zobrazení
            CheckupTicketModel model = new CheckupTicketModel
            {
                PatientFullName = new NameModel
                {
                    Name = patient.Name,
                    Surname = patient.Surname,
                    Title = patient.Title
                },
                SocialSecurityNumber = patientNum,
                DoctorFullName = new NameModel
                {
                    Name = doctorTo.Name,
                    Surname = doctorTo.Surname,
                    Title = doctorTo.Title
                },
                State = checkupTicket.State,
                CreateDate = date,
                FinishDate = checkupTicket.FinishDate,
                RequestText = checkupTicket.Description,
                ReportText = checkupTicket.Result
            };
            // Získání obrázků
            model.Pictures = db.PictureOnTicketsT.Where(o => o.Ticket.Patient.SocialSecurityNum == patientNum &&
                                                               o.Ticket.CreateDate == model.CreateDate)
                                                 .Select(s => new PictureJsonModel 
                                                 {
                                                     id = s.Picture.NameInt,
                                                     name = s.Picture.Description,
                                                     date = s.Picture.CreateDate.ToString(),
                                                     type = s.Picture.Type
                                                 }).ToList();

            return View(model);
        }

        /*
         * Akce fobrazí formulář pro vytvoření nové žádost k vyšetření.
         * patientNum - rodné číslo pacienta pro kterého bude zpráva vytvořena
         */
        public IActionResult NewCheckup(string patientNum, List<DateTime> reports)
        {
            // Získání informací o pacientovi.
            int patientId = db.PatientT.Where(o => o.SocialSecurityNum == patientNum).Select(s => s.UserId).ToList().First();
            Data.User patient = db.UserT.Where(o => o.UserId == patientId).ToList().First();

            // Naplnění modelu
            NewCheckupModel model = new NewCheckupModel
            {
                FullName = new NameModel
                {

                    Name = patient.Name,
                    Surname = patient.Surname,
                    Title = patient.Title,
                },
                SocialSecurityNum = patientNum,
                Reports = reports
            };
            return View(model);
        }

        /*
         * Akce vytvoří novou žádost o vyšetření.
         */
        [HttpPost]
        public IActionResult CreateNewCheckup()
        {
            // Zíslání informací o pacientovi, cílovém lékaři, a textu zprávy
            string patientNumber = Request.Form["patientNum"];
            string checkupText = Request.Form["checkupText"];
            int doctorICZ = int.Parse(Request.Form["doctorICZ"]);
            List<int> diagnosisList = new List<int>();
            diagnosisList.Add(int.Parse(Request.Form["Diagnosis1"]));
            if (!String.IsNullOrEmpty(Request.Form["Diagnosis2"]))
            {
                diagnosisList.Add(int.Parse(Request.Form["Diagnosis2"]));
            }
            if (!String.IsNullOrEmpty(Request.Form["Diagnosis3"]))
            {
                diagnosisList.Add(int.Parse(Request.Form["Diagnosis3"]));
            }
            if (!String.IsNullOrEmpty(Request.Form["Diagnosis4"]))
            {
                diagnosisList.Add(int.Parse(Request.Form["Diagnosis4"]));
            }

            // Zíkání informací o doktorovi (Autorovi, právě přihášeném), který žádost napsal.
            // HACK - pokud nebude přihášený uživatl mít záznam v tabulce UserT, dojde k erroru.
            var doctorId = db.UserT.Where(s => s.Login == User.Identity.Name).Select(o => o.UserId).ToList().First();

            // Pokud se jedná o admina, pak bude autorem žádosti a příjemcem výsledku on.
            // Pro admina se musí zkontrolovat, jestli má vytvořený pracovní doktorský účet.
            Data.Doctor doctorAuthor;
            if (User.IsInRole("Admin"))
            {
                var tmp = db.DoctorT.Where(o => o.UserId == doctorId).ToList();
                if (!tmp.Any())
                {
                    // Vytvoření pracovního doktorského profilu pro administrátora
                    var tmpTel = db.AdminT.Where(o => o.UserId == doctorId).Select(s => s.WorkPhone).ToList().First();
                    db.DoctorT.Add(new Data.Doctor
                    {
                        UserId = doctorId,
                        WorkPhone = tmpTel
                    });
                    db.SaveChanges();

                    doctorAuthor = db.DoctorT.Where(o => o.UserId == doctorId).ToList().First();
                }
                else
                {
                    // Pracovní profil již existuje.
                    doctorAuthor = tmp.First();
                }
            }
            else
            {
                doctorAuthor = db.DoctorT.Where(o => o.UserId == doctorId).ToList().First();
            }

            // Vytvoření žádosti.
            // Je potřeba mít jako samostatnou proměnnou, kvůli pozdějšímu užití.
            DateTime time = DateTime.Now;
            CheckupTicket ticket = new CheckupTicket
            {
                CreatedBy = doctorAuthor,
                // Cílový lékař se vyhledává podle ICZ
                ToDoctor = db.DoctorT.Where(o => o.ICZ == doctorICZ).ToList().First(),
                Patient = db.PatientT.Where(o => o.SocialSecurityNum == patientNumber).ToList().First(),
                Description = checkupText,
                State = "žádost vytvořena",
                CreateDate = new DateTime(time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second)
            };
            db.CheckupTicketT.Add(ticket);
            db.SaveChanges();

            // Pro kažnou diagnózu přiřazenou k žádosti o vyšetření,
            // je třeba vložit údaje do propojovací tabulky diagnozy a žádosti.
            foreach(int diag in diagnosisList)
            {
                // Vytvoření propojení
                db.TicketPerDiagnosisT.Add(new TicketPerDiagnosis
                {
                    Diagnosis = db.DiagnosisT.Where(o => o.DiagnosisId == diag).ToList().First(),
                    CheckupTicket = ticket
                });
                //db.SaveChanges();
            }
            // Původně byl SaveChanges na ve foru, kdyby nefungovalo, vrátit.
            db.SaveChanges();

            // Přilinkování obrázků
            // Existujících
            if (Request.Form.Keys.Contains("FileExist"))
            {
                try
                {
                    foreach (string nameString in Request.Form["FileExist"])
                    {
                        int name = int.Parse(nameString);
                        Picture pic = db.PictureT.Where(o => o.NameInt == name).ToList().First();
                        db.PictureOnTicketsT.Add(new PictureOnTicket
                        {
                            Picture = pic,
                            Ticket = ticket
                        });
                    }
                    db.SaveChanges();
                }
                catch
                {
                    // catched
                }
            }

            // Nových
            int maxName = 0;
            if (db.PictureT.Any())
            {
                maxName = db.PictureT.Max(m => m.PictureId);
            }
            foreach(IFormFile image in Request.Form.Files)
            {
                UploadImage(image, webHostEnvironment, maxName);
                Picture pic = new Picture
                {
                    NameInt = maxName,
                    Description = image.FileName,
                    SocialSecurityNum = patientNumber,
                    CreateDate = TimeNowTruncateToSec(),
                    Type = image.FileName.Substring(image.FileName.LastIndexOf("."))
                };
                db.PictureT.Add(pic);
                db.SaveChanges();
                db.PictureOnTicketsT.Add(new PictureOnTicket
                {
                    Picture = pic,
                    Ticket = ticket
                });
                db.SaveChanges();
                maxName++;
            }

            // Přesměrování na kartu pacietna.
            return RedirectToAction("PatientProfile", new { patientNum = patientNumber });
        }

        /*
         * Statická zunkce vracející datum zaokrouhlené na delé sekundy.
         */
        static DateTime TimeNowTruncateToSec()
        {
            DateTime now = DateTime.Now;
            DateTime ret = new DateTime(now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second);

            return ret;
        }

        /*
         * Statická funkce nahraje obrázek do systému.
         * image - obrázek k nahrátí
         * webHostEnv - systemové proměnné serveru
         * newNam - jméno nového souboru, ubáváno v celých číslech.
         */
        static private void UploadImage(IFormFile image, IWebHostEnvironment webHostEnv, int newName)
        {
            string uploadsFolder = Path.Combine(webHostEnv.WebRootPath, "images");
            string type = image.FileName.Substring(image.FileName.LastIndexOf("."));
            string filePath = Path.Combine(uploadsFolder, newName.ToString() + type);
            using (var fileStream = new FileStream(filePath, FileMode.Create))
            {
                image.CopyTo(fileStream);
            }
        }

        /*
         * Metoda akce pro vobrazení výkonů pro pojišťovny a stav jejich schválení.
         */
        public IActionResult Activity(string searchString, string SortOrder, ActivityModel model, int ? p)
        {

            //uložení aktuálně vyhledaného řetězce
            ViewData["Search"] = searchString;
            ViewData["CurrentSort"] = SortOrder;

            // Získání informaci o lékaři, který si pacienta zobrazuje.
            // HACK - pokud by uživatel nebyl v UserT pak dojde k chybě.
            string user = User.Identity.Name;
            var doctorId = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList().First();

            if (!String.IsNullOrEmpty(searchString))
            {
                model.medicallBills = db.MedicallBillT.Include(s => s.Diagnosis).Include(s => s.Doctor).Where(s => s.Diagnosis.Name.Contains(searchString) && s.Doctor.UserId == doctorId).ToList();
            }
            else
            {
                model.medicallBills = db.MedicallBillT.Include(s => s.Diagnosis).Include(s => s.Doctor).Where(o => o.Doctor.UserId == doctorId).OrderByDescending(s => s.CreateDate).ToList();
            }

            switch (SortOrder)
            {
                case "nazev":
                    model.medicallBills = model.medicallBills.OrderBy(o => o.Diagnosis.Name).ToList();
                    break;
                case "stav":
                    model.medicallBills = model.medicallBills.OrderBy(o => o.State).ToList();
                    break;
                default:
                    model.medicallBills = model.medicallBills.OrderByDescending(o => o.CreateDate).ToList();
                    break;
            }

            //stránkování vybraných dat
            model.PageNum = (p ?? 1);
            int pageSize = 8;
            IPagedList<MedicallBill> lide = model.medicallBills.ToPagedList(model.PageNum, pageSize);
            model.medicallBillsPage = lide;



            return View(model);
        }

        /*
         * Akce pro zobrazení rozcestníku d jednotlivým příchozím a odchozím žádostem k vyšetření.
         */
        public IActionResult Requests()
        {
            return View();
        }

        /*
         * Akce pro zobrazení příchozích žádostí o vyšetření.
         * sortOrder - filtrovací klíč
         * searchString - vyhledávaný text
         */
        public IActionResult InRequest(string sortOrder, string searchString)
        {
            ViewData["Search"] = searchString;
            ViewData["SortOrder"] = sortOrder;
            var db = new DatabaseContext();

            List<CheckupTicketToMe> MyInRequests = new List<CheckupTicketToMe>();

            // Získání informaci o lékaři, který si pacienta zobrazuje.
            // HACK - pokud by uživatel nebyl v UserT pak dojde k chybě.
            string user = User.Identity.Name;
            var doctorId = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList().First();


            if (String.IsNullOrEmpty(searchString))
            {
                MyInRequests = db.CheckupTicketT.Where(o => (o.State != "dokončeno") && (o.ToDoctor.UserId == doctorId || User.IsInRole("Admin")))
                                            .Select(s => new CheckupTicketToMe
                                            {
                                                PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                                                PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                                                SocialSecurityNumber = s.Patient.SocialSecurityNum,
                                                FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.getFullName()).First(),
                                                CreationDate = s.CreateDate
                                            }).ToList(); ;
            }
            else
            {
                Int64 parseRes;
                if (Int64.TryParse(searchString, out parseRes))
                {
                    MyInRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno" && (o.ToDoctor.UserId == doctorId || User.IsInRole("Admin"))).Select(s => new CheckupTicketToMe
                    {
                    PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                    PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                    SocialSecurityNumber = s.Patient.SocialSecurityNum,
                    FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.getFullName()).First(),
                    CreationDate = s.CreateDate
                    }).Where(s => s.SocialSecurityNumber == searchString).ToList();
                }
                else
                {
                    MyInRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno" && (o.ToDoctor.UserId == doctorId || User.IsInRole("Admin"))).Select(s => new CheckupTicketToMe
                    {
                        PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                        PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                        SocialSecurityNumber = s.Patient.SocialSecurityNum,
                        FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId)
                        .Select(p => (String.IsNullOrEmpty(p.Title) ? "" : (p.Title + " ")) + p.Name + " " + p.Surname).First(),
                        CreationDate = s.CreateDate
                    }).Where(s => s.PatientName.Contains(searchString) || s.PatientSurname.Contains(searchString) || s.FromDoctor.Contains(searchString)).ToList();
                }
            }

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
                    MyInRequests = MyInRequests.OrderByDescending(o => o.CreationDate).ToList();
                    break;
            }

            return View(MyInRequests);
        }

        /*
         * Akce zobrazující odchozí žádosti o vyšetření.
         * sortOrder - filtrovací klíč
         * searchString - vyhledávaný text
         */
        public IActionResult OutRequest(string sortOrder, string searchString)
        {
            var db = new DatabaseContext();
            ViewData["Search"] = searchString;
            ViewData["SortOrder"] = sortOrder;
            List<CheckupTicketToOtherModel> MyOutRequests = new List<CheckupTicketToOtherModel>();

            // Získání informaci o lékaři, který si pacienta zobrazuje.
            // HACK - pokud by uživatel nebyl v UserT pak dojde k chybě.
            string user = User.Identity.Name;
            var doctorId = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList().First();

            if (String.IsNullOrEmpty(searchString))
            {
                MyOutRequests = db.CheckupTicketT.Where(o => (o.CreatedBy.UserId == doctorId || User.IsInRole("Admin")))
                .Select(s => new CheckupTicketToOtherModel
                {
                    PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                    PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                    SocialSecurityNumber = s.Patient.SocialSecurityNum,
                    ToDoctor = db.UserT.Where(o => o.UserId == s.ToDoctor.UserId).Select(p => p.getFullName()).First(),
                    CreationDate = s.CreateDate,
                    State = s.State == "dokončeno"
                }).ToList().OrderByDescending(p =>p.CreationDate).ToList();
                MyOutRequests.Sort((x, y) => (y.State == x.State) ? DateTime.Compare(y.CreationDate, x.CreationDate) : (x.State ? 1 : -1));
            }
            else
            {
                Int64 parseRes;
                if (Int64.TryParse(searchString, out parseRes))
                {
                    MyOutRequests = db.CheckupTicketT.Where(o => (o.CreatedBy.UserId == doctorId || User.IsInRole("Admin")))
                    .Select(s => new CheckupTicketToOtherModel
                    {
                        PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                        PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                        SocialSecurityNumber = s.Patient.SocialSecurityNum,
                        ToDoctor = db.UserT.Where(o => o.UserId == s.ToDoctor.UserId).Select(p => p.getFullName()).First(),
                        CreationDate = s.CreateDate,
                        State = s.State == "dokončeno"
                    }).Where(s => s.SocialSecurityNumber == searchString).ToList();
                    MyOutRequests.Sort((x, y) => (y.State == x.State) ? DateTime.Compare(y.CreationDate, x.CreationDate) : (x.State ? 1 : -1));
                }
                else
                {
                    MyOutRequests = db.CheckupTicketT.Where(o => (o.CreatedBy.UserId == doctorId || User.IsInRole("Admin")))
                    .Select(s => new CheckupTicketToOtherModel
                    {
                        PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                        PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                        SocialSecurityNumber = s.Patient.SocialSecurityNum,
                        ToDoctor = db.UserT.Where(o => o.UserId == s.ToDoctor.UserId).Select(p => (String.IsNullOrEmpty(p.Title) ? "" : (p.Title + " ")) + p.Name + " " + p.Surname).First(),
                        CreationDate = s.CreateDate,
                        State = s.State == "dokončeno"
                    }).Where(s => s.PatientName.Contains(searchString) || s.PatientSurname.Contains(searchString) || s.ToDoctor.Contains(searchString)).ToList();
                    MyOutRequests.Sort((x, y) => (y.State == x.State) ? DateTime.Compare(y.CreationDate, x.CreationDate) : (x.State ? 1 : -1));
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
                    MyOutRequests = MyOutRequests.OrderByDescending(o => o.CreationDate).ToList();
                    break;
            }

            return View(MyOutRequests);
        }

        /*
         * Akce zobrazí příchozí vyřešení žádosti.
         */
        public IActionResult InFinishRequest(string sortOrder, string searchString)
        {
            ViewData["Search"] = searchString;
            ViewData["SortOrder"] = sortOrder;

            var db = new DatabaseContext();

            List<CheckupTicketToMeFinish> MyFinishRequests = new List<CheckupTicketToMeFinish>();

            // Získání informaci o lékaři, který si pacienta zobrazuje.
            // HACK - pokud by uživatel nebyl v UserT pak dojde k chybě.
            string user = User.Identity.Name;
            var doctorId = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList().First();


            if (String.IsNullOrEmpty(searchString))
            {
                MyFinishRequests = db.CheckupTicketT.Where(o => o.State == "dokončeno" && (o.ToDoctor.UserId == doctorId || User.IsInRole("Admin")))
                .Select(s => new CheckupTicketToMeFinish
                {
                    PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                    PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                    SocialSecurityNumber = s.Patient.SocialSecurityNum,
                    FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.getFullName()).First(),
                    CreationDate = s.CreateDate,
                    FinishDate = s.FinishDate.ToString("dd.MM.yyyy")
                }).ToList().OrderByDescending(o => o.CreationDate).ToList();
            }
            else
            {
                {
                    Int64 parseRes;
                    if (Int64.TryParse(searchString, out parseRes))
                    {
                        MyFinishRequests = db.CheckupTicketT.Where(o => o.State == "dokončeno" && (o.ToDoctor.UserId == doctorId || User.IsInRole("Admin")))
                        .Select(s => new CheckupTicketToMeFinish
                        {
                            PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                            PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                            SocialSecurityNumber = s.Patient.SocialSecurityNum,
                            FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId).Select(p => p.getFullName()).First(),
                            CreationDate = s.CreateDate,
                            FinishDate = s.FinishDate.ToString("dd.MM.yyyy")
                        }
                        ).Where(s => s.SocialSecurityNumber == searchString).ToList();
                    }
                    else
                    {
                        MyFinishRequests = db.CheckupTicketT.Where(o => o.State == "dokončeno" && (o.ToDoctor.UserId == doctorId || User.IsInRole("Admin")))
                        .Select(s => new CheckupTicketToMeFinish
                        {
                            PatientSurname = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Surname).First(),
                            PatientName = db.UserT.Where(o => o.UserId == s.Patient.UserId).Select(p => p.Name).First(),
                            SocialSecurityNumber = s.Patient.SocialSecurityNum,
                            FromDoctor = db.UserT.Where(o => o.UserId == s.CreatedBy.UserId)
                            .Select(p => (String.IsNullOrEmpty(p.Title) ? "" : (p.Title + " ")) + p.Name + " " + p.Surname).First(),
                            CreationDate = s.CreateDate,
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
                    MyFinishRequests = MyFinishRequests.OrderByDescending(o => o.CreationDate).ToList();
                    break;
            }
            return View(MyFinishRequests);
        }

        /*
         * Akce pro změnu vlastníka (lékaře) zprávy.
         */
        [HttpPost]
        public IActionResult ChangeOwner()
        {
            string[] changeState = Request.Form["Checkbox[]"];
            int doctorToICZ = int.Parse(Request.Form["doctorICZ"]);
            string patientNumber = Request.Form["patientNumber"];
            var doctorId = db.UserT.Where(s => s.Login == User.Identity.Name).Select(o => o.UserId).ToList().First();

            var reports = db.MedicallReportT.Where(o => o.Patient.SocialSecurityNum == patientNumber && (User.IsInRole("Admin") || o.Owner.UserId == doctorId))
                                            .Include(o => o.Owner).ToList().OrderByDescending(o => o.CreateDate).ToList();

            for(int i = 0; i < changeState.Length; i++)
            {
                if(changeState[i] == "on")
                {
                    reports.ElementAt(i).Owner = db.DoctorT.Where(o => o.ICZ == doctorToICZ).ToList().First();       
                }    
            }
            db.SaveChanges();

            return RedirectToAction("PatientProfile", new { patientNum = patientNumber });
        }

        /*
         * Json dotaz vrátí všechny diagnozy při autocompletu.
         */
        public JsonResult GetAllDiagnosis(string search)
        {
            List<Diagnosis> matchedDiagnosis;
            if (search == null || search == "?")
            {
                matchedDiagnosis = db.DiagnosisT.ToList();
            }
            else
            {
                matchedDiagnosis = db.DiagnosisT.Where(o => o.Name.ToUpper().Contains(search.ToUpper())).ToList().OrderBy(o => o.Name).ToList();
            }

            return new JsonResult(matchedDiagnosis);
        }

        /*
         * Json dotaz vrátí všechny typy lékařských výkonů pro autocomplete.
         */
        public JsonResult GetAllActivities(string search)
        {
            List<ActivityJsonModel> matchedActivities;
            if (search == null || search == "?")
            {
                matchedActivities = db.MedicallActivityPriceT.Select(s => new ActivityJsonModel
                                                                                     {
                                                                                         Id = s.MedicallActivityPriceId,
                                                                                         Name = s.Name
                                                                                     }).ToList().OrderBy(o => o.Name).ToList();
            }
            else
            {
                matchedActivities = db.MedicallActivityPriceT.Where(o => o.Name.ToUpper().Contains(search.ToUpper()))
                                                                                     .Select(s => new ActivityJsonModel
                                                                                     {
                                                                                         Id = s.MedicallActivityPriceId,
                                                                                         Name = s.Name
                                                                                     }).ToList().OrderBy(o => o.Name).ToList();
            }
            return new JsonResult(matchedActivities);
        }

        /*
         * Json dotaz získá všechny doktory pro autocomplete.
         */
        public JsonResult GetAllDoctors(string search)
        {
            List<DoctorJsonModel> matchedDoctors;
            if (search == null || search == "?")
            {
                matchedDoctors = db.DoctorT.Join(db.UserT,
                                                doctor => doctor.UserId,
                                                user => user.UserId,
                                                (doctor, user) => new DoctorJsonModel
                                                {
                                                    Id = doctor.ICZ,
                                                    Name = user.getFullName()
                                                }).ToList().OrderBy(o => o.Name).ToList();
            }
            else
            {
                matchedDoctors = db.DoctorT.Join(db.UserT.Where(o => o.Name.ToUpper().StartsWith(search.ToUpper()) || o.Surname.ToUpper().StartsWith(search.ToUpper())),
                                                doctor => doctor.UserId,
                                                user => user.UserId,
                                                (doctor, user) => new DoctorJsonModel
                                                {
                                                    Id = doctor.ICZ,
                                                    Name = user.getFullName()
                                                }).ToList().OrderBy(o => o.Name).ToList();
            }
            return new JsonResult(matchedDoctors);
        }

        /*
         * Statická funkce pro kontrolu hodnot zvolených diagnóz.
         */
        static public int TestDiagnosis(DatabaseContext db, string diag1, string diag2, string diag3, string diag4)
        {
            if (String.IsNullOrEmpty(diag1))
            {
                return 1;
            }

            int diagCnt = 1;
            int diag1Code = -1;
            int diag2Code = -1;
            int diag3Code = -1;
            int diag4Code = -1;

            if (!int.TryParse(diag1, out diag1Code))
            {
                return 1;
            }

            if (!String.IsNullOrEmpty(diag2))
            {
                if (!int.TryParse(diag2, out diag2Code))
                {
                    return 2;
                }
                diagCnt++;
            }

            if (!String.IsNullOrEmpty(diag3))
            {
                if (!int.TryParse(diag3, out diag3Code))
                {
                    return 3;
                }
                diagCnt++;
            }

            if (!String.IsNullOrEmpty(diag4))
            {
                if (!int.TryParse(diag4, out diag4Code))
                {
                    return 4;
                }
                diagCnt++;
            }


            List<int> listDiag = db.DiagnosisT.Select(s => s.DiagnosisId).Where(o => o == diag1Code ||
                                                            (diag2Code != -1 && o == diag2Code) ||
                                                            (diag3Code != -1 && o == diag3Code) ||
                                                            (diag4Code != -1 && o == diag4Code)).ToList();

            // Kontrola, zda všechny diagnozy existují
            if (listDiag.IndexOf(diag1Code) == -1)
            {
                return 1;
            }
            else if (diag2Code != -1 && listDiag.IndexOf(diag2Code) == -1)
            {
                return 2;
            }
            else if (diag3Code != -1 && listDiag.IndexOf(diag3Code) == -1)
            {
                return 3;
            }
            else if (diag4Code != -1 && listDiag.IndexOf(diag4Code) == -1)
            {
                return 4;
            }
            else if (diagCnt != listDiag.Count)
            {
                return 10;
            }

            return 0;
        }

        /*
         * Statická funkce pro kontrolu kódů lékařských výkonů.
         */
        static public int TestActivity(DatabaseContext db, string activity)
        {
            if (String.IsNullOrEmpty(activity))
            {
                return 5;
            }

            int activityCode = -1;

            if (!int.TryParse(activity, out activityCode))
            {
                return 5;
            }

            if (!db.MedicallActivityPriceT.Select(s => s.MedicallActivityPriceId).Where(o => o == activityCode).ToList().Any())
            {
                return 5;
            }
            
            return 0;
        }
    
        /*
         * Metoda zkontroluje zprávnost vutvářené zpvávy a výsledek vyhodnocení (bool) vrátí klientovi.
         */
        public JsonResult CheckNewReport(string diag1, string diag2, string diag3, string diag4, string activity)
        {
            int result = TestDiagnosis(db, diag1, diag2, diag3, diag4);

            if(result == 0)
            {
                result = TestActivity(db, activity);
            }

            return new JsonResult(result);
        }

        /*
         * Metoda zkontroluje zprávně zvoleného lékaře
         */
        static public int TestDoctor(DatabaseContext db, string doctor)
        {
            if (String.IsNullOrEmpty(doctor))
            {
                return 6;
            }

            int doctorICZ = -1;

            if (!int.TryParse(doctor, out doctorICZ))
            {
                return 6;
            }

            if (!db.DoctorT.Select(s => s.ICZ).Where(o => o == doctorICZ).ToList().Any())
            {
                return 6;
            }

            return 0;
        }

        /*
         * Metoda zkontroluje správně vytvořenou žádost na vyšetření.
         */
        public JsonResult CheckNewTicket(string diag1, string diag2, string diag3, string diag4, string doctor)
        {
            int result = TestDiagnosis(db, diag1, diag2, diag3, diag4);

            if(result == 0)
            {
                result = TestDoctor(db, doctor);
            }

            return new JsonResult(result);
        }

        /*
         * Metoda vrátí text požadované zprávy klientovi.
         */
        public JsonResult GetReport(string patientNum, string date)
        {
            DateTime mydate = DateTime.Parse(date);
            var test = db.MedicallReportT.Where(o => o.Patient.SocialSecurityNum == patientNum).ToList().First();
            string result = db.MedicallReportT.Where(o => o.Patient.SocialSecurityNum == patientNum && o.CreateDate == mydate).Select(s => s.Description).ToList().First();
            result = String.Format("##########################{0}##########################\n{1}\n######################################################################", date, result);
            return new JsonResult(result);
        }

        /*
         * Metoda vrátí text požadovaného výsledku vyšetření klientovi.
         */
        public JsonResult GetResult(string patientNum, string date)
        {

            // Získání informaci o lékaři, který si pacienta zobrazuje.
            // HACK - pokud by uživatel nebyl v UserT pak dojde k chybě.
            var doctor = db.UserT.Where(s => s.Login == User.Identity.Name).ToList().First();
            DateTime mydate = DateTime.Parse(date);
            string result = db.CheckupTicketT.Where(o => o.Patient.SocialSecurityNum == patientNum &&
                                                         o.FinishDate == mydate)
                                             .Select(s => s.Result).ToList().First();
            result = String.Format("##########################{0}##########################\nAutor: {1}\n\n{2}\n#####################################################################\n", date, doctor.getFullName() ,result);
            return new JsonResult(result);
        }

        /*
         * Metoda vrátí klientovi všechny obrázky příslušné k namému pacientovi.
         */
        public JsonResult GetPictures(string patientNum)
        {
            
            // Zíkání informací o doktorovi, který zprávu napsal.
            // HACK - pokud nebude přihášený uživatl mít zíznam v tabulce UserT, dojde k erroru.
            string userDoctor = User.Identity.Name;
            var doctorId = db.UserT.Where(s => s.Login == userDoctor).Select(o => o.UserId).ToList().First();

            // Získání obrázků z Žádostí (Ticketů)
            List<PictureJsonModel> fromTickets = db.PictureOnTicketsT
                                                   .Where(o => o.Ticket.Patient.SocialSecurityNum == patientNum &&
                                                               (o.Ticket.CreatedBy.UserId == doctorId ||
                                                                    o.Ticket.ToDoctor.UserId == doctorId ||
                                                                    User.IsInRole("Admin")))
                                                   .Select(s => new PictureJsonModel
                                                   {
                                                       id = s.Picture.NameInt,
                                                       name = s.Picture.Description,
                                                       date = s.Picture.CreateDate.ToShortDateString(),
                                                       type = s.Picture.Type
                                                   }).ToList();

            // Získání obrázků ze Zpráv
            List<PictureJsonModel> fromReports = db.PictureOnReportT
                                                   .Where(o => o.Report.Patient.SocialSecurityNum == patientNum &&
                                                               (o.Report.Owner.UserId == doctorId || User.IsInRole("Admin")))
                                                   .Select(s => new PictureJsonModel
                                                   {
                                                       id = s.Picture.NameInt,
                                                       name = s.Picture.Description,
                                                       date = s.Picture.CreateDate.ToShortDateString(),
                                                       type = s.Picture.Type
                                                   }).ToList();


            List<PictureJsonModel> result = fromTickets;
            foreach(PictureJsonModel picture in fromReports)
            {
                if (!result.Contains(picture))
                {
                    result.Add(picture);
                }
            }
            
            return new JsonResult(result);
        }

        /*
         * Metoda zkontroluje unikátnost rodného čísla.
         */
        public JsonResult TestSocialSecurityNumUnique(string num)
        {
            bool result = true;

            if(db.PatientT.Select(s => s.SocialSecurityNum).Where(o => o == num).Any())
            {
                result = false;
            }

            return new JsonResult(result);
        }

        /*
         * Metoda aktualizuje stav vyšetření o které požádal jeden lékař druhého.
         */
        public JsonResult UpdateState(string newState, string date, string patient)
        {
            db.CheckupTicketT.Where(o => o.CreateDate == DateTime.Parse(date) && o.Patient.SocialSecurityNum == patient).First().State = newState;
            db.SaveChanges();

            bool toret = true;

            return new JsonResult(toret);
        }
    }
}
