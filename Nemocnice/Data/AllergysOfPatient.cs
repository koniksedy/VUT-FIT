using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    // TODO - smazat, pacient bude mít alergie ručně psané do stringu
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