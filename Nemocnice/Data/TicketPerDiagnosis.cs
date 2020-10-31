using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    public class TicketPerDiagnosis
    {
        [Required]
        public int TicketPerDiagnosisId { get; set; }
        [Required]
        public Diagnosis Diagnosis { get; set; }
        [Required]
        public CheckupTicket CheckupTicket { get; set; }
    }
}