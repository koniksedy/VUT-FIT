using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Nemocnice.Data;
using ReflectionIT.Mvc.Paging;

namespace Nemocnice.Models
{
    public class ActivityModel
    {
        public int Records { get; set; }
        public int PageNum { get; set; }
        public int PageSize { get; set; }
        public List<MedicallBill> medicallBills { get; set; }


    }
}
