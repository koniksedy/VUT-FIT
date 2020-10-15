using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    public class Patient
    {
        // AdminId == UserId při generalizaci na více tabulek musí být Id shodné
        // s hlavní tabulkou.
        [Required]
        [Key]
        public int UserId { get; set; }
        [Required]
        public string SocialSecurityNum { get; set; }
        [Required]
        public int InsuranceCompany { get; set; }
        [Required]
        public Address HomeAddress { get; set; }
        public HealthCondition HealthCondition { get; set; }
    }
}