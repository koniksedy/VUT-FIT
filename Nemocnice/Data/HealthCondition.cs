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
        public int SocialSecurityNum { get; set; }
        public float Height { get; set; }
        public float Weight { get; set; }
        public string BloodType { get; set; }
        public DateTime LastCheckupDate { get; set; }
    }
}