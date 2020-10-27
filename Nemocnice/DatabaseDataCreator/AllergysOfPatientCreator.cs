using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class AllergysOfPatientCreator
    {
        public void AllergysOfPatientCreate(DatabaseContext db)
        {
            Allergy AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na kov").ToList().First();
            HealthCondition HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9638521235).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na zvířata (zejména kočky a psy)").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9638521235).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na kov").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 2568745120).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na plíseň").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8562101478).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na roztoče/prach").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8562101478).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na slunce").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8562101478).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na hmyzí bodnutí").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 2356987452).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na plíseň").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8963214568).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na citrusy").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8963214568).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na citrusy").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 7458236985).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na citrusy").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 7412563201).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na ryby").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 7412563201).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na ryby").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8745213698).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na ryby").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 3562147852).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na trávy a seno").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 0214785236).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na trávy a seno").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9856321452).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na pyl").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9856321452).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na pyl").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8520147852).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na zvířata (zejména kočky a psy)").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8520147852).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na zvířata (zejména kočky a psy)").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 7523698214).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na zvířata (zejména kočky a psy)").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9632541205).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na jod").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9321458745).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na jod").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9236541258).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na lepek").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9236541258).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na slunce").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 9236541258).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na lepek").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8523654124).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na lepek").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 7632589654).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na mléko").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 6325412547).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na léky").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 6325412547).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na roztoče/prach").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 6325412547).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));

            AllergyId = db.AllergyT.Where(s => s.Name == "Alergie na roztoče/prach").ToList().First();
            HealthId = db.HealthConditionT.Where(s => s.SocialSecurityNum == 8523654124).ToList().First();
            db.AllergysOfPatientT.Add(AllergysOfPatientAdd(AllergyId, HealthId));





            db.SaveChanges();
        }
        private AllergysOfPatient AllergysOfPatientAdd(Allergy allergy, HealthCondition healthCondition)
        {
            return new AllergysOfPatient { Allergy = allergy, HealthCondition = healthCondition };
        }
    }
}
