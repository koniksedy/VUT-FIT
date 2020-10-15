using System;
using System.Collections.Generic;

namespace Nemocnice.Data
{
    public class HealthCondition
    {

        public int HealthConditionId { get; set; }
        public float Height { get; set; }
        public float Weight { get; set; }
        public DateTime LastCheckupDate { get; set; }
        public string BloodType { get; set; }
       // public ICollection<Allergy> Allergy { get; set; }
    }
}