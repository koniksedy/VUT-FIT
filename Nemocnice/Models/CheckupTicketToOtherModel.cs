using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class CheckupTicketToOtherModel
    {
        public string PatientName { get; set; }
        public string PatientSurname { get; set; }
        public long SocialSecurityNumber { get; set; }
        public string ToDoctor { get; set; }
        public string CreationDate { get; set; }
        public bool State { get; set; }
    }
}
