/*
 * NewCheckupModel
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15, Kateřina Kunorzová <xkunor00>
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class NewCheckupModel
    {
        public NameModel FullName { get; set; }
        public string SocialSecurityNum { get; set; }
        public List<DateTime> Reports { get; set; } = new List<DateTime>();
    }
}
