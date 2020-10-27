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

            add = db.AddressT.Where(s => s.AddressId == 1).ToList().First();
            db.UserT.Add(UserAdd("xmalic0a", "Jana", "Malíčková", null, "789456123", "malickova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 2).ToList().First();
            db.UserT.Add(UserAdd("xbilik00", "Jan", "Bilík", "Ing.", "256321478", "bilik@seznam.cz", add));
            add = db.AddressT.Where(s => s.AddressId == 4).ToList().First();
            db.UserT.Add(UserAdd("xjanec0a", "Iva", "Janečková", "Ing.", "785985654", "janeckova@seznam.cz", add));

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

            add = db.AddressT.Where(s => s.AddressId == 14).ToList().First();
            db.UserT.Add(UserAdd("xhajek0a", "Adam", "Hájek", "Mudr.", "789456123", "hajek@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 15).ToList().First();
            db.UserT.Add(UserAdd("xjelin00", "Sabina", "Jelínková", "Mudr.", "502635452", "jelinkova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 16).ToList().First();
            db.UserT.Add(UserAdd("xkrali06", "Martina", "Králíková", "Mudr.", "708985456", "kralikova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 17).ToList().First();
            db.UserT.Add(UserAdd("xbenes01", "Kateřina", "Benešová", "Mudr.", "789456258", "benesova@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 18).ToList().First();
            db.UserT.Add(UserAdd("xdolez00", "Jakub", "Doležal", "Mudr.", "778582654", "dolezal@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 19).ToList().First();
            db.UserT.Add(UserAdd("xfiala01", "Karel", "Fiala", null, "741852541", "fiala@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 20).ToList().First();
            db.UserT.Add(UserAdd("xsedla03", "Kamil", "Sedláček", null, "805695555", "sedlacek@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 21).ToList().First();
            db.UserT.Add(UserAdd("xzeman0a", "Ondřej", "Zeman", null, "987456254", "zeman@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 22).ToList().First();
            db.UserT.Add(UserAdd("xkolar05", "Ivan", "Kolář", null, "605258321", "kolar@seznam.cz", add));

            add = db.AddressT.Where(s => s.AddressId == 23).ToList().First();
            db.UserT.Add(UserAdd("xkrejc02", "Marie", "Krejčířová", "Bc.", "778555421", "krejcirova@seznam.cz", add));

            db.UserT.Add(UserAdd("xnavra00", "Josef", "Navrátil", "Bc.", "802369544", "krejcirova@seznam.cz", null));

            db.UserT.Add(UserAdd("xcerma0a", "Jakub", "Čermák", "Bc.", "705456258", "cermak@seznam.cz", null));

            db.UserT.Add(UserAdd("xbartos33", "Daniela", "Bartošová", null, "603258444", "bartosova@seznam.cz", null));

            db.UserT.Add(UserAdd("xsimek00", "Pavel", "Šimek", null, "706582111", "simek@seznam.cz", null));

            db.UserT.Add(UserAdd("xsykorka55", "Hana", "Sýkorová", "Mgr.", "704555021", "sykorova@seznam.cz", null));

            db.UserT.Add(UserAdd("xsouku0a", "Jana", "Soukupová", "Mgr.", "802369541", "soukupova@seznam.cz", null));

            db.UserT.Add(UserAdd("xmorav07", "Tereza", "Moravcová", "Ing.", "708596023", "moravcova@seznam.cz", null));

            db.UserT.Add(UserAdd("xsevci06", "Ladislav", "Ševčík", null, "705258452", "sevcik@seznam.cz", null));


            db.UserT.Add(UserAdd("xmasek00", "Karel", "Maseka", null, "809632025", "maseka@seznam.cz", null));


            db.UserT.Add(UserAdd("xhrusk08", "Pavlína", "Hrušková", null, "702555412", "hruskova@seznam.cz", null));


            add = db.AddressT.Where(s => s.AddressId == 25).ToList().First();
            db.UserT.Add(UserAdd("xpavli00", "Zbyněk", "Pavlíček", null, "706589789", "pavlicek@seznam.cz", add));


            add = db.AddressT.Where(s => s.AddressId == 28).ToList().First();
            db.UserT.Add(UserAdd("xjanda0a", "Daniel", "Janda", "Bc.", "737562205", "janda@seznam.cz", add));

            db.SaveChanges();
        }
        private User UserAdd(string login, string name, string surname, string title, string phone, string email, Address workaddress)
        {
            return new User { Login = login, Name = name, Surname = surname, Title = title, Phone = phone, Email = email, WorkAddress = workaddress };
        }
    }
}