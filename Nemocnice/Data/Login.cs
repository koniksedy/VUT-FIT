using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Login
    {
        [Key]
        [Required]
        public string CurrentLogin { get; set; }
        [Required]
        public string HashPassword { get; set; }
        [Required]
        public int RoleCode { get; set; }
    }
}