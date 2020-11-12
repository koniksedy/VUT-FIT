using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class CardPatientModel
    {
        public string Name { get; set; }
        public string Surname { get; set; }
        public string Title { get; set; }
        public long SocialSecurityNum { get; set; }
    }
}
