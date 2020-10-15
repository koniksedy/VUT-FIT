using System;
using System.Collections.Generic;

namespace Nemocnice.Data
{
    public class HealthCondition
    {
        public int HealConditionId { get; set; }
        public float Height { get; set; }
        public float Weight { get; set; }
        public DateTime LastCheckupDate { get; set; }
        public string BloodType { get; set; }
        public virtual List<Allergy> Allergy { get; set; }
    }
}