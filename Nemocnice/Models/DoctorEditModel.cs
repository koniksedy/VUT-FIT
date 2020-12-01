/*
 * Soubor modelu pro uživatelskou část Admin a správu doktorů
 * Autor: Ondřej Pavlacký <xpavla15>
 * Poslední úprava: 1.12.2020
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Nemocnice.Data;
using X.PagedList;

namespace Nemocnice.Models
{
    public class DoctorEditModel
    {
        public NameModel DoctorFullName { get; set; }
        public string SocialSecurityNum { get; set; }
        public int PageNum { get; set; }
        public IPagedList<DoctorJoined1> DoctorJoined { get; set; }
        public List<DoctorJoined1> doctors { get; set; }
    }
}
