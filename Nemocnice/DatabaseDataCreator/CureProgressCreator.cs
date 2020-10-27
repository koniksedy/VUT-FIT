using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class CureProgressCreator
    {
        public void CureProgressCreate(DatabaseContext db)
        {
            MedicallReport ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 25).ToList().First();
            Diagnosis DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "probíhá léčba"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 2).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "probíhá léčba"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 10).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina klíční kosti").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 4).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina nártní kosti").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "probíhá léčba"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 5).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina bérce").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "probíhá léčba"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 25).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 8).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 9).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "probíhá léčba"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 10).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 14).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět ledvin").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 15).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "probíhá léčba"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 20).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "probíhá léčba"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 23).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "probíhá léčba"));

            ReportId = db.MedicallReportT.Where(s => s.Patient.PatientID == 1).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));


            db.SaveChanges();
        }
        private CureProgress CureProgressAdd(MedicallReport medicallReport, Diagnosis diagnosis, string stateOfTreatment)
        {
            return new CureProgress { MedicallReport = medicallReport, Diagnosis = diagnosis, StateOfTreatment = stateOfTreatment };
        }
    }
}
