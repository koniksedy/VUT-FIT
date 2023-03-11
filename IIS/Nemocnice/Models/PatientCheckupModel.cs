/*
 * PatientCheckupModel
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15, Kateřina Kunorzová <xkunor00>
 */
using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using X.PagedList;


namespace Nemocnice.Models
{
    public class PatientCheckupModel
    {
        public User User { get; set; }
        public Patient Patient { get; set; }
        public IPagedList<CheckupTicket> CheckupTicketsDone {get;set;}
        public IPagedList<CheckupTicket> CheckupTicketsRunning { get; set; }
        public int tab1 { get; set; }
        public int tab2 { get; set; }
    }
}
