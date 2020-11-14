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
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xmalic0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xbilik00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Admin");

            user = new NemocniceUser { UserName = "xjanec0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");
            await _userManager.AddToRoleAsync(user, "Insurance");

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
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xhajek0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xjelin00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xkrali06" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xbenes01" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xdolez00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Doctor");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xfiala01" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xsedla03" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xzeman0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xkolar05" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xkrejc02" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xnavra00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xcerma0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xbarto08" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xsimek00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xsykor01" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xsouku0a" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xmorav07" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xsevci06" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xmasek00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xhrusk08" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");

            user = new NemocniceUser { UserName = "xpavli00" };
            result = await _userManager.CreateAsync(user, "1234567890");
            await _userManager.AddToRoleAsync(user, "Patient");
            */

            return View();
        }
    }
}
