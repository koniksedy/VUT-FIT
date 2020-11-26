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
    public class AdminModel
    {
        public int PageNum { get; set; }
        public IPagedList<CardModel> patientsPage { get; set; }
        public List<CardModel> patients { get; set; }
    }



}
