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
                    Patient.HomeAddress = new Address();
                    db.SaveChanges();
                }
                                    
                Input = new InputModel
                {
                    HouseNumber = Patient.HomeAddress.HouseNumber.ToString() == "0" ? "" : Patient.HomeAddress.HouseNumber.ToString(),
                    StreetName = Patient.HomeAddress.StreetName == null ? "" : Patient.HomeAddress.StreetName,
                    City = Patient.HomeAddress.City == null ? "" : Patient.HomeAddress.City,
                    ZIP = Patient.HomeAddress.ZIP == null ? 0 : Patient.HomeAddress.ZIP
                };



            }
            else
            {
                if (uzivatel.WorkAddress != null)
                {
                    uzivatel.WorkAddress = new Address();
                    db.SaveChanges();
                }
                    Input = new InputModel
                    {
                        HouseNumber = uzivatel.WorkAddress.HouseNumber.ToString() == "0" ? "" : uzivatel.WorkAddress.HouseNumber.ToString(),
                        StreetName = uzivatel.WorkAddress.StreetName == null ? "" : uzivatel.WorkAddress.StreetName ,
                        City = String.IsNullOrEmpty(uzivatel.WorkAddress.City) ? "xxx" : uzivatel.WorkAddress.City ,
                        ZIP = uzivatel.WorkAddress.ZIP == 0 ? 0 : uzivatel.WorkAddress.ZIP
                    };
                
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
            var uzivatel = db.UserT.Where(x => x.Login == userName).Include(x => x.WorkAddress).First();




            if(uzivatel.WorkAddress == null)
            {
                uzivatel.WorkAddress = new Address();
            }
            
            try
            {


                if (int.Parse(Input.HouseNumber) > 0 && Input.ZIP > 0)
                {
                    if (User.IsInRole("Patient"))
                    {
                       
                        var Patient = db.PatientT.Include(x => x.HomeAddress).Where(x => x.UserId == uzivatel.UserId).First();
                        if (Patient.HomeAddress == null)
                        {
                            Patient.HomeAddress = new Address();
                        }
                        Patient.HomeAddress.HouseNumber = int.Parse(Input.HouseNumber);
                        Patient.HomeAddress.StreetName = Input.StreetName;
                        Patient.HomeAddress.City = Input.City;
                        Patient.HomeAddress.ZIP = Input.ZIP;

                    }
                    else
                    {
                        uzivatel.WorkAddress.HouseNumber = int.Parse(Input.HouseNumber);
                        uzivatel.WorkAddress.StreetName = Input.StreetName;
                        uzivatel.WorkAddress.City = Input.City;
                        uzivatel.WorkAddress.ZIP = Input.ZIP;
                    }

                }
                else
                {
                    StatusMessage = "Chyba při zadávání adresy";
                    return RedirectToPage();
                }
            }
            catch
            {

            }

            db.SaveChanges();
            await _signInManager.RefreshSignInAsync(user);
            _logger.LogInformation("Vaše adresa byla změněna.");
            StatusMessage = "Vaše adresa byla změněna.";
            return RedirectToPage();
        }
    }
}
