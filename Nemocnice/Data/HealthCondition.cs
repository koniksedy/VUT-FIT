using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    // TODO - weight na int
    // TODO - height na int
    public class HealthCondition
    {
        // rodné číslo
        [Key]
        public int HealthConditionId { get; set; }
        [Required]
        public string SocialSecurityNum { get; set; }
        public int Height { get; set; }
        public int Weight { get; set; }
        public string BloodType { get; set; }
        public DateTime LastCheckupDate { get; set; }
        public string Allergys { get; set; }

        public string GetHeightString()
        {
            return (int)Height == 0 ? "---" : Height.ToString();
        }

        public string GetWeightString()
        {
            return (int)Weight == 0 ? "---" : Weight.ToString();
        }

        public string GetBlodTypeString()
        {
            return String.IsNullOrEmpty(BloodType) ? "---" : BloodType;
        }
    }
}