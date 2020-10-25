using Microsoft.EntityFrameworkCore.Migrations.Operations;
using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class HealthConditionCreator
    {
        public void HealthConditionCreate(DatabaseContext db)
        {
            

            DateTime datumCas = new DateTime(2016, 7, 15, 3, 15, 0);
            db.HealthConditionT.Add(HealthConditionAdd(9863215564, 180, 82,"A+", datumCas));
            datumCas = new DateTime(2016, 7, 4, 13, 21, 0);
            db.HealthConditionT.Add(HealthConditionAdd(9552254188, 165, 60, "A+", datumCas));
            datumCas = new DateTime(2016, 1, 1, 14, 45, 5);
            db.HealthConditionT.Add(HealthConditionAdd(9638521235, 170, 70, "A+", datumCas));
            datumCas = new DateTime(2016, 2, 2, 10, 32, 15);
            db.HealthConditionT.Add(HealthConditionAdd(2568745120, 156, 55, "A+", datumCas));
            datumCas = new DateTime(2016, 3, 3, 11, 2, 17);
            db.HealthConditionT.Add(HealthConditionAdd(5698742103, 168, 63, "AB", datumCas));
            datumCas = new DateTime(2016, 4, 5, 12, 11, 19);
            db.HealthConditionT.Add(HealthConditionAdd(2356987452, 174, 70, "AB", datumCas));
            datumCas = new DateTime(2017, 5, 6, 13, 24, 20);
            db.HealthConditionT.Add(HealthConditionAdd(8562101478, 172, 72, "AB", datumCas));
            datumCas = new DateTime(2017, 6, 7, 14, 30, 23);
            db.HealthConditionT.Add(HealthConditionAdd(8963214568, 192, 86, "AB", datumCas));
            datumCas = new DateTime(2017, 7, 18, 15, 44, 35);
            db.HealthConditionT.Add(HealthConditionAdd(7458236985, 185, 90, "A-", datumCas));
            datumCas = new DateTime(2017, 8, 31, 12, 54, 30);
            db.HealthConditionT.Add(HealthConditionAdd(7412563201, 180, 88, "A-", datumCas));
            datumCas = new DateTime(2017, 8, 22, 8, 58, 39);
            db.HealthConditionT.Add(HealthConditionAdd(8745213698, 163, 59, "A-", datumCas));
            datumCas = new DateTime(2017, 9, 17, 7, 47, 40);
            db.HealthConditionT.Add(HealthConditionAdd(2014785630, 178, 75, "A-", datumCas));
            datumCas = new DateTime(2018, 10, 16, 9, 40, 42);
            db.HealthConditionT.Add(HealthConditionAdd(9852369874, 169, 65, "A-", datumCas));
            datumCas = new DateTime(2018, 11, 9, 10, 36, 46);
            db.HealthConditionT.Add(HealthConditionAdd(3562147852, 171, 69, "A-", datumCas));
            datumCas = new DateTime(2018, 12, 25, 11, 24, 48);
            db.HealthConditionT.Add(HealthConditionAdd(0214785236, 188, 95, "0", datumCas));
            datumCas = new DateTime(2018, 12, 21, 12, 21, 50);
            db.HealthConditionT.Add(HealthConditionAdd(9856321452, 184, 88, "0", datumCas));
            datumCas = new DateTime(2018, 1, 3, 12, 27, 52);
            db.HealthConditionT.Add(HealthConditionAdd(8520147852, 185, 89, "0", datumCas));
            datumCas = new DateTime(2018, 2, 9, 8, 19, 59);
            db.HealthConditionT.Add(HealthConditionAdd(4563217892, 186, 87, "0", datumCas));
            datumCas = new DateTime(2019, 2, 14, 7, 17, 0);
            db.HealthConditionT.Add(HealthConditionAdd(7523698214, 161, 61, "0", datumCas));
            datumCas = new DateTime(2019, 3, 30, 9, 12, 2);
            db.HealthConditionT.Add(HealthConditionAdd(9632541205, 162, 67, "0", datumCas));
            datumCas = new DateTime(2019, 4, 29, 10, 0, 4);
            db.HealthConditionT.Add(HealthConditionAdd(9321458745, 163, 60, "B+", datumCas));
            datumCas = new DateTime(2019, 5, 27, 11, 5, 8);
            db.HealthConditionT.Add(HealthConditionAdd(9236541258, 166, 65, "B+", datumCas));
            datumCas = new DateTime(2019, 6, 23, 11, 10, 16);
            db.HealthConditionT.Add(HealthConditionAdd(8745213698, 175, 74, "B+", datumCas));
            datumCas = new DateTime(2019, 7, 20, 12, 15, 32);
            db.HealthConditionT.Add(HealthConditionAdd(9032145698, 179, 78, "B+", datumCas));
            datumCas = new DateTime(2019, 7, 14, 13, 20, 48);
            db.HealthConditionT.Add(HealthConditionAdd(8523654124, 172, 70, "B+", datumCas));
            datumCas = new DateTime(2019, 8, 11, 17, 25, 42);
            db.HealthConditionT.Add(HealthConditionAdd(7632589654, 195, 96, "B-", datumCas));
            datumCas = new DateTime(2020, 9, 10, 15, 30, 33);
            db.HealthConditionT.Add(HealthConditionAdd(5478512365, 191, 89, "B-", datumCas));
            datumCas = new DateTime(2020, 10, 6, 16, 38, 44);
            db.HealthConditionT.Add(HealthConditionAdd(4521589632, 182, 80, "B-", datumCas));
            datumCas = new DateTime(2020, 9, 4, 14, 49, 55);
            db.HealthConditionT.Add(HealthConditionAdd(5236896541, 183, 82, "B-", datumCas));
            datumCas = new DateTime(2020, 5, 15, 13, 51, 12);
            db.HealthConditionT.Add(HealthConditionAdd(6325412547, 155, 54, "B-", datumCas));
            db.SaveChanges();
        }
        private HealthCondition HealthConditionAdd(long socialSecurityNum, float height, float weight, string bloodType, DateTime lastCheckupDate)
        {
            return new HealthCondition { SocialSecurityNum = socialSecurityNum, Height = height, Weight = weight, BloodType = bloodType, LastCheckupDate = lastCheckupDate };
        }
    }
}
