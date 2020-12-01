/*
 * DoctorListModel
 * Autor: Michal Šedý <xsedym02>, Ondřej Pavlacký <xpavla15, Kateřina Kunorzová <xkunor00>
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using X.PagedList;
using Nemocnice.Data;

namespace Nemocnice.Models
{
    public class DoctorListModel
    {
        public IPagedList<DoctorJoined> DoctorJoined { get; set; }
        public int pageNumber { get; set; }
    }
}

