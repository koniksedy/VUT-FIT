using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class CheckupTicketModel
    {
        public NameModel PatientFullName { get; set; }
        public string SocialSecurityNumber { get; set; }
        public NameModel DoctorFullName { get; set; }
        public string State { get; set; }
        public DateTime CreateDate { get; set; }
        public DateTime FinishDate { get; set; }
        public string RequestText { get; set; }
        public string ReportText { get; set; }
        public List<DateTime> AllReports { get; set; } = new List<DateTime>();
    }
}
