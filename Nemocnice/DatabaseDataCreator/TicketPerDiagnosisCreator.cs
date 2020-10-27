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
            CheckupTicket TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 25).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 2).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina klíční kosti").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 3).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina nártní kosti").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 4).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 6).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 8).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 9).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět ledvin").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 14).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 15).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 20).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 23).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            TicketId = db.CheckupTicketT.Where(s => s.Patient.PatientID == 1).ToList().First();
            db.TicketPerDiagnosisT.Add(TicketPerDiagnosisAdd(DiagnosisId, TicketId));

            db.SaveChanges();
        }
        private TicketPerDiagnosis TicketPerDiagnosisAdd(Diagnosis diagnosis, CheckupTicket checkUpTicket)
        {
            return new TicketPerDiagnosis { Diagnosis = diagnosis, CheckupTicket = checkUpTicket };
        }
    }
}
