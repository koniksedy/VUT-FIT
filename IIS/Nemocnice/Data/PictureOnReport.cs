using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Data
{
    public class PictureOnReport
    {
        public int PictureOnReportId { get; set; }
        [Required]
        public Picture Picture { get; set; }
        [Required]
        public MedicallReport Report { get; set; }
    }
}
