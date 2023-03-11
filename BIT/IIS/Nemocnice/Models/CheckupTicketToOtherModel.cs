﻿/*
 * CheckupTicketToOtherModel
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15, Kateřina Kunorzová <xkunor00>
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class CheckupTicketToOtherModel
    {
        public string PatientName { get; set; }
        public string PatientSurname { get; set; }
        public string SocialSecurityNumber { get; set; }
        public string ToDoctor { get; set; }
        public DateTime CreationDate { get; set; }
        public bool State { get; set; }

        public string getDateAsString()
        {
            return CreationDate.ToString("dd.MM.yyyy");
        }
    }
}
