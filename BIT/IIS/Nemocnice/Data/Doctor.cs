using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Doctor
    {
        // AdminId == UserId při generalizaci na více tabulek musí být Id shodné
        // s hlavní tabulkou.
        [Required]
        public int DoctorId { get; set; }
        [Required]
        public int ICZ { get; set; }
        [Required]
        public int UserId { get; set; }
        [Required]
        public string WorkPhone { get; set; }
    }
}