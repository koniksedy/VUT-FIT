using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class PatientTreatmentLogCreator
    {
        public void PatientTreatmentLogCreate(DatabaseContext db)
        {
            Patient PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 9638521235).ToList().First();
            Diagnosis DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            DateTime start = new DateTime(2016, 1, 1, 14, 45, 5);
            DateTime end = System.DateTime.MinValue;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 2568745120).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            start = new DateTime(2016, 2, 2, 11, 02, 25);
            end = new DateTime(2016, 5, 18, 14, 22, 20);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 5698742103).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina klíční kosti").ToList().First();
            start = new DateTime(2016, 3, 3, 11, 15, 56);
            end = new DateTime(2016, 5, 12, 14, 45, 2);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 2356987452).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina nártní kosti").ToList().First();
            start = new DateTime(2016, 4, 5, 12, 11, 19);
            end = new DateTime(2016, 6, 1, 16, 10, 5);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8562101478).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina bérce").ToList().First();
            start = new DateTime(2017, 5, 6, 13, 44, 28);
            end = new DateTime(2017, 10, 6, 10, 42, 48);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8963214568).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            start = new DateTime(2017, 6, 7, 14, 42, 25);
            end = new DateTime(2017, 6, 29, 9, 2, 55);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 7412563201).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            start = new DateTime(2017, 8, 31, 14, 12, 38);
            end = new DateTime(2017, 9, 21, 10, 12, 45);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8745213698).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            start = new DateTime(2017, 8, 22, 10, 58, 39);
            end = new DateTime(2017, 8, 28, 8, 54, 9);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 2014785630).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            start = new DateTime(2017, 9, 17, 8, 20, 15);
            end = System.DateTime.MinValue;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 9856321452).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět ledvin").ToList().First();
            start = new DateTime(2018, 12, 21, 12, 54, 52);
            end = new DateTime(2019, 1, 10, 7, 21, 2);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8520147852).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            start = new DateTime(2018, 1, 3, 12, 42, 12);
            end = new DateTime(2018, 2, 6, 8, 21, 2);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 9236541258).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            start = new DateTime(2019, 5, 27, 11, 45, 9);
            end = new DateTime(2019, 6, 18, 10, 5, 19);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8523654124).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            start = new DateTime(2019, 7, 14, 13, 28, 58);
            end = new DateTime(2019, 7, 31, 12, 4, 8);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 5478512365).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            start = new DateTime(2020, 9, 10, 15, 40, 13);
            end = System.DateTime.MinValue;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            db.SaveChanges();
        }
        private PatientTreatmentLog PatientTreatmentLogAdd( Patient patient, Diagnosis diagnosis, DateTime startOfTreatment, DateTime endOfTreatment)
        {
            return new PatientTreatmentLog { Patient = patient, Diagnosis = diagnosis, StartOfTreatment = startOfTreatment, EndOfTreatment = endOfTreatment };
        }
    }
}
