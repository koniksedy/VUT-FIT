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
        private readonly ILogger<ChangePasswordModel> _logger;

        public ChangeAddressModel(
            UserManager<NemocniceUser> userManager,
            SignInManager<NemocniceUser> signInManager,
            ILogger<ChangePasswordModel> logger)
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
            
            [Required]

            [StringLength(100, ErrorMessage = "Povinné pole čísla domu")]
            [Display(Name = "Current password")]
            public int HouseNumber { get; set; }

            [Required]
            [Display(Name = "New password")]
            [StringLength(100, ErrorMessage = "Povinné pole ulice")]
            public string StreetName { get; set; }

            [Display(Name = "Confirm new password")]
            [StringLength(100, ErrorMessage = "Povinné pole města domu")]
            public string City { get; set; }

            [Display(Name = "Confirm new password")]
            [StringLength(100, ErrorMessage = "Povinné pole PSČ")]
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





            if ( Input.HouseNumber > 0 && Input.ZIP > 0 ) { 
                uzivatel.WorkAddress.HouseNumber = Input.HouseNumber;
                uzivatel.WorkAddress.StreetName = Input.StreetName;
                uzivatel.WorkAddress.City = Input.City;
                uzivatel.WorkAddress.ZIP = Input.ZIP;
                db.SaveChanges();

            }
            else
            {
                StatusMessage = "Unexpected error when trying to set phone number.";
                return RedirectToPage();
            }
            await _signInManager.RefreshSignInAsync(user);
            _logger.LogInformation("User changed their password successfully.");
            StatusMessage = "Vaše adresa byla změněna.";
            return RedirectToPage();
        }
    }
}
