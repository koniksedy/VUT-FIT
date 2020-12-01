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
            int ID = db.UserT.Where(s => s.Login == "xmalic0a").Select(x => x.UserId).ToList().First();
            HealthCondition health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "8605274117").ToList().First();
            Address add = db.AddressT.Where(s => s.AddressId == 3).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "8605274117", 111, add,health));

            ID = db.UserT.Where(s => s.Login == "7204236798").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7204236798").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 4).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7204236798", 201, add, health));

            ID = db.UserT.Where(s => s.Login == "9409278736").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "9409278736").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 5).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "9409278736", 111, add, health));

            ID = db.UserT.Where(s => s.Login == "7306261116").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7306261116").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 6).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7306261116", 201, add, health));

            ID = db.UserT.Where(s => s.Login == "5804249330").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "5804249330").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 7).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "5804249330", 201, add, health));

            ID = db.UserT.Where(s => s.Login == "7310079755").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7310079755").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 8).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7310079755", 111, add, health));

            ID = db.UserT.Where(s => s.Login == "5609101014").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "5609101014").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 9).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "5609101014", 211, add, health));

            ID = db.UserT.Where(s => s.Login == "5407078259").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "5407078259").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 10).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "5407078259", 211, add, health));

            ID = db.UserT.Where(s => s.Login == "7952217779").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7952217779").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 11).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7952217779", 111, add, health));

            ID = db.UserT.Where(s => s.Login == "6454221719").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "6454221719").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 12).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "6454221719", 111, add, health));

            ID = db.UserT.Where(s => s.Login == "6960249307").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "6960249307").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 13).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "6960249307", 111, add, health));

            ID = db.UserT.Where(s => s.Login == "8661094849").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "8661094849").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 14).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "8661094849", 207, add, health));

            ID = db.UserT.Where(s => s.Login == "7262204400").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7262204400").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 15).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7262204400", 207, add, health));

            ID = db.UserT.Where(s => s.Login == "0254270401").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "0254270401").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 16).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "0254270401", 207, add, health));

            ID = db.UserT.Where(s => s.Login == "0261203349").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "0261203349").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 17).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "0261203349", 209, add, health));

            ID = db.UserT.Where(s => s.Login == "7561200394").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7561200394").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 18).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7561200394", 209, add, health));

            ID = db.UserT.Where(s => s.Login == "7356020210").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7356020210").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 19).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7356020210", 209, add, health));

            ID = db.UserT.Where(s => s.Login == "0110038379").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "0110038379").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 20).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "0110038379", 205, add, health));

            ID = db.UserT.Where(s => s.Login == "7310190998").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7310190998").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 21).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7310190998", 205, add, health));

            ID = db.UserT.Where(s => s.Login == "7705156437").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7705156437").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 22).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7705156437", 205, add, health));

            ID = db.UserT.Where(s => s.Login == "0053229792").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "0053229792").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 23).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "0053229792", 205, add, health));

            ID = db.UserT.Where(s => s.Login == "8862026096").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "8862026096").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 24).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "8862026096", 205, add, health));

            ID = db.UserT.Where(s => s.Login == "7951257193").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "7951257193").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 25).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "7951257193", 111, add, health));

            ID = db.UserT.Where(s => s.Login == "8052047377").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "8052047377").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 26).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "8052047377", 111, add, health));

            ID = db.UserT.Where(s => s.Login == "6712254901").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "6712254901").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 27).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "6712254901", 111, add, health));

            ID = db.UserT.Where(s => s.Login == "6261046044").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == "6261046044").ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 28).ToList().First();
            db.PatientT.Add(PatientAdd(ID, "5478512365", 111, add, health));

            db.SaveChanges();
        }
        private Patient PatientAdd(int userId, string socialSecurityNum, int insuranceCompany, Address homeAddress, HealthCondition healthCondition)
        {
            return new Patient { UserId = userId, SocialSecurityNum = socialSecurityNum, InsuranceCompany = insuranceCompany, HomeAddress = homeAddress, HealthCondition = healthCondition };
        }
    }
}
