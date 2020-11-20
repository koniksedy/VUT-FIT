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
            /*
            Patient PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9638521235").ToList().First();
            Diagnosis DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            DateTime start = new DateTime(2016, 1, 1, 14, 50, 2);
            DateTime end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2568745120").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            start = new DateTime(2016, 2, 2, 11, 02, 25);
            end = new DateTime(2016, 5, 18, 14, 22, 20);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5698742103").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina klíční kosti").ToList().First();
            start = new DateTime(2016, 3, 3, 11, 15, 56);
            end = new DateTime(2016, 5, 12, 14, 45, 2);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2356987452").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina nártní kosti").ToList().First();
            start = new DateTime(2016, 4, 5, 12, 11, 19);
            end = new DateTime(2016, 6, 1, 16, 10, 5);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8562101478").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina bérce").ToList().First();
            start = new DateTime(2017, 5, 6, 13, 44, 28);
            end = new DateTime(2017, 10, 6, 10, 42, 48);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8963214568").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            start = new DateTime(2017, 6, 7, 14, 42, 25);
            end = new DateTime(2017, 6, 29, 9, 2, 55);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7412563201").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            start = new DateTime(2017, 8, 31, 14, 12, 38);
            end = new DateTime(2017, 9, 21, 10, 12, 45);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8745213698").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            start = new DateTime(2017, 8, 22, 10, 58, 39);
            end = new DateTime(2017, 8, 28, 8, 54, 9);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2014785630").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            start = new DateTime(2017, 9, 17, 8, 20, 15);
            end = System.DateTime.MinValue;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9856321452").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět ledvin").ToList().First();
            start = new DateTime(2018, 12, 21, 12, 54, 52);
            end = new DateTime(2019, 1, 10, 7, 21, 2);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8520147852").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            start = new DateTime(2018, 1, 3, 12, 42, 12);
            end = new DateTime(2018, 2, 6, 8, 21, 2);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9236541258").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            start = new DateTime(2019, 5, 27, 11, 45, 9);
            end = new DateTime(2019, 6, 18, 10, 5, 19);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8523654124").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            start = new DateTime(2019, 7, 14, 13, 28, 58);  
            end = new DateTime(2019, 7, 31, 12, 4, 8);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5478512365").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            start = new DateTime(2020, 9, 10, 15, 40, 13);
            end = System.DateTime.MinValue;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));
            */

            Patient PatientId;
            Diagnosis DiagnosisId;
            DateTime start;
            DateTime end;

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8605274117").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            start = new DateTime(2016, 2, 1, 10, 20, 0);
            end = new DateTime(2016, 3, 7, 9, 12, 0); ;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7204236798").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            start = new DateTime(2017, 11, 25, 13, 34, 0);
            end = new DateTime(2017, 12, 12, 17, 55, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7204236798").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            start = new DateTime(2018, 3, 10, 15, 47, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9409278736").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            start = new DateTime(2017, 4, 23, 14, 38, 0);
            end = new DateTime(2017, 4, 23, 14, 38, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9409278736").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            start = new DateTime(2018, 9, 7, 7, 0, 0);
            end = new DateTime(2018, 9, 7, 7, 0, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9409278736").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            start = new DateTime(2019, 10, 1, 10, 21, 0);
            end = new DateTime(2019, 10, 1, 10, 21, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7306261116").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            start = new DateTime(2018, 3, 30, 14, 43, 0);
            end = new DateTime(2018, 3, 30, 14, 43, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7306261116").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět dásní").ToList().First();
            start = new DateTime(2018, 4, 28, 13, 34, 0);
            end = new DateTime(2018, 4, 28, 13, 34, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5804249330").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina holenní kosti").ToList().First();
            start = new DateTime(2019, 5, 17, 17, 56, 0);
            end = new DateTime(2019, 5, 17, 17, 56, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310079755").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            start = new DateTime(2018, 4, 9, 15, 47, 0);
            end = new DateTime(2018, 4, 9, 15, 47, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310079755").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět slepého střeva").ToList().First();
            start = new DateTime(2019, 2, 11, 14, 39, 0);
            end = new DateTime(2019, 2, 11, 14, 39, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310079755").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            start = new DateTime(2019, 8, 24, 7, 0, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5609101014").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            start = new DateTime(2018, 6, 7, 11, 22, 0);
            end = new DateTime(2018, 8, 9, 15, 43, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5407078259").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Mozková mrtvice").ToList().First();
            start = new DateTime(2019, 5, 19, 7, 5, 0);
            end = new DateTime(2019, 5, 19, 7, 5, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7952217779").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            start = new DateTime(2020, 1, 12, 15, 48, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "6454221719").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            start = new DateTime(2020, 4, 14, 11, 22, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "6454221719").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            start = new DateTime(2020, 5, 10, 9, 14, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "6960249307").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            start = new DateTime(2020, 7, 28, 8, 5, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8661094849").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            start = new DateTime(2020, 7, 13, 17, 57, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7262204400").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina holenní kosti").ToList().First();
            start = new DateTime(2020, 2, 15, 8, 10, 0);
            end = new DateTime(2020, 2, 15, 8, 10, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0254270401").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Tuberkulóza").ToList().First();
            start = new DateTime(2018, 12, 17, 12, 31, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0254270401").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            start = new DateTime(2019, 8, 17, 11, 23, 0);
            end = new DateTime(2020, 1, 20, 15, 44, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0261203349").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            start = new DateTime(2019, 11, 14, 8, 6, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            start = new DateTime(2017, 12, 30, 17, 57, 0);
            end = new DateTime(2018, 1, 27, 10, 19, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            start = new DateTime(2018, 5, 12, 8, 10, 0);
            end = new DateTime(2019, 2, 10, 12, 32, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7356020210").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            start = new DateTime(2018, 12, 8, 16, 53, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7356020210").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            start = new DateTime(2018, 12, 25, 15, 45, 0);
            end = new DateTime(2019, 1, 5, 17, 58, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0110038379").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zápal plic").ToList().First();
            start = new DateTime(2019, 5, 22, 16, 49, 0);
            end = new DateTime(2019, 5, 30, 9, 11, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310190998").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            start = new DateTime(2018, 7, 19, 7, 2, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7705156437").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            start = new DateTime(2019, 11, 25, 9, 15, 0);
            end = new DateTime(2019, 11, 25, 9, 15, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0053229792").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět mozkových blan").ToList().First();
            start = new DateTime(2018, 2, 27, 8, 7, 0);
            end = new DateTime(2018, 3, 5, 12, 28, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            start = new DateTime(2019, 5, 17, 10, 20, 0);
            end = new DateTime(2019, 5, 17, 10, 20, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            start = new DateTime(2019, 7, 24, 9, 11, 0);
            end = new DateTime(2019, 7, 30, 7, 3, 0);
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            start = new DateTime(2020, 2, 14, 17, 54, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7951257193").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            start = new DateTime(2020, 4, 11, 9, 16, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8052047377").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina předloktí").ToList().First();
            start = new DateTime(2020, 7, 19, 17, 59, 0);
            end = default;
            db.PatientTreatmentLogT.Add(PatientTreatmentLogAdd(PatientId, DiagnosisId, start, end));

            db.SaveChanges();
        }
        private PatientTreatmentLog PatientTreatmentLogAdd( Patient patient, Diagnosis diagnosis, DateTime startOfTreatment, DateTime endOfTreatment)
        {
            return new PatientTreatmentLog { Patient = patient, Diagnosis = diagnosis, StartOfTreatment = startOfTreatment, EndOfTreatment = endOfTreatment };
        }
    }
}
