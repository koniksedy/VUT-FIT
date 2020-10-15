using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Allergy
    {


        public int AllergyId { get; set; }
        [Required]
        public string Name { get; set; }
        [Required]
        public string Description { get; set; }
        public virtual List<Picture> Img { get; set; }
       // public ICollection<HealthCondition> HealthCondition { get; set; } 
    }
}