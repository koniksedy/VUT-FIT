using System;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class PatientTreatmentLog
    {
        public int PatientTreatmentLogId { get; set; }
        [Required]
        public Patient Patient { get; set; }
        [Required]
        public Diagnosis Diagnosis { get; set; }
        [Required]
        public DateTime StartOfTreatment { get; set; }
        public DateTime EndOfTreatment { get; set; }
    }
}