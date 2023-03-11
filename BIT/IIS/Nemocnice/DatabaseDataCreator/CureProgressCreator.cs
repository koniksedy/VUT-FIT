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
            /*
            MedicallReport ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2016, 1, 1, 14, 50, 2)).ToList().First();
            Diagnosis DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2016, 1, 10, 9, 30, 24)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

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
            */

            MedicallReport ReportId;
            Diagnosis DiagnosisId;

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2016, 2, 1, 10, 20, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2016, 3, 7, 9, 12, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 118799 && s.CreateDate == new DateTime(2017, 11, 25, 13, 34, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 966699 && s.CreateDate == new DateTime(2017, 12, 12, 17, 55, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2018, 3, 10, 15, 47, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 966699 && s.CreateDate == new DateTime(2017, 4, 23, 14, 38, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 118799 && s.CreateDate == new DateTime(2018, 9, 7, 7, 0, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2019, 10, 1, 10, 21, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jednorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 147852 && s.CreateDate == new DateTime(2018, 3, 30, 14, 43, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 147852 && s.CreateDate == new DateTime(2018, 4, 28, 13, 34, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět dásní").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 147852 && s.CreateDate == new DateTime(2019, 5, 17, 17, 56, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina holenní kosti").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 100021 && s.CreateDate == new DateTime(2018, 4, 9, 15, 47, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 100021 && s.CreateDate == new DateTime(2019, 2, 11, 14, 39, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět slepého střeva").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 987588 && s.CreateDate == new DateTime(2019, 8, 24, 7, 0, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 147852 && s.CreateDate == new DateTime(2018, 6, 7, 11, 22, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2018, 8, 9, 15, 43, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 987588 && s.CreateDate == new DateTime(2019, 5, 19, 7, 5, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Mozková mrtvice").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 100024 && s.CreateDate == new DateTime(2020, 1, 12, 15, 48, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 147852 && s.CreateDate == new DateTime(2020, 1, 27, 8, 10, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "kontrola"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 254789 && s.CreateDate == new DateTime(2020, 5, 30, 7, 1, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "kontrola"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2020, 4, 14, 11, 22, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 100024 && s.CreateDate == new DateTime(2020, 5, 10, 9, 14, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 147852 && s.CreateDate == new DateTime(2020, 7, 28, 8, 5, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 100024 && s.CreateDate == new DateTime(2020, 7, 13, 17, 57, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 254789 && s.CreateDate == new DateTime(2020, 8, 21, 15, 48, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Neštovice").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "kontrola"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2020, 2, 15, 8, 10, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina holenní kosti").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 987588 && s.CreateDate == new DateTime(2018, 12, 17, 12, 31, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Tuberkulóza").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 987777 && s.CreateDate == new DateTime(2019, 8, 17, 11, 23, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 254789 && s.CreateDate == new DateTime(2020, 1, 20, 15, 44, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 174587 && s.CreateDate == new DateTime(2019, 11, 14, 8, 6, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2017, 12, 30, 17, 57, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 254789 && s.CreateDate == new DateTime(2018, 1, 27, 10, 19, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 111245 && s.CreateDate == new DateTime(2018, 5, 12, 8, 10, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 147852 && s.CreateDate == new DateTime(2019, 2, 10, 12, 32, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 111245 && s.CreateDate == new DateTime(2018, 12, 8, 16, 53, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2018, 12, 25, 15, 45, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2019, 1, 5, 17, 58, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 254789 && s.CreateDate == new DateTime(2019, 5, 22, 16, 49, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zápal plic").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 987478 && s.CreateDate == new DateTime(2019, 5, 30, 9, 11, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zápal plic").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 111245 && s.CreateDate == new DateTime(2018, 7, 19, 7, 2, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 174587 && s.CreateDate == new DateTime(2018, 12, 3, 16, 54, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "kontrola"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 254789 && s.CreateDate == new DateTime(2019, 11, 25, 9, 15, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 111245 && s.CreateDate == new DateTime(2018, 2, 27, 8, 7, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět mozkových blan").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 987478 && s.CreateDate == new DateTime(2018, 3, 5, 12, 28, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět mozkových blan").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 987777 && s.CreateDate == new DateTime(2019, 5, 17, 10, 20, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "jadnorázové"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 147852 && s.CreateDate == new DateTime(2019, 7, 24, 9, 11, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 254789 && s.CreateDate == new DateTime(2019, 7, 30, 7, 3, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "dokončeno"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 987478 && s.CreateDate == new DateTime(2020, 2, 14, 17, 54, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 159357 && s.CreateDate == new DateTime(2020, 4, 11, 9, 16, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 254789 && s.CreateDate == new DateTime(2020, 8, 9, 8, 7, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "kontrola"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 147852 && s.CreateDate == new DateTime(2020, 7, 19, 17, 59, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina předloktí").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "zahájení"));

            ReportId = db.MedicallReportT.Where(s => s.Author.ICZ == 174587 && s.CreateDate == new DateTime(2020, 8, 6, 7, 3, 0)).ToList().First();
            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina předloktí").ToList().First();
            db.CureProgressT.Add(CureProgressAdd(ReportId, DiagnosisId, "kontrola"));

            db.SaveChanges();
        }
        private CureProgress CureProgressAdd(MedicallReport medicallReport, Diagnosis diagnosis, string stateOfTreatment)
        {
            return new CureProgress { MedicallReport = medicallReport, Diagnosis = diagnosis, StateOfTreatment = stateOfTreatment };
        }
    }
}
