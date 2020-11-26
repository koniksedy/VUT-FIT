/*
 * Model pro zobrazení Card (kartotéky)
 * Autor: Michal Šedý (xsedym02)
 * Poslední změna: 12.11.2020
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Nemocnice.Data;
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
    }
}
