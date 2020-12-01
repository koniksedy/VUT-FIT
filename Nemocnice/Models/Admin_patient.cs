/*
 * Soubor modelu pro uživatelskou část Admin a správu pacientů
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
    public class Admin_patient
    {
        public NameModel PatientFullName { get; set; }
        public string SocialSecurityNum { get; set; }
        public int Insurance { get; set; }
        public int UserId { get; set; }
        public int PageNum { get; set; }
        public IPagedList<Admin_patient> patientsPage { get; set; }
        public List<Admin_patient> patients { get; set; }
    }
}