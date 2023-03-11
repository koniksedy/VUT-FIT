/*
 * DoctorJoined1
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15, Kateřina Kunorzová <xkunor00>
 */
using Nemocnice.Data;

namespace Nemocnice.Models
{
    public class DoctorJoined1
    {
        public Data.Doctor Doctor { get; set; }
        public Data.User User { get; set; }

    }
}