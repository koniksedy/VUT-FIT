using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    public class CureProgress
    {
        [Required]
        public int CureProgressId { get; set; }
        [Required]
        public MedicallReport MedicallReport { get; set; }
        [Required]
        public Diagnosis Diagnosis { get; set; }
        public string StateOfTreatment { get; set; }
    }
}