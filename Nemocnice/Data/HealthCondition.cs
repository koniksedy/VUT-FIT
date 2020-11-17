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
        public long SocialSecurityNum { get; set; }
        public float Height { get; set; }
        public float Weight { get; set; }
        public string BloodType { get; set; }
        public DateTime LastCheckupDate { get; set; }
    }
}