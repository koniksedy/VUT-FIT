/*
 * Soubor modelu pro uživatelskou část Admin a správu pracovníků pojišťovny
 * Autor: Kateřina Kunorzová <xkunor00>
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
    public class Admin_insurance
    {
        public NameModel InsuranceFullName { get; set; }
        public string Position { get; set; }
        public int PageNum1 { get; set; }
        public int UserId { get; set; }
        public IPagedList<Admin_insurance> insurancePage { get; set; }
        public List<Admin_insurance> insurance { get; set; }
        public int PersonalID { get; set; }
        public string WorkPhone { get; set; }
        public string Login { get; set; }
    }
}
