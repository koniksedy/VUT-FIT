using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class PatientCreator
    {
        public void PatientCreate(DatabaseContext db)
        {
            int ID = db.UserT.Where(s => s.Login.CurrentLogin == "xmalic0a").Select(x => x.UserId).ToList().First();
            HealthCondition health = db.HealthConditionT.Where(s => s.HealthConditionId == 1).ToList().First();
            Address add = db.AddressT.Where(s => s.AddressId == 3).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 6325412547, 111, add,health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xnovot00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 2).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 4).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 773173581, 201, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xjanec0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 3).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 5).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 123002147, 111, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xpospi0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 4).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 6).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 755608987, 201, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xhajek0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 5).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 7).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 203145258, 201, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xjelin00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 6).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 8).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 789456007, 111, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xkrali06").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 7).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 9).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 805233654, 211, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xbenes01").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 8).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 10).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 933605211, 211, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xdolez00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 9).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 11).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 773581250, 111, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xfiala01").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 10).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 12).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 805699888, 111, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xsedla03").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 11).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 13).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 737582609, 111, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xzeman0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 12).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 14).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 888666321, 207, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xkolar05").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 13).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 15).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 807456123, 207, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xkrejc02").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 14).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 16).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 902147456, 207, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xnavra00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 15).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 17).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 733654200, 209, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xcerma0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 16).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 18).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 811204569, 209, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xbarto08").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 17).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 19).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 755654002, 209, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xsimek00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 18).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 20).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 758987411, 205, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xsykor01").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 19).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 21).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 987200321, 205, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xsouku0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 20).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 22).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 888555222, 205, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xmorav07").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 21).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 23).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 401214455, 205, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xsevci06").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 22).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 24).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 737541970, 205, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xmasek00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 23).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 25).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 774147582, 111, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xhrusk08").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 24).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 26).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 852963005, 111, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xpavli00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 25).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 27).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 789588201, 111, add, health));

            ID = db.UserT.Where(s => s.Login.CurrentLogin == "xjanda0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.HealthConditionId == 26).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 28).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 788582301, 111, add, health));

            db.SaveChanges();
        }
        private Patient PatientAdd(int userId, long socialSecurityNum, int insuranceCompany, Address homeAddress, HealthCondition healthCondition)
        {
            return new Patient { UserId = userId, SocialSecurityNum = socialSecurityNum, InsuranceCompany = insuranceCompany, HomeAddress = homeAddress, HealthCondition = healthCondition };
        }
    }
}
