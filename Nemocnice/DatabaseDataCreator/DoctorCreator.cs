using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class DoctorCreator
    {
        public void DoctorCreate(DatabaseContext db)
        {
            int ID = db.UserT.Where(s => s.Login.CurrentLogin == "xdvora0a").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "789456258"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xcerny00").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "702564147"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xvesel0a").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "737852456"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xhorak00").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "908789456"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xnemec08").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "852741960"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xmarek05").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "123456780"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xpospi0a").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "896541258"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xhajek0a").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "302147785"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xjelin00").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "236587458"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xkrali06").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "704541258"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xbenes01").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "963214001"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xdolez00").Select(x => x.UserId).ToList().First();
            db.DoctorT.Add(DoctorAdd(ID, "789321002"));

            db.SaveChanges();
        }
        private Doctor DoctorAdd(int userId, string workPhone)
        {
            return new Doctor { UserId = userId, WorkPhone = workPhone };
        }
    }
}
