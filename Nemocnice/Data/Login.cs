using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Login
    {
        public int LoginId { get; set; }
        [Required]
        public string HashPassword { get; set; }
        [Required]
        public virtual List<Role> Roles { get; set; }
    }
}