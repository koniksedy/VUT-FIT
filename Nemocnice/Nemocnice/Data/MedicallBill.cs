using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    public class MedicallBill
    {
        [Required]
        public int MedicallBillId { get; set; }
        [Required]
        public virtual Doctor Doctor { get; set; }
        [Required]
        public string SocialSecurityNum { get; set; }
        [Required]
        public MedicallActivityPrice MedicallActivityPrice { get; set; }
        [Required]
        public virtual Diagnosis Diagnosis { get; set; }
        public string State { get; set; }
        [Required]
        public DateTime CreateDate { get; set; }
        public DateTime DecisionDate { get; set; }
    }
}