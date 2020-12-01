using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class CheckupTicketToMeFinish
    {
        public string PatientSurname { get; set; }
        public string PatientName { get; set; }
        public string SocialSecurityNumber { get; set; }
        public string FromDoctor { get; set; }
        public DateTime CreationDate { get; set; }
        public string FinishDate { get; set; }

        public string GetCreationDateAsString()
        {
            return CreationDate.ToString("dd.MM.yyyy");
        }
    }
}
