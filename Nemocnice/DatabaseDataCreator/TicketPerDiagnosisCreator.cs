using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class TicketPerDiagnosisCreator
    {
        public void TicketPerDiagnosisCreate(DatabaseContext db)
        {

            Diagnosis DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            CheckupTicket TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "8605274117").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "7204236798").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina klíční kosti").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "9409278736").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina nártní kosti").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "7306261116").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "5804249330").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "7310079755").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "5609101014").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět ledvin").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "7952217779").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "6454221719").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "6960249307").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "8661094849").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.SocialSecurityNum == "7262204400").ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            db.SaveChanges();
        }
        private TicketPerDiagnosis TicketPerDiagnosisAdd(Diagnosis diagnosis, CheckupTicket checkUpTicket)
        {
            return new TicketPerDiagnosis { Diagnosis = diagnosis, CheckupTicket = checkUpTicket };
        }
    }
}
