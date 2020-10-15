using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    public class AllergysOfPatient
    {
        [Required]
        public int AllergysOfPatientId { get; set; }
        [Required]
        public Allergy Allergy { get; set; }
        [Required]
        public HealthCondition HealthCondition {get; set;}

    }
}