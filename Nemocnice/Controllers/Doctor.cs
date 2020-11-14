/*
 * Třída kontroleru uživatele s rolí Doctor.
 * Autor: Michal Šedý <xsedym02>
 * Poslední úprave: 12.11.2020
 */

/*
 * Testovací makro, když je nastaveno nejsou pacienti filtrování dle doktorů.
 * Všichni doktori vydí všechno.
 */
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
    /*
     * Třída kontroleru obslujujícího uživatele s oprávněním Doktor.
     * K jednotlivým akcím a následně zobrazením může přistoupit pouze oprávněný uživatel.
     */
    [Authorize(Roles = "Doctor")]
    public class Doctor : Controller
    {
        /*
         * Informace o právě přihlášeném uživateli.
         */
        private readonly UserManager<NemocniceUser> _userManager;

        /*
         * Proměnná umožňující napříč kontrollerem přistupovat k databázi
         */
        private DatabaseContext db { get; set; } = new DatabaseContext();

        public Doctor(UserManager<NemocniceUser> userManager)
        {
            _userManager = userManager;
        }


        public IActionResult Index()
        {
            return View();
        }


        /*
         * Akce karotéky. Vypíše všechny pacienty uložené v databázi.
         * sortOrder - typ řazení (podle jnéma, příjmení, rodného čísla)
         * searchString - hledaný řetězec (v případě vyhledávání)
         */
        public IActionResult Card(string sortOrder, string searchString)
        {
            // Uložení přávě vyhledávaného řetězce.
            // Při řazení výsledků budeme už vědět, o jaké výsledky se jedná.
            ViewData["Search"] = searchString;

            // Seznam všech pacientů v databázi
            List<CardModel> Patients;

            
            // Získání údajů ke každému pacientovi (Příjmení, Jméno, Titul, R.Č., pojišťovny).
            // Informace jsou získávány ze spojení dvou tabulek: PatientT (R.Č., pojišťovna) a UserT (Příjmení, Jméno, Titul).
            if (String.IsNullOrEmpty(searchString))
            {
                // Není použito vyhledávání (chceme všechny pacienty)
                Patients =
                    db.PatientT.Join(
                        db.UserT,
                        patient => patient.UserId,
                        user => user.UserId,
                        (patient, user) => new CardModel
                        {
                            Surname = user.Surname,
                            Name = user.Name,
                            Title = user.Title,
                            SocialSecurityNum = patient.SocialSecurityNum,
                            Insurance = patient.InsuranceCompany
                        }
                        ).ToList();
            }
            else
            {
                // Je potřeba vyhledat konkrétní pacienty odpovídající hledanému výrazu.
                // Hledání probíhá skrz položky (Jméno, Příjmení, R.Č.).
                // Rodné číslo je převáděno na string. Hledání probíhá na základě StartsWith.
                Patients =
                db.PatientT.Join(
                    db.UserT,
                    patient => patient.UserId,
                    user => user.UserId,
                    (patient, user) => new CardModel
                    {
                        Surname = user.Surname,
                        Name = user.Name,
                        Title = user.Title,
                        SocialSecurityNum = patient.SocialSecurityNum,
                        Insurance = patient.InsuranceCompany
                    }
                    ).Where(s => s.SocialSecurityNum.ToString().StartsWith(searchString) ||
                                    s.Name.StartsWith(searchString) ||
                                    s.Surname.StartsWith(searchString)).ToList();
            }

            // Řazení dle jednotlivých krytérií nastaveních v sortOrder.
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
            
            // TODO mohu poslat samostatný model? není tam něco navíc?
            return View(Patients);
        }


        /*
         * Metoda pro vytvoření nového pacienta v databázi.
         * Zahrnuje vytvoření Adresy, Uživatele, Pacienta, Uživatele pro Identity FW a přidělení práv.
         */
        [HttpPost]
        public async Task<IActionResult> CreatePatientAsync()
        {
            // Hodnoty pro nového pacienta získané přes POST
            string name = Request.Form["NewName"];
            string surname = Request.Form["NewSurname"];
            string title = Request.Form["NewTitle"];
            long socialNum = long.Parse(Request.Form["NewNum"]);
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

            // Kontrola, zda pacient již neexistuje, pokud ano, nic neděláme.
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
            user = new User
            {
                Login = socialNum.ToString(),
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
                HealthCondition = null
            });
            db.SaveChanges();

            // Vytvoření nového uřivatele pro Identity Framework.
            // Základní heslo je 1234567890, uživatelské jméno je shodné s rodným číslem.
            // Udělení oprávnění Patient.
            var userIdentity = new NemocniceUser { UserName = user.Login };
            var result = await _userManager.CreateAsync(userIdentity, "1234567890");
            await _userManager.AddToRoleAsync(userIdentity, "Patient");

            // Návrat zpět do kartotéky.
            return RedirectToAction("Card");

        }


        /*
         * Metoda pro zobrazení zpráv (ne žádanek a jejich výsledků) pojících se k pacientovi.
         * V základu slouží k vytvoření nové zprávy.
         * Umožňuje zobrazit a upravoval libovolné stare zprávy.
         * Netoda zpřístupní poze ty zápisy, jejichž správcem je přihlášený doktor.
         * (Během testování (TEST) jsou přístupné všechny záznamy všem.
         * TODO - Odstranit TEST
         * reportModel - model obsahující informace pro zobrazení zprávy pacienta
         * socialNum - rodné číslo zobrazovaného pacienta
         * date - datum zobrazovaného zápisu (pro nový dápis se rovná default)
         */
        public IActionResult Report(ReportModel reportModel, long socialNum, DateTime date)
        {
            // Získání loginu přihlášeného doktora pro kontrolu přístupu ke zprávám.
            // v id_list by vždy něco mělo být (do této metody se nemůže dostat nikdo, kdo není Doktorem,
            // Při použití TEST k tomu může dojít. (V databázi Identity je uživatel, který není v UserT).
            string user = User.Identity.Name;
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();

            // Kontrola modelu
            if (!ModelState.IsValid)
            {
                return View(reportModel);
            }

            // Nutná kontrola při nekonzistentní databázi (V databázi Identity je uživatel, který není v UserT).
            // Němělo by k tomu dojít, ale je to celkem vhodné ponechat (Kdo ví, co se může pokazit).
#if (!TEST)
            if (id_list.Any())
            {
                // Získání Všech zpráv napsaných přihášeným doktorem pro pacienta s rodným číslem socialNum
                var patiensDoctorReport = db.MedicallReportT.Where(o => (o.Patient.SocialSecurityNum == socialNum) && o.Owner.UserId == id_list.First());
#else
            var patiensDoctorReport = db.MedicallReportT.Where(o => o.Patient.SocialSecurityNum == socialNum);
#endif
            // Pokud je datum default, pak se vytváří nová zpráva, jinak se zobrazuje stará.
            reportModel.ActualReportDate = (date == default) ? DateTime.Now : date;
            // Soubor všech zpráv, pouze data. (to stačí)
            reportModel.AllReports = patiensDoctorReport.Select(s => s.CreateDate).ToList();

            // Pokud má pasient již nějakou starší zprávu, vybereme tu o jednu jednotku starší a zobrazíme.
            if (reportModel.AllReports.Any())
            {
                // Seřazení časů všech pacientových zpráv.
                reportModel.AllReports = reportModel.AllReports.OrderByDescending(x => x).ToList();

                // Kontrola, zda je vytvářená nová zpráva.
                if (date == default)
                {
                    // Pokud je vytvářená nová zpráva zpbrazíme také poslední zprávu.
                    reportModel.PreviousReportDate = reportModel.AllReports.First();
                    reportModel.PreviousReport = patiensDoctorReport.Where(o => o.CreateDate == reportModel.PreviousReportDate).Select(s => s.Description).ToList().First();
                }
                else
                {
                    // Zobrazujeme starší zprávu. Jako aktuální (k případné úpravě) bude hledaná zpráva.
                    reportModel.ActualReport = patiensDoctorReport.Where(o => o.CreateDate == date).Select(s => s.Description).ToList().First();
                    // Získání zprávy o jednu jednotku starší, pokud taková existuje.
                    var oldCreateDateIdx = reportModel.AllReports.FindIndex(o => o == date) + 1;
                    if (oldCreateDateIdx < reportModel.AllReports.Count)
                    {
                        reportModel.PreviousReportDate = reportModel.AllReports.ElementAt(oldCreateDateIdx);
                        reportModel.PreviousReport = patiensDoctorReport.Where(o => o.CreateDate == reportModel.PreviousReportDate).Select(s => s.Description).ToList().First();
                    }
                    else
                    {
                        reportModel.PreviousReportDate = default;
                        reportModel.PreviousReport = "";
                    }
                }
            }
            else
            {
                // Píšeme novou zprávu a pacient nemá žádnou předchozí.
                reportModel.PreviousReportDate = default;
                reportModel.PreviousReport = "";
            }

            // Získání informací o pacientovi.
            var userId = db.PatientT.Where(o => o.SocialSecurityNum == socialNum).Select(s => s.UserId).ToList().First();
            var patient = db.UserT.Where(o => o.UserId == userId);
            // Uložení informací o pacientovi do modelu.
            reportModel.Name = patient.Select(s => s.Name).ToList().First();
            reportModel.Surname = patient.Select(s => s.Surname).ToList().First();
            reportModel.Title = patient.Select(s => s.Title).ToList().First();
            reportModel.Insurance = db.PatientT.Where(o => o.SocialSecurityNum == socialNum).Select(s => s.InsuranceCompany).ToList().First();
            reportModel.SocialSecurityNumber = socialNum;
#if (!TEST)
        }
#endif
            return View(reportModel);
        }


        /*
         * Metoda obsatrávající vytvoření nové zprávy. S tím související aktualizace/vytvoření
         * zdravotního stavu, vytvoření výkazu pro pojišťovnu a aktualizace stavu pacienta.
         */
        [HttpPost]
        public IActionResult CreateReport()
        {
            // Získání potřebných hodnot o pacientovi, zprávě, diagnozach a výkonu pomocí POST.
            long patientNum = long.Parse(Request.Form["PatientNum"].ToString());
            string reportText = Request.Form["ReportText"];
            string billString = Request.Form["Bill"];
            int idxStart = billString.IndexOf('(') + 1;
            int stringLen = billString.IndexOf(')') - idxStart;
            int billCode = int.Parse(billString.Substring(idxStart, stringLen));
            DateTime reportDate = DateTime.Parse(Request.Form["ReportDate"]);

            List<int> diagnosis = new List<int>();
            foreach(string str in Request.Form["Diagnosis[]"])
            {
                idxStart = str.IndexOf('(') + 1;
                stringLen = str.IndexOf(')') - idxStart;
                diagnosis.Add(int.Parse(str.Substring(idxStart, stringLen)));
            }
            List<string> diagnosisState = new List<string>(Request.Form["DiagnosisState[]"]);

            // Zíkání informací o lékařovi, který zprávu napsal.
            // Pokud nebude lékař v tabulce DoctorT, pakbude id_list prázdný.
            // K tomu může dojít pouze při nekonzistentní databázi, nebo definici makra TEST.
            string user = User.Identity.Name;
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();

            // Pokud je nastaven TEST, pak bude jako doktor lokálně určen Doctor {UserId = 987654321}
            Data.Doctor doctor;
#if (!TEST)
            doctor = db.DoctorT.Where(o => o.UserId == id_list.First()).ToList().First();
#else
            // Pokud testový Doctor s UserId ještě neexistuje, vytvoříme jej.
            if (!db.DoctorT.Where(o => o.UserId == 987654321).ToList().Any())
            {
                doctor = new Data.Doctor
                {
                    UserId = 987654321,
                    WorkPhone = "777777777"
                };
                db.DoctorT.Add(doctor);
                db.SaveChanges();
            }
            else
            {
                doctor = db.DoctorT.Where(o => o.UserId == 987654321).ToList().First();
            }
#endif

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

            // Registrace výkonu
            // Kód výkonu musí v databázi výkonú existovat, jinak ERROR.
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
                List<PatientTreatmentLog> openDiagnosis =
                    oldTreatementLog.Any() ? oldTreatementLog.Where(o => o.Diagnosis.DiagnosisId == diagnosis.ElementAt(i) && o.EndOfTreatment == default).ToList() : new List<PatientTreatmentLog>();
                if (diagnosisState.ElementAt(i) == "cured")
                {
                    if(openDiagnosis.Any())
                    {
                        // Ukončení existujícího léčení
                        openDiagnosis.First().EndOfTreatment = reportDate;
                    }
                    else
                    {
                        // Vytvoření a okamžité ukončení
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
                    // Pokud ne, pak se vytvoří nová poznámka, v opačném případě se jedná pouze o kontrolu.
                    if(!openDiagnosis.Any())
                    {
                        // Vytvoření nové diagnozy pacienta
                        db.PatientTreatmentLogT.Add(new PatientTreatmentLog
                        {
                            Patient = db.PatientT.Where(o => o.SocialSecurityNum == patientNum).ToList().First(),
                            Diagnosis = db.DiagnosisT.Where(o => o.DiagnosisId == diagnosis.ElementAt(i)).ToList().First(),
                            StartOfTreatment = reportDate
                        });
                        db.SaveChanges();
                    }
                }
                
                // Určení CureProgress pro každou diagnozu vráci zprávy. (Zahájení léčby, kontrola, konec)
                db.CureProgressT.Add(new CureProgress
                {
                    MedicallReport = report,
                    Diagnosis = db.DiagnosisT.Where(o => o.DiagnosisId == diagnosis.ElementAt(i)).ToList().First(),
                    StateOfTreatment = (diagnosisState.ElementAt(i) == "cured") ? (openDiagnosis.Any() ? "Ukončení léčby" : "Jednorázová léčba")
                                                                            : (openDiagnosis.Any() ? "Kontrola" : "Zahájení léčby")
                });
                db.SaveChanges();
            }

            // Aktualizace Healthcondition pacienta, konkrétně datum jeho poslední návštěvy.
            // Pokud nemá pacient vytvořen HealthCondition, vytvoříme jej.       
            Data.Patient p = db.PatientT.Where(o => o.SocialSecurityNum == patientNum).ToList().First();
            if (p.HealthCondition == null)
            {
                p.HealthCondition = new HealthCondition
                {
                    SocialSecurityNum = patientNum
                };
            }
            p.HealthCondition.LastCheckupDate = reportDate;
            db.SaveChanges();

            return RedirectToAction("Card");
        }

        /*
         * Metoda upraví text staréhé zprávy.
         * Není zde prováděná kontrola na lékaře.
         * Předpokládá se, že do systému se neuloží ve stejný čas pro pacienta dvě stejné zprávy.
         */
        [HttpPost]
        public IActionResult UpdateReport()
        {
            // Získání dat o konkrétním pacientovi a datu zprávy z POST
            DateTime date = DateTime.Parse(Request.Form["ReportDate"]);
            long patientNum = long.Parse(Request.Form["PatientNum"]);
            string reportText = Request.Form["ReportText"];

            // Aktualizace textu.
            db.MedicallReportT.Where(o => o.Patient.SocialSecurityNum == patientNum && o.CreateDate == date)
                .ToList().First().Description = reportText;
            db.SaveChanges();

            return RedirectToAction("Card");
        }

        public IActionResult Activity(string userName, string SortOrder)
        {
            var db = new DatabaseContext();

            string user = User.Identity.Name;
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

            switch (SortOrder)
            {
                case "nazev":
                    Activities = Activities.OrderBy(o => o.ActivityName).ToList();
                    break;
                case "stav":
                    Activities = Activities.OrderBy(o => o.State).ToList();
                    break;
                default:
                    Activities = Activities.OrderByDescending(o => o.CreateDate).ToList();
                    break;
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


#if (!TEST)
            if(id_list.Any())
            {
                MyInRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno").Where(o => o.ToDoctor.UserId == id_list.First())
#else
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
                ).ToList().OrderByDescending(o => DateTime.Parse(o.CreationDate)).ToList();
#if (!TEST)
            }
#endif


            return View(MyInRequests);
        }

        public IActionResult OutRequest()
        {
            var db = new DatabaseContext();

            List<CheckupTicketToOtherModel> MyOutRequests = new List<CheckupTicketToOtherModel>();

            string user = User.FindFirstValue(ClaimTypes.Name);
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();

#if (!TEST)
            if (id_list.Any())
            {
                MyOutRequests = db.CheckupTicketT.Where(o => o.CreatedBy.UserId == id_list.First())
#else
                MyOutRequests = db.CheckupTicketT
#endif
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
#if (!TEST)
            }
#endif
            return View(MyOutRequests);
        }

        public IActionResult InFinishRequest()
        {
            var db = new DatabaseContext();

            List<CheckupTicketToMeFinish> MyFinishRequests = new List<CheckupTicketToMeFinish>();

            string user = User.FindFirstValue(ClaimTypes.Name);
            var id_list = db.UserT.Where(s => s.Login == user).Select(o => o.UserId).ToList();

#if (!TEST)
            if(id_list.Any())
            {
                MyFinishRequests = db.CheckupTicketT.Where(o => o.State != "dokončeno").Where(o => o.ToDoctor.UserId == id_list.First())
#else
            MyFinishRequests = db.CheckupTicketT.Where(o => o.State == "dokončeno")
#endif
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
#if (!TEST)
            }
#endif
            return View(MyFinishRequests);
        }

        

        
    }
}
