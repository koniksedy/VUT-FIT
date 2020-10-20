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
            db.LoginT.Add(LoginAdd("xlogin01","Heslo",1));
            db.LoginT.Add(LoginAdd("xlogin02", "Heslo1", 5));
            db.LoginT.Add(LoginAdd("xlogin03", "Heslo2", 3));
            db.LoginT.Add(LoginAdd("xlogin04", "Heslo3", 2));
            db.SaveChanges();
        }
        private Login LoginAdd(string currentLogin, string hashPassword, int roleId)
        {
            return new Login { CurrentLogin = currentLogin, HashPassword = hashPassword, RoleCode = roleId };
        }
    }
}
