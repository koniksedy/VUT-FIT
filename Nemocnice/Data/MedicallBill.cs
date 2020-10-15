using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class MedicallBill
    {
        public int MedicallBillId { get; set; }
        [Required]
        public DateTime CreateDate { get; set; }

        public string State { get; set; }
        [Required]
        public string PersonalNumber { get; set; }
        [Required]
        public virtual List<MedicallActivityPrice> MedicallActivityPrice { get; set; }
        [Required]
        public virtual Doctor Doctor { get; set; }
        [Required]
        public virtual Diagnosis Diagnosis { get; set; }
    }
}