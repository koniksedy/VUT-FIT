using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    public class Patient
    {
        // AdminId == UserId při generalizaci na více tabulek musí být Id shodné
        // s hlavní tabulkou.
        [Key]
        [Required]
        public int PatientID { get; set; }
        [Required]
        public int UserId { get; set; }
        [Required]
        public long SocialSecurityNum { get; set; }
        [Required]
        public int InsuranceCompany { get; set; }
        [Required]
        public Address HomeAddress { get; set; }
        public HealthCondition HealthCondition { get; set; }
    }
}