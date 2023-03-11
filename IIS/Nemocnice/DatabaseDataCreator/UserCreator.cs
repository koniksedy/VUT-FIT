using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class UserCreator
    {
        public void UserCreate(DatabaseContext db)
        {
            Address add = db.AddressT.Where(s => s.AddressId == 3).ToList().First();
            db.UserT.Add(UserAdd("xnovot00", "Adam", "Novotný", "Ing.", "737564254", "novotny@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 3).ToList().First();
            db.UserT.Add(UserAdd("7204236798", "Adam", "Novotný", "Ing.", "737564254", "novotny@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 1).ToList().First();
            db.UserT.Add(UserAdd("xmalic0a", "Jana", "Malíčková", null, "789456123", "malickova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 2).ToList().First();
            db.UserT.Add(UserAdd("xbilik00", "Jan", "Bilík", "Ing.", "256321478", "bilik@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 4).ToList().First();
            db.UserT.Add(UserAdd("xjanec0a", "Ivan", "Janečka", "Ing.", "785985654", "janecka@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 4).ToList().First();
            db.UserT.Add(UserAdd("9409278736", "Ivan", "Janečka", "Ing.", "785985654", "janecka@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 5).ToList().First();
            db.UserT.Add(UserAdd("xsvobo00", "David", "Svoboda", "Ing.", "785123589", "svoboda@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 6).ToList().First();
            db.UserT.Add(UserAdd("xovale0a", "Petra", "Oválková", "Ing.", "703256854", "ovalkova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 7).ToList().First();
            db.UserT.Add(UserAdd("xdvora0a", "Petr", "Dvořák", "Mudr.", "741852963", "dvorak@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 8).ToList().First();
            db.UserT.Add(UserAdd("xcerny00", "Jiří", "Černý", "Mudr.", "775869852", "cerny@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 9).ToList().First();
            db.UserT.Add(UserAdd("xvesel0a", "Hana", "Veselá", "Mudr.", "775869852", "vesela@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 10).ToList().First();
            db.UserT.Add(UserAdd("xhorak00", "Jan", "Horák", "Mudr.", "805654123", "horak@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 11).ToList().First();
            db.UserT.Add(UserAdd("xnemec08", "Irena", "Němečková", "Mudr.", "773562147", "nemeckova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 12).ToList().First();
            db.UserT.Add(UserAdd("xmarek05", "Zdeněk", "Marek", "Mudr.", "737589654", "marek@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 13).ToList().First();
            db.UserT.Add(UserAdd("xpospi0a", "Dominik", "Pospíšil", "Mudr.", "774547123", "pospisil@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 13).ToList().First();
            db.UserT.Add(UserAdd("7306261116", "Dominik", "Pospíšil", "Mudr.", "774547123", "pospisil@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 14).ToList().First();
            db.UserT.Add(UserAdd("xhajek0a", "Adam", "Hájek", "Mudr.", "789456123", "hajek@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 14).ToList().First();
            db.UserT.Add(UserAdd("5804249330", "Adam", "Hájek", "Mudr.", "789456123", "hajek@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 15).ToList().First();
            db.UserT.Add(UserAdd("xjelin00", "Sabina", "Jelínek", "Mudr.", "502635452", "jelinek@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 15).ToList().First();
            db.UserT.Add(UserAdd("7310079755", "Sabina", "Jelínek", "Mudr.", "502635452", "jelinek@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 16).ToList().First();
            db.UserT.Add(UserAdd("xkrali06", "Martin", "Králík", "Mudr.", "708985456", "kralik@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 16).ToList().First();
            db.UserT.Add(UserAdd("5609101014", "Martin", "Králík", "Mudr.", "708985456", "kralik@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 17).ToList().First();
            db.UserT.Add(UserAdd("xbenes01", "Marek", "Beneš", "Mudr.", "789456258", "benes@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 17).ToList().First();
            db.UserT.Add(UserAdd("5407078259", "Marek", "Beneš", "Mudr.", "789456258", "benes@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 18).ToList().First();
            db.UserT.Add(UserAdd("xdolez00", "Java", "Doležalová", "Mudr.", "778582654", "dolezalova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 18).ToList().First();
            db.UserT.Add(UserAdd("7952217779", "Java", "Doležalová", "Mudr.", "778582654", "dolezalova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 19).ToList().First();
            db.UserT.Add(UserAdd("6454221719", "Karla", "Fialova", null, "741852541", "fialova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 20).ToList().First();
            db.UserT.Add(UserAdd("6960249307", "Kamila", "Sedláčková", null, "805695555", "sedlackova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 21).ToList().First();
            db.UserT.Add(UserAdd("8661094849", "Marta", "Zemanová", null, "987456254", "zemanova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 22).ToList().First();
            db.UserT.Add(UserAdd("7262204400", "Ivana", "Kolářová", null, "605258321", "kolarova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 23).ToList().First();
            db.UserT.Add(UserAdd("0254270401", "Marie", "Krejčířová", "Bc.", "778555421", "krejcirova@seznam.cz", add));

            db.UserT.Add(UserAdd("0261203349", "Josefie", "Navrátilová", "Bc.", "802369544", "navratilova@seznam.cz", null));

            db.UserT.Add(UserAdd("7561200394", "Jiřina", "Čermákáková", "Bc.", "705456258", "cermak@seznam.cz", null));

            db.UserT.Add(UserAdd("7356020210", "Daniela", "Bartošová", null, "603258444", "bartosova@seznam.cz", null));

            db.UserT.Add(UserAdd("0110038379", "Pavel", "Šimek", null, "706582111", "simek@seznam.cz", null));

            db.UserT.Add(UserAdd("7310190998", "Herman", "Sýkora", "Mgr.", "704555021", "sykora@seznam.cz", null));

            db.UserT.Add(UserAdd("7705156437", "Jan", "Soukup", "Mgr.", "802369541", "soukup@seznam.cz", null));

            db.UserT.Add(UserAdd("0053229792", "Tereza", "Moravcová", "Ing.", "708596023", "moravcova@seznam.cz", null));

            db.UserT.Add(UserAdd("8862026096", "Ladislava", "Ševčíková", null, "705258452", "sevcikova@seznam.cz", null));


            db.UserT.Add(UserAdd("7951257193", "Karla", "Masecká", null, "809632025", "masekcka@seznam.cz", null));


            db.UserT.Add(UserAdd("8052047377", "Pavlína", "Hrušková", null, "702555412", "hruskova@seznam.cz", null));


            add = db.AddressT.Where(s => s.AddressId == 25).ToList().First();
            db.UserT.Add(UserAdd("6712254901", "Zbyněk", "Pavlíček", null, "706589789", "pavlicek@seznam.cz", add));


            add = db.AddressT.Where(s => s.AddressId == 28).ToList().First();
            db.UserT.Add(UserAdd("6261046044", "Daniela", "Jandaová", "Bc.", "737562205", "jandova@seznam.cz", add));

            db.UserT.Add(UserAdd("Admin", "Admin", "Adminovič", null, "777777777", "admin@admin.cz", null));

            db.SaveChanges();
        }
        private User UserAdd(string login, string name, string surname, string title, string phone, string email, Address workaddress)
        {
            return new User { Login = login, Name = name, Surname = surname, Title = title, Phone = phone, Email = email, WorkAddress = workaddress };
        }
    }
}