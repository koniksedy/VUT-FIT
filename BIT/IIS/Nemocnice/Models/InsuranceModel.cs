/*
 * Soubor modelu pro uživatelskou část pracovník pojišťovny
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
    public class InsuranceModel 
    {
        public int cena { get; set; }

        public string nazev { get; set; }
        public string nazevZmena { get; set; }
        public int cenaZmena { get; set; }
        public int PageNum2 { get; set; }
        public int PageNum1 { get; set; }
        public int PageNum { get; set; }
        public IPagedList<MedicallBill> medicallBillsPage { get; set; }
        public IPagedList<MedicallActivityPrice> medicallActivityPricePage { get; set; }
        public List<MedicallActivityPrice> medicallActivityPrice { get; set; }
        public List<MedicallBill> medicallBills { get; set; }
        public IPagedList<Diagnosis> diagnosisPage { get; set; }
        public List<Diagnosis> diagnosis { get; set; }
    }



}
