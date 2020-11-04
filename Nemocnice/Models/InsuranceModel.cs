using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Nemocnice.Data;

namespace Nemocnice.Models
{
    public class InsuranceModel 
    {
        public int cena { get; set; }

        public string nazev { get; set; }
        public string nazevZmena { get; set; }
        public int cenaZmena { get; set; }

        public List<MedicallActivityPrice> medicallActivityPrice { get; set; }
    }
}
