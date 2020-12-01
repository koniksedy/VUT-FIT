using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using Nemocnice.Data;

namespace Nemocnice.Areas.Identity.Pages.Account.Manage
{
    public class ChangeAddressModel : PageModel
    {
        private readonly UserManager<NemocniceUser> _userManager;
        private readonly SignInManager<NemocniceUser> _signInManager;
        private readonly ILogger<ChangeAddressModel> _logger;

        public ChangeAddressModel(
            UserManager<NemocniceUser> userManager,
            SignInManager<NemocniceUser> signInManager,
            ILogger<ChangeAddressModel> logger)
        {
            _userManager = userManager;
            _signInManager = signInManager;
            _logger = logger;
        }

        [BindProperty]
        public InputModel Input { get; set; }

        [TempData]
        public string StatusMessage { get; set; }
        
        
        public class InputModel
        {

            [Display(Name = "Číslo domu")]
            [DefaultValue("0")]
            [RegularExpression("[0-9]{0,9}", ErrorMessage = "Neplatné číslo domu.")]
            public string HouseNumber { get; set; }


            [Display(Name = "Název ulice")]
            public string StreetName { get; set; }

            [Required(ErrorMessage = "Město je povinné.")]
            [Display(Name = "Město")]
            [StringLength(100, ErrorMessage = "Takhle dlouhlé město snad neexituje.")]
            public string City { get; set; }

            [Required(ErrorMessage = "PSČ je povinné")]
            [RegularExpression("([0-9]{5})|([0-9]{3} [0-9]{2})", ErrorMessage = "Neplatné PSČ.")]
            [Display(Name = "PSČ")]
            
            public int ZIP { get; set; }
        }

        private async Task LoadAsync(NemocniceUser user)
        {
            var userName = await _userManager.GetUserNameAsync(user);

            var db = new DatabaseContext();
            var uzivatel = db.UserT.Include(x => x.WorkAddress).Where(x => x.Login == userName).First();

            db.SaveChanges();
            if (User.IsInRole("Patient"))
            {
                var Patient = db.PatientT.Include(x => x.HomeAddress).Where(x => x.UserId == uzivatel.UserId).First();
                if (Patient.HomeAddress == null)
                {
                    Input = new InputModel
                    {
                        HouseNumber = "",
                        StreetName = "",
                        City = "",
                        ZIP = 0
                    };
                }
                else
                {
                    Input = new InputModel
                    {
                        HouseNumber = Patient.HomeAddress.HouseNumber.ToString() == "0" ? "" : Patient.HomeAddress.HouseNumber.ToString(),
                        StreetName = String.IsNullOrEmpty(Patient.HomeAddress.StreetName) ? "" : Patient.HomeAddress.StreetName,
                        City = Patient.HomeAddress.City,
                        ZIP = Patient.HomeAddress.ZIP
                    };
                }
            }
            else
            {
                if (uzivatel.WorkAddress == null)
                {
                    Input = new InputModel
                    {
                        HouseNumber = "",
                        StreetName = "",
                        City = "",
                        ZIP = 0
                    };
                }
                else
                {
                    Input = new InputModel
                    {
                        HouseNumber = uzivatel.WorkAddress.HouseNumber.ToString() == "0" ? "" : uzivatel.WorkAddress.HouseNumber.ToString(),
                        StreetName = String.IsNullOrEmpty(uzivatel.WorkAddress.StreetName) ? "" : uzivatel.WorkAddress.StreetName,
                        City = uzivatel.WorkAddress.City,
                        ZIP = uzivatel.WorkAddress.ZIP
                    };
                }
            }
        }

        public async Task<IActionResult> OnGetAsync()
        {
            var user = await _userManager.GetUserAsync(User);

            if (user == null)
            {
                return NotFound($"Unable to load user with ID '{_userManager.GetUserId(User)}'.");
            }

            await LoadAsync(user);

            return Page();
        }

        public async Task<IActionResult> OnPostAsync()
        {
            if (!ModelState.IsValid)
            {
                return Page();
            }

            var user = await _userManager.GetUserAsync(User);
            if (user == null)
            {
                return NotFound($"Unable to load user with ID '{_userManager.GetUserId(User)}'.");
            }
            var userName = await _userManager.GetUserNameAsync(user);
            var db = new DatabaseContext();
            var uzivatel = db.UserT.Include(x => x.WorkAddress).Where(x => x.Login == userName).First();


            // TODO kontrola na záporné PSČ
            if (User.IsInRole("Patient"))
            {
                var Patient = db.PatientT.Include(x => x.HomeAddress).Where(x => x.UserId == uzivatel.UserId).First();
                if (Patient.HomeAddress == null)
                {
                    Patient.HomeAddress = new Address();
                }

                Patient.HomeAddress.HouseNumber = int.Parse(String.IsNullOrEmpty(Input.HouseNumber) ? "0" : Input.HouseNumber);
                Patient.HomeAddress.StreetName = String.IsNullOrEmpty(Input.StreetName) ? "" : Input.StreetName;
                Patient.HomeAddress.City = Input.City;
                Patient.HomeAddress.ZIP = Input.ZIP;

            }
            else
            {
                if(uzivatel.WorkAddress == null)
                {
                    uzivatel.WorkAddress = new Address();
                }

                uzivatel.WorkAddress.HouseNumber = int.Parse(String.IsNullOrEmpty(Input.HouseNumber) ? "0" : Input.HouseNumber);
                uzivatel.WorkAddress.StreetName = String.IsNullOrEmpty(Input.StreetName) ? "" : Input.StreetName;
                uzivatel.WorkAddress.City = Input.City;
                uzivatel.WorkAddress.ZIP = Input.ZIP;

            }
            db.SaveChanges();

            await _signInManager.RefreshSignInAsync(user);
            _logger.LogInformation("Vaše adresa byla změněna.");
            StatusMessage = "Vaše adresa byla změněna.";
            return RedirectToPage();
        }
    }
}
