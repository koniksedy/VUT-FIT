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
        public int PageNum { get; set; }
        public IPagedList<MedicallBill> medicallBillsPage { get; set; }
        public List<MedicallActivityPrice> medicallActivityPrice { get; set; }
        public List<MedicallBill> medicallBills { get; set; }
    }



}
