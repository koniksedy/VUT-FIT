using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Nemocnice.Data;
using Nemocnice.Models;

namespace Nemocnice.Controllers
{
    [Authorize(Roles = "Doctor")]
    public class Doctor : Controller
    {
        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Card()
        {
            var db = new DatabaseContext();

            List<CardPatientModel> Patients =
                db.PatientT.Join(
                    db.UserT,
                    patient => patient.PatientID,
                    user => user.UserId,
                    (patient, user) => new CardPatientModel
                        {
                            Surname = user.Surname,
                            Name = user.Name,
                            SocialSecurityNum = patient.SocialSecurityNum
                        }
                    ).ToList().OrderBy(o => o.Surname).ToList();

            return View(Patients);

        }
    }
}
