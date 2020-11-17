using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class CheckupInModel
    {
        public string PatientName { get; set; }
        public string PatienSurname { get; set; }
        public string PatientTitle { get; set; }
        public long SocialSecurityNumber { get; set; }
        public string DoctorName { get; set; }
        public string DoctorSurname { get; set; }
        public string DoctorTitle { get; set; }
        public int DoctorICZ { get; set; }
        public string State { get; set; }
        public DateTime Date { get; set; }
        public string RequestText { get; set; }
        public string ReportText { get; set; }
    }
}
