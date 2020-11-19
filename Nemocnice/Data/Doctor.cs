using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Doctor
    {
        // AdminId == UserId při generalizaci na více tabulek musí být Id shodné
        // s hlavní tabulkou.
        // TODO ICZ přesunout na element a generovat interní privátní klíč ?
        [Key]
        [Required]
        public int ICZ { get; set; }
        [Required]
        public int UserId { get; set; }
        [Required]
        public string WorkPhone { get; set; }
    }
}