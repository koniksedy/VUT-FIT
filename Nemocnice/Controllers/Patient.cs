using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Nemocnice.Data;
using Nemocnice.Models;
using System.Linq;
using PagedList;
using ReflectionIT.Mvc.Paging;
using cloudscribe.Web.Pagination;
using cloudscribe.Pagination.Models;


namespace Nemocnice.Controllers
{
    [Authorize(Roles = "Patient")]
    public class Patient : Controller
    {
        public IActionResult Index()
        {
            return View();
        }

        public IActionResult PatientCard()
        {
            var db = new DatabaseContext();
            PatientCardModel PatientCardModel = new PatientCardModel();
            var user = User.Identity.Name;

            PatientCardModel.User = db.UserT.Where(x => x.Login == user).Include(s => s.WorkAddress).FirstOrDefault();
            PatientCardModel.Patient = db.PatientT.Where(x => x.UserId == PatientCardModel.User.UserId).Include(s => s.HomeAddress).Include(s => s.HealthCondition).FirstOrDefault();

            return View(PatientCardModel);
        }

        public async System.Threading.Tasks.Task<IActionResult> CheckupAsync(int pageNumber = 1, int pageSize = 3)
        {

            int ExcludeRecord = (pageSize * pageNumber) - pageSize;
            var db = new DatabaseContext();

            PatientCheckupModel PatientCheckupModel = new PatientCheckupModel();
            var user = User.Identity.Name;

            var query = db.CheckupTicketT.Where(x => x.State == "dokončeno").OrderByDescending(s => s.CreateDate);

            PatientCheckupModel.User = db.UserT.Where(x => x.Login == user).Include(s => s.WorkAddress).FirstOrDefault();
            PatientCheckupModel.Patient = db.PatientT.Where(x => x.UserId == PatientCheckupModel.User.UserId).Include(s => s.HomeAddress).Include(s => s.HealthCondition).FirstOrDefault();
            //PatientCheckupModel.CheckupTicketsDone = db.CheckupTicketT.Where(x => x.State == "dokončeno" ).ToList();
            PatientCheckupModel.CheckupTicketsRunning = db.CheckupTicketT.Where(x => x.State != "dokončeno").ToList();
            PatientCheckupModel.CheckupTicketsDone = db.CheckupTicketT.Where(x => x.State == "dokončeno").OrderByDescending(s => s.CreateDate).Skip(ExcludeRecord).Take(pageSize).ToList();

            var result = new PagedResult<CheckupTicket>
            {
                Data = db.CheckupTicketT.AsNoTracking().Where(x => x.State == "dokončeno").ToList(),
                TotalItems = PatientCheckupModel.CheckupTicketsDone.Count(),
                PageNumber = pageNumber,
                PageSize = pageSize

            };
            PatientCheckupModel.Result = result;

            return View(PatientCheckupModel);
        }

        public IActionResult Detail(int currentTicketID)
        {
            var db = new DatabaseContext();
            var ticket = db.CheckupTicketT.Where(x => x.CheckupTicketId == currentTicketID).Include(x => x.CreatedBy).Include(x => x.ToDoctor).Include(x => x.Patient).FirstOrDefault();
            return View(ticket);
        }
    }


}
