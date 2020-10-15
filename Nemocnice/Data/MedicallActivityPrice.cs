using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class MedicallActivityPrice
    {
        [Required]
        public int MedicallActivityPriceId { get; set; }
        [Required]
        public string Name { get; set; }
        [Required]
        public decimal Amount { get; set; }
    }
}