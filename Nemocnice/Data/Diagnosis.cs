using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Diagnosis
    {
        [Required]
        public int DiagnosisId { get; set; }
        [Required]
        public string Name { get; set; }

    }
}