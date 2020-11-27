using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Identity.UI.Services;
using Microsoft.AspNetCore.Identity.UI.V3.Pages.Account.Internal;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Nemocnice.Data;

namespace Nemocnice.Controllers
{
    public class LoginCreator : Controller
    {

        private readonly SignInManager<NemocniceUser> _signInManager;
        private readonly UserManager<NemocniceUser> _userManager;
        private readonly RoleManager<IdentityRole> _roleManager;
        private readonly ILogger<RegisterModel> _logger;
        private readonly IEmailSender _emailSender;

        public LoginCreator(
            UserManager<NemocniceUser> userManager,
            SignInManager<NemocniceUser> signInManager,
            RoleManager<IdentityRole> roleManager,
            ILogger<RegisterModel> logger,
            IEmailSender emailSender)
        {
            _userManager = userManager;
            _signInManager = signInManager;
            _roleManager = roleManager;
            _logger = logger;
            _emailSender = emailSender;
        }


        public IActionResult Index()
        {
            return View();
        }

        public async Task<IActionResult> CreateAsync()
        {

            bool x;
            x = await _roleManager.RoleExistsAsync("Admin");
            if (!x)
            {
                var role = new IdentityRole();
                role.Name = "Admin";
                await _roleManager.CreateAsync(role);
            }

            x = await _roleManager.RoleExistsAsync("Patient");
            if (!x)
            {
                var role = new IdentityRole();
                role.Name = "Patient";
                await _roleManager.CreateAsync(role);
            }

            x = await _roleManager.RoleExistsAsync("Doctor");
            if (!x)
            {
                var role = new IdentityRole();
                role.Name = "Doctor";
                await _roleManager.CreateAsync(role);
            }

            x = await _roleManager.RoleExistsAsync("Insurance");
            if (!x)
            {
                var role = new IdentityRole();
                role.Name = "Insurance";
                await _roleManager.CreateAsync(role);
            }

            /*
            var user = new NemocniceUser { UserName = "xnovot00" };
            var result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Admin");

            user = new NemocniceUser { UserName = "7204236798" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xmalic0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xbilik00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Admin");

            user = new NemocniceUser { UserName = "xjanec0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Insurance");

            user = new NemocniceUser { UserName = "9409278736" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xsvobo00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Insurance");

            user = new NemocniceUser { UserName = "xovale0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Insurance");

            user = new NemocniceUser { UserName = "xdvora0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "xcerny00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "xvesel0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "xhorak00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "xnemec08" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "xmarek05" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "xpospi0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "7306261116" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xhajek0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "5804249330" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xjelin00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "7310079755" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xkrali06" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "5609101014" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xbenes01" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "5407078259" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xdolez00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");

            user = new NemocniceUser { UserName = "7952217779" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "6454221719" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "6960249307" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "8661094849" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "7262204400" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "0254270401" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "0261203349" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "7561200394" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "7356020210" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "0110038379" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "7310190998" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "7705156437" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "0053229792" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "8862026096" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "7951257193" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "8052047377" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "6712254901" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "6261046044" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "Admin" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Admin");
            */
            
            return View();
        }
    }
}
