using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Models
{
    public class CheckupTicketToMe
    {
        public string PatientSurname { get; set; }
        public string PatientName { get; set; }
        public string SocialSecurityNumber { get; set; }
        public string FromDoctor { get; set; }
        public string CreationDate { get; set; }
    }
}
