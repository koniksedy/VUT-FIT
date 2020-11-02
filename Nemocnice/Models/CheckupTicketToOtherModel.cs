using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class CheckupTicketToOtherModel
    {
        public string PatientName;
        public string PatientSurname;
        public long SocialSecurityNumber;
        public string ToDoctor;
        public DateTime CreationDate;
        public bool State;
    }
}
