using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class LoginCreator
    {
        public void LoginCreate(DatabaseContext db)
        {
            db.LoginT.Add(LoginAdd("xnovot00", "Heslo4", 1010));
            db.LoginT.Add(LoginAdd("xmalic0a", "Heslo5", 0010));
            db.LoginT.Add(LoginAdd("xbilik00", "Heslo5", 1000));
            db.LoginT.Add(LoginAdd("xjanec0a", "Heslo6", 0011));
            db.LoginT.Add(LoginAdd("xsvobo00", "Heslo7", 0001));
            db.LoginT.Add(LoginAdd("xovale0a", "Heslo8", 0001));
            db.LoginT.Add(LoginAdd("xdvora0a", "Heslo9", 0100));
            db.LoginT.Add(LoginAdd("xcerny00", "Heslo10", 0100));
            db.LoginT.Add(LoginAdd("xvesel0a", "Heslo11", 0100));
            db.LoginT.Add(LoginAdd("xhorak00", "Heslo12", 0100));
            db.LoginT.Add(LoginAdd("xnemec08", "Heslo13", 0100));
            db.LoginT.Add(LoginAdd("xmarek05", "Heslo14", 0100));
            db.LoginT.Add(LoginAdd("xpospi0a", "Heslo15", 0110));
            db.LoginT.Add(LoginAdd("xhajek0a", "Heslo16", 0110));
            db.LoginT.Add(LoginAdd("xjelin00", "Heslo17", 0110));
            db.LoginT.Add(LoginAdd("xkrali06", "Heslo18", 0110));
            db.LoginT.Add(LoginAdd("xbenes01", "Heslo19", 0110));
            db.LoginT.Add(LoginAdd("xdolez00", "Heslo20", 0110));
            db.LoginT.Add(LoginAdd("xfiala01", "Heslo21", 0010));
            db.LoginT.Add(LoginAdd("xsedla03", "Heslo22", 0010));
            db.LoginT.Add(LoginAdd("xzeman0a", "Heslo23", 0010));
            db.LoginT.Add(LoginAdd("xkolar05", "Heslo24", 0010));
            db.LoginT.Add(LoginAdd("xkrejc02", "Heslo25", 0010));
            db.LoginT.Add(LoginAdd("xnavra00", "Heslo26", 0010));
            db.LoginT.Add(LoginAdd("xcerma0a", "Heslo27", 0010));
            db.LoginT.Add(LoginAdd("xbarto08", "Heslo28", 0010));
            db.LoginT.Add(LoginAdd("xsimek00", "Heslo29", 0010));
            db.LoginT.Add(LoginAdd("xsykor01", "Heslo30", 0010));
            db.LoginT.Add(LoginAdd("xsouku0a", "Heslo31", 0010));
            db.LoginT.Add(LoginAdd("xmorav07", "Heslo32", 0010));
            db.LoginT.Add(LoginAdd("xsevci06", "Heslo32", 0010));
            db.LoginT.Add(LoginAdd("xmasek00", "Heslo32", 0010));
            db.LoginT.Add(LoginAdd("xhrusk08", "Heslo32", 0010));
            db.LoginT.Add(LoginAdd("xpavli00", "Heslo32", 0010));
            db.LoginT.Add(LoginAdd("xjanda0a", "Heslo32", 0010));
            db.SaveChanges();
        }
        private Login LoginAdd(string currentLogin, string hashPassword, int roleId)
        {
            return new Login { CurrentLogin = currentLogin, HashPassword = hashPassword, RoleCode = roleId };
        }
    }
}
