/*
 * Soubor modelu pro uživatelskou část Doktor, konkrétně výkazy
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
    public class ActivityModel
    {
        public int PageNum { get; set; }
        public List<MedicallBill> medicallBills { get; set; }
        public IPagedList<MedicallBill> medicallBillsPage { get; set; }


    }
}
