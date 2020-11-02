using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class CheckupTicketToMe
    {
        public string PatientSurname;
        public string PatientName;
        public long SocialSecurityNumber;
        public string FromDoctor;
        public DateTime CreationDate;
    }
}
