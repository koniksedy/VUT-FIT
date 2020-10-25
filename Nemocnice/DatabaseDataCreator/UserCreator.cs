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
            Login log = db.LoginT.Where(s => s.CurrentLogin == "xnovot00").ToList().First();
            Address add = db.AddressT.Where(s => s.AddressId == 3).ToList().First();
            db.UserT.Add(UserAdd(log, "Adam", "Novotný", "Ing.", "737564254", "novotny@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xmalic0a").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 1).ToList().First();
            db.UserT.Add(UserAdd(log, "Jana", "Malíčková", null, "789456123", "malickova@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xbilik00").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 2).ToList().First();
            db.UserT.Add(UserAdd(log, "Jan", "Bilík", "Ing.", "256321478", "bilik@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xjanec0a").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 4).ToList().First();
            db.UserT.Add(UserAdd(log, "Iva", "Janečková", "Ing.", "785985654", "janeckova@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xsvobo00").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 5).ToList().First();
            db.UserT.Add(UserAdd(log, "David", "Svoboda", "Ing.", "785123589", "svoboda@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xovale0a").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 6).ToList().First();
            db.UserT.Add(UserAdd(log, "Petra", "Oválková", "Ing.", "703256854", "ovalkova@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xdvora0a").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 7).ToList().First();
            db.UserT.Add(UserAdd(log, "Petr", "Dvořák", "Mudr.", "741852963", "dvorak@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xcerny00").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 8).ToList().First();
            db.UserT.Add(UserAdd(log, "Jiří", "Černý", "Mudr.", "775869852", "cerny@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xvesel0a").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 9).ToList().First();
            db.UserT.Add(UserAdd(log, "Hana", "Veselá", "Mudr.", "775869852", "vesela@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xhorak00").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 10).ToList().First();
            db.UserT.Add(UserAdd(log, "Jan", "Horák", "Mudr.", "805654123", "horak@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xnemec08").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 11).ToList().First();
            db.UserT.Add(UserAdd(log, "Irena", "Němečková", "Mudr.", "773562147", "nemeckova@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xmarek05").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 12).ToList().First();
            db.UserT.Add(UserAdd(log, "Zdeněk", "Marek", "Mudr.", "737589654", "marek@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xpospi0a").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 13).ToList().First();
            db.UserT.Add(UserAdd(log, "Dominik", "Pospíšil", "Mudr.", "774547123", "pospisil@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xhajek0a").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 14).ToList().First();
            db.UserT.Add(UserAdd(log, "Adam", "Hájek", "Mudr.", "789456123", "hajek@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xjelin00").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 15).ToList().First();
            db.UserT.Add(UserAdd(log, "Sabina", "Jelínková", "Mudr.", "502635452", "jelinkova@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xkrali06").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 16).ToList().First();
            db.UserT.Add(UserAdd(log, "Martina", "Králíková", "Mudr.", "708985456", "kralikova@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xbenes01").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 17).ToList().First();
            db.UserT.Add(UserAdd(log, "Kateřina", "Benešová", "Mudr.", "789456258", "benesova@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xdolez00").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 18).ToList().First();
            db.UserT.Add(UserAdd(log, "Jakub", "Doležal", "Mudr.", "778582654", "dolezal@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xfiala01").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 19).ToList().First();
            db.UserT.Add(UserAdd(log, "Karel", "Fiala", null, "741852541", "fiala@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xsedla03").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 20).ToList().First();
            db.UserT.Add(UserAdd(log, "Kamil", "Sedláček", null, "805695555", "sedlacek@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xzeman0a").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 21).ToList().First();
            db.UserT.Add(UserAdd(log, "Ondřej", "Zeman", null, "987456254", "zeman@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xkolar05").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 22).ToList().First();
            db.UserT.Add(UserAdd(log, "Ivan", "Kolář", null, "605258321", "kolar@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xkrejc02").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 23).ToList().First();
            db.UserT.Add(UserAdd(log, "Marie", "Krejčířová", "Bc.", "778555421", "krejcirova@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xnavra00").ToList().First();
            db.UserT.Add(UserAdd(log, "Josef", "Navrátil", "Bc.", "802369544", "krejcirova@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xcerma0a").ToList().First();
            db.UserT.Add(UserAdd(log, "Jakub", "Čermák", "Bc.", "705456258", "cermak@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xbarto08").ToList().First();
            db.UserT.Add(UserAdd(log, "Daniela", "Bartošová", null, "603258444", "bartosova@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xsimek00").ToList().First();
            db.UserT.Add(UserAdd(log, "Pavel", "Šimek", null, "706582111", "simek@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xsykor01").ToList().First();
            db.UserT.Add(UserAdd(log, "Hana", "Sýkorová", "Mgr.", "704555021", "sykorova@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xsouku0a").ToList().First();
            db.UserT.Add(UserAdd(log, "Jana", "Soukupová", "Mgr.", "802369541", "soukupova@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xmorav07").ToList().First();
            db.UserT.Add(UserAdd(log, "Tereza", "Moravcová", "Ing.", "708596023", "moravcova@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xsevci06").ToList().First();
            db.UserT.Add(UserAdd(log, "Ladislav", "Ševčík", null, "705258452", "sevcik@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xmasek00").ToList().First();
            db.UserT.Add(UserAdd(log, "Karel", "Maseka", null, "809632025", "maseka@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xhrusk08").ToList().First();
            db.UserT.Add(UserAdd(log, "Pavlína", "Hrušková", null, "702555412", "hruskova@seznam.cz", null));

            log = db.LoginT.Where(s => s.CurrentLogin == "xpavli00").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 25).ToList().First();
            db.UserT.Add(UserAdd(log, "Zbyněk", "Pavlíček", null, "706589789", "pavlicek@seznam.cz", add));

            log = db.LoginT.Where(s => s.CurrentLogin == "xjanda0a").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 28).ToList().First();
            db.UserT.Add(UserAdd(log, "Daniel", "Janda", "Bc.", "737562205", "janda@seznam.cz", add));

            db.SaveChanges();
        }
        private User UserAdd(Login login, string name, string surname, string title, string phone, string email, Address workaddress)
        {
            return new User { Login = login, Name = name, Surname = surname, Title = title, Phone = phone, Email = email, WorkAddress = workaddress };
        }
    }
}