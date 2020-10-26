using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class HealthCondition
    {
        // rodné číslo
        [Key]
        [Required]
        public int HealthConditionId { get; set; }
        [Required]
        public long SocialSecurityNum { get; set; }
        public float Height { get; set; }
        public float Weight { get; set; }
        public string BloodType { get; set; }
        public DateTime LastCheckupDate { get; set; }
    }
}