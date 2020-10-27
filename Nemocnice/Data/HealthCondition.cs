using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class HealthCondition
    {
        // rodné číslo
        [Key]
        public int HealthConditionId { get; set; }
        [Required]
<<<<<<< HEAD
        public int HealthConditionId { get; set; }
        [Required]
=======
>>>>>>> 1f99c343045d8f88e3676a1b024eb6503bbe5bac
        public long SocialSecurityNum { get; set; }
        public float Height { get; set; }
        public float Weight { get; set; }
        public string BloodType { get; set; }
        public DateTime LastCheckupDate { get; set; }
    }
}