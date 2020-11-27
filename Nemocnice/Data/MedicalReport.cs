using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Diagnostics.CodeAnalysis;

namespace Nemocnice.Data
{
    public class MedicallReport
    {
        [Required]
        public int MedicallReportId { get; set; }
        [Required]
        public Doctor Author { get; set; }
        [Required]
        public Patient Patient { get; set; }
        public string Description { get; set; }
        [DefaultValue(false)]
        public bool IncludePic { get; set; }
        public Doctor Owner { get; set; }
        [Required]
        public DateTime CreateDate { get; set; }
    }
}