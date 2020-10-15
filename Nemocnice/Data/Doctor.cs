using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Doctor
    {
        public int DoctorId { get; set; }
        [Required]
        public string ICZ { get; set; }
        [Required]
        public string WorkPhone { get; set; }
    }
}