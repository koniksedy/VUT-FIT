/*
 * PictureJsonModel
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15, Kateřina Kunorzová <xkunor00>
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class PictureJsonModel
    {
        public int id { get; set; }
        public string name { get; set; }
        public string date { get; set; }
        public string type { get; set; }
    }
}
