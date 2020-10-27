using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class CheckUpTicketCreator
    {
        public void CheckUpTicketCreate(DatabaseContext db)
        {

            Doctor DoctorBy = db.DoctorT.Where(s => s.ICZ == 1).ToList().First();
            Doctor DoctorTo = db.DoctorT.Where(s => s.ICZ == 2).ToList().First();
            Patient PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 9638521235).ToList().First();
            DateTime start = new DateTime(2016, 1, 1, 14, 45, 5);
            DateTime end = new DateTime(2016, 1, 1, 16, 45, 56);
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "onec odio nisi, accumsan sed porttitor sit amet, laoreet a nulla. Maecenas laoreet est magna, luctus blandit massa luctus et. Pellentesque condimentum sit amet magna ultricies auctor. Suspendisse cursus in metus sed imperdiet.", false, "Lupénka.", false, "dokončeno", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 2).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 4).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 2568745120).ToList().First();
            start = new DateTime(2016, 2, 2, 11, 2, 25);
            end = System.DateTime.MinValue;
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Vivamus vel mi velit. Morbi venenatis dui a interdum maximus. Aliquam gravida consectetur tellus vel euismod. Vestibulum id nisi sed odio pulvinar euismod. Mauris gravida lectus a egestas tincidunt.", true, null, false,"Pacient poslán na retgen, čekám na výsledek.", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 3).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 4).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 5698742103).ToList().First();
            start = new DateTime(2016, 3, 3, 11, 15, 56);
            end = System.DateTime.MinValue;
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Nulla id justo rutrum, tempus mi vel, bibendum felis. Phasellus ac sem sit amet nulla vestibulum imperdiet.", true, null, false, "Pacient poslán na retgen, čekám na výsledek.", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 4).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 4).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 2356987452).ToList().First();
            start = new DateTime(2016, 4, 5, 12, 11, 19);
            end = System.DateTime.MinValue;
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Phasellus vitae quam luctus, accumsan est nec, sollicitudin metus. Integer vel est quam. Donec fringilla, ante non tincidunt interdum, lacus urna vehicula orci, a hendrerit augue arcu nec ipsum.", true, null, false, "Pacient poslán na retgen, čekám na výsledek.", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 5).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 7).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8963214568).ToList().First();
            start = new DateTime(2017, 6, 7, 14, 42, 25);
            end = new DateTime(2017, 6, 7, 16, 12, 5);
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Donec elit risus, commodo at velit ac, molestie consectetur nibh. Nullam interdum ex ut augue vehicula dictum. In dictum nulla ac pulvinar gravida.", false, "Zlomenina prstu.", true, "dokončeno", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 6).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 1).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 7412563201).ToList().First();
            start = new DateTime(2017, 8, 31, 14, 12, 38);
            end = System.DateTime.MinValue;
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Nulla iaculis arcu ut tellus egestas semper. Nulla facilisi. Phasellus dictum ex at orci tempor porta in nec orci.", false, null, false, "Pacient poslán na odběr krve, čekám na výsledek.", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 7).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 10).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8745213698).ToList().First();
            start = new DateTime(2017, 8, 22, 10, 58, 39);
            end = System.DateTime.MinValue;
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Aliquam dolor tortor, feugiat mattis scelerisque eget, consectetur non neque. ", true, null, false, "Pacient poslán na vyšetření k traumatologovi.", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 8).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 2).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 9856321452).ToList().First();
            start = new DateTime(2018, 12, 21, 12, 54, 52);
            end = new DateTime(2018, 12, 21, 13, 32, 50);
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Morbi sit amet risus id sem pulvinar sollicitudin vel quis purus. Vestibulum egestas mi nec tincidunt eleifend. Maecenas viverra egestas nunc non sagittis.", false, "Zánět ledvin.", false, "dokončeno", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 9).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 10).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8520147852).ToList().First();
            start = new DateTime(2018, 1, 3, 12, 42, 12);
            end = new DateTime(2018, 1, 3, 14, 52, 12);
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "In iaculis faucibus ipsum vel egestas. Sed convallis nibh sed ultrices ullamcorper. Aenean congue purus at nibh venenatis eleifend.", false, "U pacienta byla prokázána borelióza.", false, "dokončeno", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 10).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 6).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 9236541258).ToList().First();
            start = new DateTime(2019, 5, 27, 11, 45, 9);
            end = System.DateTime.MinValue;
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Cras lacinia massa eu est blandit, maximus condimentum felis facilisis. Fusce massa sapien, lobortis luctus feugiat in, ullamcorper nec purus.", false, null, false, "Pacient poslán na krevní obraz, čekám na výsledek.", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 11).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 3).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 8523654124).ToList().First();
            start = new DateTime(2019, 7, 14, 13, 28, 58);
            end = new DateTime(2019, 7, 14, 14, 1, 58);
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Phasellus volutpat efficitur turpis dictum dapibus. Phasellus sed tellus fermentum, viverra enim vitae, tincidunt erat.", false, "Pacient má zánět průdušek. nařizuji domácí léčení.", false, "dokončeno", start, end));

            DoctorBy = db.DoctorT.Where(s => s.ICZ == 12).ToList().First();
            DoctorTo = db.DoctorT.Where(s => s.ICZ == 7).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == 5478512365).ToList().First();
            start = new DateTime(2020, 9, 10, 15, 40, 13);
            end = System.DateTime.MinValue;
            db.CheckupTicketT.Add(CheckupTicketAdd(DoctorBy, DoctorTo, PatientId, "Cras efficitur et augue vel tincidunt. Maecenas facilisis arcu orci, sed consectetur sapien molestie a. Nulla eu posuere turpis.", true, null, false, "Pacient poslán na rentgenové vyšetření. Čekám na výsledek.", start, end));





            db.SaveChanges();
        }
        private CheckupTicket CheckupTicketAdd(Doctor createdBy, Doctor toDoctor, Patient patient, string description, bool includePicDescript, string result, bool includePicRes, string state, DateTime createDate, DateTime finishDate)
        {
            return new CheckupTicket { CreatedBy = createdBy, ToDoctor = toDoctor, Patient = patient, Description = description, IncludePicDescript = includePicDescript, Result = result, IncludePicRes = includePicRes, State = state, CreateDate = createDate, FinishDate = finishDate };
        }
    }
}
