using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class InsureEmpCreator
    {
        public void InsureEmpCreate(DatabaseContext db)
        {
            int ID = db.UserT.Where(s => s.Login.CurrentLogin == "xjanec0a").Select(x => x.UserId).ToList().First();
            db.InsureEmpT.Add(InsureEmpAdd(ID,"Specialistka zákaznické péče", "737562021"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xsvobo00").Select(x => x.UserId).ToList().First();
            db.InsureEmpT.Add(InsureEmpAdd(ID, "Ředitel", "774541258"));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xovale0a").Select(x => x.UserId).ToList().First();
            db.InsureEmpT.Add(InsureEmpAdd(ID, "Likvidátorka pojistných událostí", "887586214"));
            db.SaveChanges();
        }
        private InsureEmp InsureEmpAdd(int userId, string possition, string workPhone)
        {
            return new InsureEmp { UserId = userId, Possition = possition, WorkPhone = workPhone };
        }
    }
}
