using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class PatientCheckupModel
    {
        public User User { get; set; }
        public Patient Patient { get; set; }
        public List<CheckupTicket> CheckupTicketsDone {get;set;}
        public List<CheckupTicket> CheckupTicketsRunning { get; set; }
    }
}
