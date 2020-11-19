using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class PatientProfileModel
    {
        public NameModel PatientFullName { get; set; }
        public long SocialSecurityNumber { get; set; }
        public int InsuranceCompany { get; set; }
        public string Email { get; set; }
        public string Tel { get; set; }
        public string Age { get; set; }
        public Data.Address Address { get; set; } = new Data.Address();
        public Data.HealthCondition HealthCondition { get; set; } = new Data.HealthCondition();
        public List<string> Allergys { get; set; } = new List<string>();
        public List<DateTime> AllReports { get; set; } = new List<DateTime>();
        public List<CheckupToMeLightModel> CheckupToMe { get; set; } = new List<CheckupToMeLightModel>();
        public List<CheckupToOtherLightModle> CheckupToOther { get; set; } = new List<CheckupToOtherLightModle>();

    }
}
