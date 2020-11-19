using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class NewCheckupModel
    {
        public NameModel FullName { get; set; }
        public long SocialSecurityNum { get; set; }
    }
}
