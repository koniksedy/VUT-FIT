using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    public class ActivityPricingPerDiagnosis
    {   
        [Required]
        public int ActivityPricingPerDiagnosisId { get; set; }
        [Required]
        public Diagnosis Diagnosis { get; set; }
        [Required]
        public MedicallActivityPrice MedicallActivityPrice { get; set; }
    }
}