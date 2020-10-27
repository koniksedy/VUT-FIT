using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class MedicallBillCreator
    {
        public void MedicallBillCreate(DatabaseContext db)
        {
            Doctor DoctorId = db.DoctorT.Where(s => s.ICZ == 1).ToList().First();
            long PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 9638521235).Select(x => x.SocialSecurityNum).ToList().First();
            MedicallActivityPrice MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            Diagnosis DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            DateTime datumVykon = new DateTime(2016, 1, 1, 14, 45, 5);
            DateTime datumRozhodnuti = new DateTime(2016, 1, 10, 8, 45, 15);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 2).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 2568745120).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            datumVykon = new DateTime(2016, 2, 2, 11, 2, 25);
            datumRozhodnuti = new DateTime(2016, 2, 15, 15, 02, 05);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 3).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 5698742103).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina klíční kosti").ToList().First();
            datumVykon = new DateTime(2016, 3, 3, 11, 15, 56);
            datumRozhodnuti = new DateTime(2016, 3, 13, 12, 2, 47);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 4).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 2356987452).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina nártní kosti").ToList().First();
            datumVykon = new DateTime(2016, 4, 5, 12, 11, 19);
            datumRozhodnuti = System.DateTime.MinValue;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 4).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8562101478).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina bérce").ToList().First();
            datumVykon = new DateTime(2017, 5, 6, 13, 44, 28);
            datumRozhodnuti = System.DateTime.MinValue;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 5).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8963214568).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            datumVykon = new DateTime(2017, 6, 7, 14, 42, 25);
            datumRozhodnuti = new DateTime(2017, 6, 25, 9, 12, 36);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 6).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 7412563201).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            datumVykon = new DateTime(2017, 8, 31, 14, 12, 38);
            datumRozhodnuti = new DateTime(2017, 9, 12, 14, 58, 48);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 7).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8745213698).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Vyšetření traumatologem").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            datumVykon = new DateTime(2017, 8, 22, 10, 58, 39);
            datumRozhodnuti = new DateTime(2017, 8, 30, 10, 51, 2);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 7).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 2014785630).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            datumVykon = new DateTime(2017, 9, 17, 8, 20, 15);
            datumRozhodnuti = new DateTime(2017, 9, 29, 9, 23, 16);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 8).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 9856321452).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět ledvin").ToList().First();
            datumVykon = new DateTime(2018, 12, 21, 12, 54, 52);
            datumRozhodnuti = new DateTime(2019, 1, 14, 9, 29, 9);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 9).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8520147852).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            datumVykon = new DateTime(2018, 1, 3, 12, 42, 12);
            datumRozhodnuti = System.DateTime.MinValue;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 10).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 9236541258).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            datumVykon = new DateTime(2019, 5, 27, 11, 45, 9);
            datumRozhodnuti = System.DateTime.MinValue;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 11).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8523654124).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Preventivní prohlídka").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            datumVykon = new DateTime(2019, 7, 14, 13, 28, 58);
            datumRozhodnuti = System.DateTime.MinValue;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 12).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 5478512365).Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny stehenní kosti").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            datumVykon = new DateTime(2020, 9, 10, 15, 40, 13);
            datumRozhodnuti = new DateTime(2020, 9, 28, 10, 30, 38);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            db.SaveChanges();
        }
        private MedicallBill MedicallBillAdd( Doctor doctor, long socialSecurityNum, MedicallActivityPrice medicallActivityPrice, Diagnosis diagnosis, string state, DateTime createDate, DateTime decisionDate)
        {
            return new MedicallBill { Doctor = doctor, SocialSecurityNum = socialSecurityNum, MedicallActivityPrice = medicallActivityPrice, Diagnosis = diagnosis, State = state, CreateDate = createDate, DecisionDate = decisionDate };
        }
    }
}
