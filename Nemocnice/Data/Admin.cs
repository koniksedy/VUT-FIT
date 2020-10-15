using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Admin
    {

        public int AdminId { get; set; }
        [Required]
        public virtual List<Role> Roles { get; set; }
        [Required]
        public string OutsorceCompany { get; set; }
        [Required]
        public string WorkPhone { get; set; }
    }
}