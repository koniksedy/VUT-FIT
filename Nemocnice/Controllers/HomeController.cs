using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Nemocnice.Models;
using Nemocnice.Data;
using Microsoft.EntityFrameworkCore.Internal;
using Microsoft.EntityFrameworkCore;
using X.PagedList;

namespace Nemocnice.Controllers
{
    public class HomeController : Controller
    {
        private readonly ILogger<HomeController> _logger;

        public HomeController(ILogger<HomeController> logger)
        {
            _logger = logger;
        }

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Privacy()
        {
            return View();
        }

        public IActionResult DoctorList(string sortOrder, DoctorListModel model, int? pageNumber)
        {
            var db = new DatabaseContext();
            int pageSize = 10;

            if (String.IsNullOrEmpty(sortOrder))
            {
                model.pageNumber = (pageNumber ?? 1);
                model.DoctorJoined = db.DoctorT.Join(db.UserT, doctor => doctor.UserId, user => user.UserId,
                (doctor, user) => new DoctorJoined
                {
                    Surname = user.Surname,
                    Name = user.Name,
                    Title = user.Title,
                    UserId = user.UserId,
                    ICZ = doctor.ICZ,
                    Email = user.Email,
                    Phone = doctor.WorkPhone,
                    WorkAddress = user.WorkAddress,
                }).ToPagedList(model.pageNumber, pageSize);
            }
            else
            {
                switch (sortOrder)
                {
                    case "byName":
                        model.DoctorJoined = model.DoctorJoined.OrderBy(o => o.Name).ToPagedList(model.pageNumber, pageSize);
                        break;
                    case "bySurname":
                        model.DoctorJoined = model.DoctorJoined.OrderBy(o => o.Surname).ToPagedList(model.pageNumber, pageSize);
                        break;
                    case "byNumber":
                        model.DoctorJoined = model.DoctorJoined.OrderBy(o => o.ICZ).ToPagedList(model.pageNumber, pageSize);
                        break;
                    default:
                        model.DoctorJoined = model.DoctorJoined.ToPagedList(model.pageNumber, pageSize);
                        break;
                }
            }


            return View(model);
        }

        public IActionResult DoctorDetail(DoctorJoined item)
        {

            return View(item);
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
