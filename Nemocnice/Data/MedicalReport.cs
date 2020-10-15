using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class MedicallReport
    {
        public int MedicallReportId { get; set; }
        [Required]
        public DateTime CreateDate { get; set; }

        public string Description { get; set; }
        [Required]
        public Doctor Author { get; set; }
        [Required]
        public Patient Patient { get; set; }
        public virtual List<Picture> Img { get; set; }
        [Required]
        public virtual List<Text> AccessRights { get; set; }
    }
}