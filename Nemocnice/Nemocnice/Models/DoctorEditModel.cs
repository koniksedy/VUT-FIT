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
    public class DoctorEditModel
    {
        public NameModel DoctorFullName { get; set; }
        public string SocialSecurityNum { get; set; }
        public int PageNum { get; set; }
        public IPagedList<DoctorJoined1> DoctorJoined { get; set; }
        public List<DoctorJoined1> doctors { get; set; }
    }
}
