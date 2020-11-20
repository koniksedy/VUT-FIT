using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using X.PagedList;

namespace Nemocnice.Models
{
    public class PatientCardModel
    {
        public User User { get; set; }
        public Patient Patient { get; set; }
        public List<MedicallActivityPrice> medicallActivityPrice { get; set; }
        public IPagedList<MedicallReport> medicallReports { get; set; }
        public IPagedList<CheckupTicket> checkupTickets { get; set; }
        public IPagedList<Allergy> Allergies { get; set; }
        public IPagedList<MedicallBill> MedicallBills { get; set; }
        public IPagedList<PatientTreatmentLog> PatientTreatmentLogs { get; set; }
        public IPagedList<CureProgress> CureProgresses { get; set; }
        public int Records1 { get; set; }
        public int PageNum1 { get; set; }
        public int PageSize1 { get; set; }
        public int Records2 { get; set; }
        public int PageNum2 { get; set; }
        public int PageSize2 { get; set; }
        public int Records3 { get; set; }
        public int PageNum3 { get; set; }
        public int PageSize3 { get; set; }
        public int pagetoswap { get; set; }
        public int TabNumber { get; set; }
        public int PageNum4 { get; set; }
        public int PageNum5 { get; set; }
    }
}
