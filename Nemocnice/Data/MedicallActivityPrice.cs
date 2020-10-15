using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class MedicallActivityPrice
    {
        public int MedicallActivityPriceId { get; set; }
        [Required]
        public string Name { get; set; }
        [Required]
        public decimal Amount { get; set; }
        //public virtual List<Diagnosis> Diagnoses { get; set; }

    }
}