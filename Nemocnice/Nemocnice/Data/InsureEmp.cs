using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class InsureEmp
    {
        // AdminId == UserId při generalizaci na více tabulek musí být Id shodné
        // s hlavní tabulkou.
        [Key]
        [Required]
        public int PersonalId { get; set; }
        [Required]
        public int UserId { get; set; }
        public string Possition { get; set; }
        [Required]
        public string WorkPhone { get; set; }     
    }
}