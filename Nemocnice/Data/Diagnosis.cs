using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Diagnosis
    {
        public int DiagnosisId { get; set; }
        [Required]
        public string Name { get; set; }
       // public virtual List<CheckupTicket> CheckupTickets { get; set; }
       // public virtual List<MedicallActivityPrice> MedicallActivityPrice { get; set; }
    }
}