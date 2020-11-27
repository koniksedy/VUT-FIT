using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Admin
    {
        // AdminId == UserId při generalizaci na více tabulek musí být Id shodné
        // s hlavní tabulkou.
        [Key]
        [Required]
        public int AdminId { get; set; }
        [Required]
        public int UserId { get; set; }
        public string Role { get; set; }
        [Required]
        public string WorkPhone { get; set; }
        public string OutsorceCompany { get; set; }
    }
}