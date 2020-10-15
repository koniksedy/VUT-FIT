using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Patient
    {
        public int PatientId { get; set; }
        [Required]
        public int InsuranceCompany { get; set; }
        [Required]
        public Address HomeAddress { get; set; }
        public HealthCondition HealthCondition { get; set; }
    }
}