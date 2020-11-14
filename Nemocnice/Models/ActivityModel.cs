using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class ActivityModel
    {
        public string ActivityName { get; set; }
        public decimal Value { get; set; }
        public long PatientNum { get; set; }
        public DateTime CreateDate { get; set; }
        public string State { get; set; }


    }
}
