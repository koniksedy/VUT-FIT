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
            /*
            Doctor DoctorId = db.DoctorT.Where(s => s.
            == 159357).ToList().First();
            long PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9638521235").Select(x => x.SocialSecurityNum).ToList().First();
            MedicallActivityPrice MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            Diagnosis DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            DateTime datumVykon = new DateTime(2016, 1, 1, 14, 50, 2);
            DateTime datumRozhodnuti = new DateTime(2016, 1, 10, 8, 45, 15);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2568745120").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            datumVykon = new DateTime(2016, 2, 2, 11, 2, 25);
            datumRozhodnuti = new DateTime(2016, 2, 15, 15, 02, 05);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5698742103").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina klíční kosti").ToList().First();
            datumVykon = new DateTime(2016, 3, 3, 11, 15, 56);
            datumRozhodnuti = new DateTime(2016, 3, 13, 12, 2, 47);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2356987452").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina nártní kosti").ToList().First();
            datumVykon = new DateTime(2016, 4, 5, 12, 11, 19);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8562101478").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina bérce").ToList().First();
            datumVykon = new DateTime(2017, 5, 6, 13, 44, 28);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8963214568").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            datumVykon = new DateTime(2017, 6, 7, 14, 42, 25);
            datumRozhodnuti = new DateTime(2017, 6, 25, 9, 12, 36);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7412563201").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            datumVykon = new DateTime(2017, 8, 31, 14, 12, 38);
            datumRozhodnuti = new DateTime(2017, 9, 12, 14, 58, 48);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8745213698").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Vyšetření traumatologem").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            datumVykon = new DateTime(2017, 8, 22, 10, 58, 39);
            datumRozhodnuti = new DateTime(2017, 8, 30, 10, 51, 2);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2014785630").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            datumVykon = new DateTime(2017, 9, 17, 8, 20, 15);
            datumRozhodnuti = new DateTime(2017, 9, 29, 9, 23, 16);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9856321452").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět ledvin").ToList().First();
            datumVykon = new DateTime(2018, 12, 21, 12, 54, 52);
            datumRozhodnuti = new DateTime(2019, 1, 14, 9, 29, 9);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8520147852").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            datumVykon = new DateTime(2018, 1, 3, 12, 42, 12);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9236541258").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            datumVykon = new DateTime(2019, 5, 27, 11, 45, 9);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8523654124").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Preventivní prohlídka").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            datumVykon = new DateTime(2019, 7, 14, 13, 28, 58);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5478512365").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny stehenní kosti").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            datumVykon = new DateTime(2020, 9, 10, 15, 40, 13);
            datumRozhodnuti = new DateTime(2020, 9, 28, 10, 30, 38);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));
            */

            Doctor DoctorId;
            string PatientId;
            MedicallActivityPrice MedicalId;
            Diagnosis DiagnosisId;
            DateTime datumVykon;
            DateTime datumRozhodnuti;

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8605274117").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            datumVykon = new DateTime(2016, 2, 1, 10, 20, 0);
            datumRozhodnuti = new DateTime(2016, 4, 10, 11, 20, 20);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8605274117").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            datumVykon = new DateTime(2016, 3, 7, 9, 12, 0);
            datumRozhodnuti = new DateTime(2016, 4, 6, 9, 12, 12);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7204236798").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            datumVykon = new DateTime(2017, 11, 25, 13, 34, 0);
            datumRozhodnuti = new DateTime(2018, 2, 17, 14, 34, 34);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7204236798").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            datumVykon = new DateTime(2017, 12, 12, 17, 55, 0);
            datumRozhodnuti = new DateTime(2018, 4, 27, 19, 55, 55);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7204236798").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            datumVykon = new DateTime(2018, 3, 10, 15, 47, 0);
            datumRozhodnuti = new DateTime(2016, 2, 15, 15, 02, 05);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9409278736").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            datumVykon = new DateTime(2017, 4, 23, 14, 38, 0);
            datumRozhodnuti = new DateTime(2017, 7, 19, 15, 38, 38);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9409278736").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Preventivní prohlídka").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            datumVykon = new DateTime(2018, 9, 7, 7, 0, 0);
            datumRozhodnuti = new DateTime(2018, 10, 1, 7, 0, 0);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9409278736").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            datumVykon = new DateTime(2019, 10, 1, 10, 21, 0);
            datumRozhodnuti = new DateTime(2020, 1, 11, 11, 21, 21);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7306261116").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Vstupní vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            datumVykon = new DateTime(2018, 3, 30, 14, 43, 0);
            datumRozhodnuti = new DateTime(2016, 2, 15, 15, 02, 05);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7306261116").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Preventivní prohlídka").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět dásní").ToList().First();
            datumVykon = new DateTime(2018, 4, 28, 13, 34, 0);
            datumRozhodnuti = new DateTime(2018, 7, 17, 14, 34, 34);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5804249330").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny holenní kosti").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina holenní kosti").ToList().First();
            datumVykon = new DateTime(2019, 5, 17, 17, 56, 0);
            datumRozhodnuti = new DateTime(2019, 9, 27, 19, 56, 56);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310079755").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Vyšetření traumatologem").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            datumVykon = new DateTime(2018, 4, 9, 15, 47, 0);
            datumRozhodnuti = new DateTime(2018, 8, 23, 17, 47, 47);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310079755").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět slepého střeva").ToList().First();
            datumVykon = new DateTime(2019, 2, 11, 14, 39, 0);
            datumRozhodnuti = new DateTime(2019, 5, 19, 15, 39, 39);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310079755").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            datumVykon = new DateTime(2019, 8, 24, 7, 0, 0);
            datumRozhodnuti = new DateTime(2019, 9, 1, 7, 0, 0);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5609101014").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            datumVykon = new DateTime(2018, 6, 7, 11, 22, 0);
            datumRozhodnuti = new DateTime(2016, 2, 15, 15, 02, 05);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5609101014").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            datumVykon = new DateTime(2018, 8, 9, 15, 43, 0);
            datumRozhodnuti = new DateTime(2018, 11, 21, 16, 43, 43);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5407078259").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Mozková mrtvice").ToList().First();
            datumVykon = new DateTime(2019, 5, 19, 7, 5, 0);
            datumRozhodnuti = new DateTime(2019, 6, 3, 8, 5, 5);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7952217779").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            datumVykon = new DateTime(2020, 1, 12, 15, 48, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7952217779").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            datumVykon = new DateTime(2020, 1, 27, 8, 10, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7952217779").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            datumVykon = new DateTime(2020, 5, 30, 7, 1, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "6454221719").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            datumVykon = new DateTime(2020, 4, 14, 11, 22, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "6454221719").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            datumVykon = new DateTime(2020, 5, 10, 9, 14, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "6960249307").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            datumVykon = new DateTime(2020, 7, 28, 8, 5, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8661094849").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            datumVykon = new DateTime(2020, 7, 13, 17, 57, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8661094849").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            datumVykon = new DateTime(2020, 8, 21, 15, 48, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7262204400").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny holenní kosti").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina holenní kosti").ToList().First();
            datumVykon = new DateTime(2020, 2, 15, 8, 10, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0254270401").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Vstupní vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Tuberkulóza").ToList().First();
            datumVykon = new DateTime(2018, 12, 17, 12, 31, 0);
            datumRozhodnuti = new DateTime(2019, 3, 15, 13, 31, 31);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0254270401").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            datumVykon = new DateTime(2019, 8, 17, 11, 23, 0);
            datumRozhodnuti = new DateTime(2019, 10, 11, 12, 23, 23);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0254270401").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            datumVykon = new DateTime(2020, 1, 20, 15, 44, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0261203349").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            datumVykon = new DateTime(2019, 11, 14, 8, 6, 0);
            datumRozhodnuti = new DateTime(2020, 1, 3, 8, 6, 6);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            datumVykon = new DateTime(2017, 12, 30, 17, 57, 0);
            datumRozhodnuti = new DateTime(2018, 4, 27, 19, 57, 57);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            datumVykon = new DateTime(2018, 1, 27, 10, 19, 0);
            datumRozhodnuti = new DateTime(2018, 3, 10, 11, 19, 19);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            datumVykon = new DateTime(2018, 5, 12, 8, 10, 0);
            datumRozhodnuti = new DateTime(2016, 2, 15, 15, 02, 05);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            datumVykon = new DateTime(2019, 2, 10, 12, 32, 0);
            datumRozhodnuti = new DateTime(2016, 2, 15, 15, 02, 05);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7356020210").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            datumVykon = new DateTime(2018, 12, 8, 16, 53, 0);
            datumRozhodnuti = new DateTime(2019, 4, 26, 18, 53, 53);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7356020210").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            datumVykon = new DateTime(2018, 12, 25, 15, 45, 0);
            datumRozhodnuti = new DateTime(2019, 3, 18, 14, 36, 36);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7356020210").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            datumVykon = new DateTime(2019, 1, 5, 17, 58, 0);
            datumRozhodnuti = new DateTime(2019, 5, 28, 19, 58, 58);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0110038379").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zápal plic").ToList().First();
            datumVykon = new DateTime(2019, 5, 22, 16, 49, 0);
            datumRozhodnuti = new DateTime(2019, 9, 24, 17, 49, 49);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0110038379").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zápal plic").ToList().First();
            datumVykon = new DateTime(2019, 5, 30, 9, 11, 0);
            datumRozhodnuti = new DateTime(2019, 6, 6, 9, 11, 11);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310190998").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            datumVykon = new DateTime(2018, 7, 19, 7, 2, 0);
            datumRozhodnuti = new DateTime(2018, 8, 2, 7, 2, 2);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310190998").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            datumVykon = new DateTime(2018, 12, 3, 16, 54, 0);
            datumRozhodnuti = new DateTime(2019, 4, 26, 18, 54, 54);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7705156437").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            datumVykon = new DateTime(2019, 11, 25, 9, 15, 0);
            datumRozhodnuti = new DateTime(2020, 1, 8, 10, 15, 15);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0053229792").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět mozkových blan").ToList().First();
            datumVykon = new DateTime(2018, 2, 27, 8, 7, 0);
            datumRozhodnuti = new DateTime(2018, 3, 4, 8, 7, 7);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0053229792").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět mozkových blan").ToList().First();
            datumVykon = new DateTime(2018, 3, 5, 12, 28, 0);
            datumRozhodnuti = new DateTime(2018, 5, 14, 13, 28, 28);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Vyšetření traumatologem").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            datumVykon = new DateTime(2019, 5, 17, 10, 20, 0);
            datumRozhodnuti = new DateTime(2016, 2, 15, 15, 02, 05);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "zamítnuto", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            datumVykon = new DateTime(2019, 7, 24, 9, 11, 0);
            datumRozhodnuti = new DateTime(2019, 8, 6, 9, 11, 11);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            datumVykon = new DateTime(2019, 7, 30, 7, 3, 0);
            datumRozhodnuti = new DateTime(2019, 8, 2, 7, 3, 3);
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, "schváleno", datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            datumVykon = new DateTime(2020, 2, 14, 17, 54, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7951257193").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            datumVykon = new DateTime(2020, 4, 11, 9, 16, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7951257193").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            datumVykon = new DateTime(2020, 8, 9, 8, 7, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8052047377").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny paže").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina předloktí").ToList().First();
            datumVykon = new DateTime(2020, 7, 19, 17, 59, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8052047377").Select(x => x.SocialSecurityNum).ToList().First();
            MedicalId = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny paže").ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina předloktí").ToList().First();
            datumVykon = new DateTime(2020, 8, 6, 7, 3, 0);
            datumRozhodnuti = default;
            db.MedicallBillT.Add(MedicallBillAdd(DoctorId, PatientId, MedicalId, DiagnosisId, null, datumVykon, datumRozhodnuti));



            db.SaveChanges();
        }
        private MedicallBill MedicallBillAdd( Doctor doctor, string socialSecurityNum, MedicallActivityPrice medicallActivityPrice, Diagnosis diagnosis, string state, DateTime createDate, DateTime decisionDate)
        {
            return new MedicallBill { Doctor = doctor, SocialSecurityNum = socialSecurityNum, MedicallActivityPrice = medicallActivityPrice, Diagnosis = diagnosis, State = state, CreateDate = createDate, DecisionDate = decisionDate };
        }
    }
}
