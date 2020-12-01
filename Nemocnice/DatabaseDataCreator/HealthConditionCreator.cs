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
            // TODO casy

            DateTime datumCas = new DateTime(2016, 7, 15, 3, 15, 0);
            db.HealthConditionT.Add(HealthConditionAdd("8605274117", 180, 82,"A+", datumCas));
            datumCas = new DateTime(2016, 7, 4, 13, 21, 0);
            db.HealthConditionT.Add(HealthConditionAdd("7204236798", 165, 60, "A+", datumCas));
            datumCas = new DateTime(2016, 1, 1, 14, 45, 5);
            db.HealthConditionT.Add(HealthConditionAdd("9409278736", 170, 70, "A+", datumCas));
            datumCas = new DateTime(2016, 2, 2, 10, 32, 15);
            db.HealthConditionT.Add(HealthConditionAdd("7306261116", 156, 55, "A+", datumCas));
            datumCas = new DateTime(2016, 3, 3, 11, 2, 17);
            db.HealthConditionT.Add(HealthConditionAdd("5804249330", 168, 63, "AB", datumCas));
            datumCas = new DateTime(2016, 4, 5, 12, 11, 19);
            db.HealthConditionT.Add(HealthConditionAdd("7310079755", 174, 70, "AB", datumCas));
            datumCas = new DateTime(2017, 5, 6, 13, 24, 20);
            db.HealthConditionT.Add(HealthConditionAdd("5609101014", 172, 72, "AB", datumCas));
            datumCas = new DateTime(2017, 6, 7, 14, 30, 23);
            db.HealthConditionT.Add(HealthConditionAdd("5407078259", 192, 86, "AB", datumCas));
            datumCas = new DateTime(2017, 7, 18, 15, 44, 35);
            db.HealthConditionT.Add(HealthConditionAdd("7952217779", 185, 90, "A-", datumCas));
            datumCas = new DateTime(2017, 8, 31, 12, 54, 30);
            db.HealthConditionT.Add(HealthConditionAdd("6454221719", 180, 88, "A-", datumCas));
            datumCas = new DateTime(2017, 8, 22, 8, 58, 39);
            db.HealthConditionT.Add(HealthConditionAdd("6960249307", 163, 59, "A-", datumCas));
            datumCas = new DateTime(2017, 9, 17, 7, 47, 40);
            db.HealthConditionT.Add(HealthConditionAdd("8661094849", 178, 75, "A-", datumCas));
            datumCas = new DateTime(2018, 10, 16, 9, 40, 42);
            db.HealthConditionT.Add(HealthConditionAdd("7262204400", 169, 65, "A-", datumCas));
            datumCas = new DateTime(2018, 11, 9, 10, 36, 46);
            db.HealthConditionT.Add(HealthConditionAdd("0254270401", 171, 69, "A-", datumCas));
            datumCas = new DateTime(2018, 12, 25, 11, 24, 48);
            db.HealthConditionT.Add(HealthConditionAdd("0261203349", 188, 95, "0", datumCas));
            datumCas = new DateTime(2018, 12, 21, 12, 21, 50);
            db.HealthConditionT.Add(HealthConditionAdd("7561200394", 184, 88, "0", datumCas));
            datumCas = new DateTime(2018, 1, 3, 12, 27, 52);
            db.HealthConditionT.Add(HealthConditionAdd("7356020210", 185, 89, "0", datumCas));
            datumCas = new DateTime(2018, 2, 9, 8, 19, 59);
            db.HealthConditionT.Add(HealthConditionAdd("0110038379", 186, 87, "0", datumCas));
            datumCas = new DateTime(2019, 2, 14, 7, 17, 0);
            db.HealthConditionT.Add(HealthConditionAdd("7310190998", 161, 61, "0", datumCas));
            datumCas = new DateTime(2019, 3, 30, 9, 12, 2);
            db.HealthConditionT.Add(HealthConditionAdd("7705156437", 162, 67, "0", datumCas));
            datumCas = new DateTime(2019, 4, 29, 10, 0, 4);
            db.HealthConditionT.Add(HealthConditionAdd("0053229792", 163, 60, "B+", datumCas));
            datumCas = new DateTime(2019, 5, 27, 11, 5, 8);
            db.HealthConditionT.Add(HealthConditionAdd("8862026096", 166, 65, "B+", datumCas));
            datumCas = new DateTime(2019, 6, 23, 11, 10, 16);
            db.HealthConditionT.Add(HealthConditionAdd("7951257193", 175, 74, "B+", datumCas));
            datumCas = new DateTime(2019, 7, 20, 12, 15, 32);
            db.HealthConditionT.Add(HealthConditionAdd("8052047377", 179, 78, "B+", datumCas));
            datumCas = new DateTime(2019, 7, 14, 13, 20, 48);
            db.HealthConditionT.Add(HealthConditionAdd("6712254901", 172, 70, "B+", datumCas));
            datumCas = default;
            db.HealthConditionT.Add(HealthConditionAdd("6261046044", 195, 96, "B-", datumCas));
            db.SaveChanges();
        }
        private HealthCondition HealthConditionAdd(string socialSecurityNum, int height, int weight, string bloodType, DateTime lastCheckupDate)
        {
            return new HealthCondition { SocialSecurityNum = socialSecurityNum, Height = height, Weight = weight, BloodType = bloodType, LastCheckupDate = lastCheckupDate };
        }
    }
}
