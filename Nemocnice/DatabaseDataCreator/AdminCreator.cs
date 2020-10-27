using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class AdminCreator
    {
        public void AdminCreate(DatabaseContext db)
        {
            int ID = db.UserT.Where(s => s.Login.CurrentLogin == "xnovot00").Select(x => x.UserId).ToList().First();
            db.AdminT.Add(AdminAdd(ID, "Síťový administrátor","778586603", null));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xbilik00").Select(x => x.UserId).ToList().First();
            db.AdminT.Add(AdminAdd(ID, "Personální administrátor", "805258741", "People Company"));

            db.SaveChanges();
        }
        private Admin AdminAdd(int userId, string role, string workPhone, string outsorcecompany)
        {
            return new Admin { UserId = userId, Role = role, WorkPhone = workPhone, OutsorceCompany = outsorcecompany };
        }
    }
}
