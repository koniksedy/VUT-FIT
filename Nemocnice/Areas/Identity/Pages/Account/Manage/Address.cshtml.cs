using System;
using System.Collections.Generic;
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
            public int HouseNumber { get; set; }


            [Display(Name = "Název ulice")]
            public string StreetName { get; set; }

            [Required]
            [Display(Name = "Město")]
            [StringLength(100, ErrorMessage = "Povinné pole města domu")]
            public string City { get; set; }

            [Required]
            [Display(Name = "PSČ")]
            
            public int ZIP { get; set; }
        }

        private async Task LoadAsync(NemocniceUser user)
        {
            var userName = await _userManager.GetUserNameAsync(user);

            var db = new DatabaseContext();
            var uzivatel = db.UserT.Where(x => x.Login == userName).Include(x => x.WorkAddress).First();

            db.SaveChanges();
            if(uzivatel.WorkAddress != null)
            {
                Input = new InputModel
                {
                    HouseNumber = uzivatel.WorkAddress.HouseNumber,
                    StreetName = uzivatel.WorkAddress.StreetName ?? String.Empty,
                    City = uzivatel.WorkAddress.City ?? String.Empty,
                    ZIP = uzivatel.WorkAddress.ZIP
                };
            }


        }

        public async Task<IActionResult> OnGetAsync()
        {
            var user = await _userManager.GetUserAsync(User);
            var userTypePatient = false;
            if (await _userManager.IsInRoleAsync(user, "Patient"))
            {
                userTypePatient = true;
            }
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


                if (Input.HouseNumber > 0 && Input.ZIP > 0)
                {
                    uzivatel.WorkAddress.HouseNumber = Input.HouseNumber;
                    uzivatel.WorkAddress.StreetName = Input.StreetName;


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
            uzivatel.WorkAddress.City = Input.City;
            uzivatel.WorkAddress.ZIP = Input.ZIP;
            db.SaveChanges();
            await _signInManager.RefreshSignInAsync(user);
            _logger.LogInformation("User changed their password successfully.");
            StatusMessage = "Vaše adresa byla změněna.";
            return RedirectToPage();
        }
    }
}
