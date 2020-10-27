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

            HealthCondition health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 6325412547).ToList().First();

            Address add = db.AddressT.Where(s => s.AddressId == 3).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 6325412547, 111, add,health));

            ID = db.UserT.Where(s => s.Login == "xnovot00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9638521235).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 4).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 9638521235, 201, add, health));

            ID = db.UserT.Where(s => s.Login == "xjanec0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 2568745120).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 5).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 2568745120, 111, add, health));

            ID = db.UserT.Where(s => s.Login == "xpospi0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 5698742103).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 6).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 5698742103, 201, add, health));

            ID = db.UserT.Where(s => s.Login == "xhajek0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 2356987452).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 7).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 2356987452, 201, add, health));

            ID = db.UserT.Where(s => s.Login == "xjelin00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8562101478).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 8).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 8562101478, 111, add, health));

            ID = db.UserT.Where(s => s.Login == "xkrali06").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8963214568).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 9).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 8963214568, 211, add, health));

            ID = db.UserT.Where(s => s.Login == "xbenes01").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 7458236985).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 10).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 7458236985, 211, add, health));

            ID = db.UserT.Where(s => s.Login == "xdolez00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 7412563201).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 11).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 7412563201, 111, add, health));

            ID = db.UserT.Where(s => s.Login == "xfiala01").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8745213698).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 12).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 8745213698, 111, add, health));

            ID = db.UserT.Where(s => s.Login == "xsedla03").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 2014785630).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 13).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 2014785630, 111, add, health));

            ID = db.UserT.Where(s => s.Login == "xzeman0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9852369874).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 14).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 9852369874, 207, add, health));

            ID = db.UserT.Where(s => s.Login == "xkolar05").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 3562147852).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 15).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 3562147852, 207, add, health));

            ID = db.UserT.Where(s => s.Login == "xkrejc02").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 214785236).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 16).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 214785236, 207, add, health));

            ID = db.UserT.Where(s => s.Login == "xnavra00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9856321452).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 17).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 9856321452, 209, add, health));

            ID = db.UserT.Where(s => s.Login == "xcerma0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8520147852).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 18).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 8520147852, 209, add, health));

            ID = db.UserT.Where(s => s.Login == "xbarto08").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 4563217892).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 19).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 4563217892, 209, add, health));

            ID = db.UserT.Where(s => s.Login == "xsimek00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 7523698214).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 20).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 7523698214, 205, add, health));

            ID = db.UserT.Where(s => s.Login == "xsykor01").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9632541205).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 21).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 9632541205, 205, add, health));

            ID = db.UserT.Where(s => s.Login == "xsouku0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9321458745).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 22).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 9321458745, 205, add, health));

            ID = db.UserT.Where(s => s.Login == "xmorav07").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9236541258).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 23).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 9236541258, 205, add, health));

            ID = db.UserT.Where(s => s.Login == "xsevci06").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8745213698).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 24).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 8745213698, 205, add, health));

            ID = db.UserT.Where(s => s.Login == "xmasek00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9032145698).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 25).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 9032145698, 111, add, health));

            ID = db.UserT.Where(s => s.Login == "xhrusk08").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8523654124).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 26).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 8523654124, 111, add, health));

            ID = db.UserT.Where(s => s.Login == "xpavli00").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 7632589654).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 27).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 7632589654, 111, add, health));

            ID = db.UserT.Where(s => s.Login == "xjanda0a").Select(x => x.UserId).ToList().First();
            health = db.HealthConditionT.Where(s => s.SocialSecurityNum == 5478512365).ToList().First();
            add = db.AddressT.Where(s => s.AddressId == 28).ToList().First();
            db.PatientT.Add(PatientAdd(ID, 5478512365, 111, add, health));

            db.SaveChanges();
        }
        private Patient PatientAdd(int userId, long socialSecurityNum, int insuranceCompany, Address homeAddress, HealthCondition healthCondition)
        {
            return new Patient { UserId = userId, SocialSecurityNum = socialSecurityNum, InsuranceCompany = insuranceCompany, HomeAddress = homeAddress, HealthCondition = healthCondition };
        }
    }
}
