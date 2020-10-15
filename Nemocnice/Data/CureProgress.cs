using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class CureProgress
    {
        public int CureProgressId { get; set; }
        [Required]
        public MedicallReport MedicallReport { get; set; }
        [Required]
        public Diagnosis Diagnosis { get; set; }
        [Required]
        public string StateOfTreatment { get; set; }
    }
}