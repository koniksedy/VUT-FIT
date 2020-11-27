/*
 * Model dat pro zobrazení Report (zprávy).
 * Autor: Michal Šedý <xsedym02>
 * Poslední úpravy: 12.11.2020
 */
using System;
using System.Collections.Generic;

namespace Nemocnice.Models
{
    /*
     * Model uchovává informace o:
     *     - Všech zprávách pacienta (datum)
     *     - Aktuální (upravované, nebo nové) zprávě
     *     - Předchozí zprávě (o jednu jednotku starší než je aktuální)
     *     - Údaje o pacientovi
     */
    public class PatientReportsModel
    {
        public NameModel PatientFullName { get; set; }
        public string SocialSecurityNumber { get; set; }
        public int Insurance { get; set; }
        public string PreviousReport { get; set; }
        public DateTime PreviousReportDate { get; set; }
        public string ActualReport { get; set; }
        public DateTime ActualReportDate { get; set; }
        public List<DateTime> AllTickets { get; set; } = new List<DateTime>();
        public List<DateTime> AllReports {get; set;} = new List<DateTime>();
        public List<PictureJsonModel> Pictures { get; set; } = new List<PictureJsonModel>();
    }
}
