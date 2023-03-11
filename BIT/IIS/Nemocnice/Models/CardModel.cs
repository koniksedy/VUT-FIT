/*
 * CardModel
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15, Kateřina Kunorzová <xkunor00>
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using X.PagedList;

namespace Nemocnice.Models
{
    /*
     * Model uchovává informace o pacientovi.
     */
    public class CardModel
    {
        public NameModel PatientFullName { get; set; }
        public string SocialSecurityNum { get; set; }
        public int Insurance { get; set; }
        public int PageNum { get; set; }
        public IPagedList<CardModel> patientsPage { get; set; }
        public List<CardModel> patients { get; set; }
    }
}
