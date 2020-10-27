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


        public async Task AddUserAsynchAsync(string name, string password, int R)
        {
            //var user = new NemocniceUser();
            //user.UserName = name;
            //user.Email = name;

            //string userPWD = password;

            var user = new NemocniceUser { UserName = name };
            var result = await _userManager.CreateAsync(user, "1234567890");


           // IdentityResult chkUser = await _userManager.CreateAsync(user, userPWD);

            /*
            IdentityResult result1, result2, result3, result4;
            if(R >> 3 == 1)
            {
                result1 = await _userManager.AddToRoleAsync(user, "Admin");

            }

            if(((R >> 2) & 1) == 1)
            {
                result2 = await _userManager.AddToRoleAsync(user, "Doctor");
            }

            if (((R >> 1) & 1) == 1)
            {
                result3 = await _userManager.AddToRoleAsync(user, "Patient");
            }

            if ((R & 1) == 1)
            {
                result4 = await _userManager.AddToRoleAsync(user, "Insurance");
            }

            */

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

            var res1 = AddUserAsynchAsync("xnovot00@kokot.cz", "LongPassword", 1010);
            //_ = AddUserAsynchAsync("xmalic0a", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xbilik00", "LongPassword", 1000);
            //_ = AddUserAsynchAsync("xjanec0a", "LongPassword", 0011);
            //_ = AddUserAsynchAsync("xsvobo00", "LongPassword", 0001);
            //_ = AddUserAsynchAsync("xovale0a", "LongPassword", 0001);
            //_ = AddUserAsynchAsync("xdvora0a", "LongPassword", 0100);
            //_ = AddUserAsynchAsync("xcerny00", "LongPassword", 0100);
            //_ = AddUserAsynchAsync("xvesel0a", "LongPassword", 0100);
            //_ = AddUserAsynchAsync("xhorak00", "LongPassword", 0100);
            //_ = AddUserAsynchAsync("xnemec08", "LongPassword", 0100);
            //_ = AddUserAsynchAsync("xmarek05", "LongPassword", 0100);
            //_ = AddUserAsynchAsync("xpospi0a", "LongPassword", 0110);
            //_ = AddUserAsynchAsync("xhajek0a", "LongPassword", 0110);
            //_ = AddUserAsynchAsync("xjelin00", "LongPassword", 0110);
            //_ = AddUserAsynchAsync("xkrali06", "LongPassword", 0110);
            //_ = AddUserAsynchAsync("xbenes01", "LongPassword", 0110);
            //_ = AddUserAsynchAsync("xdolez00", "LongPassword", 0110);
            //_ = AddUserAsynchAsync("xfiala01", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xsedla03", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xzeman0a", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xkolar05", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xkrejc02", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xnavra00", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xcerma0a", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xbarto08", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xsimek00", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xsykor01", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xsouku0a", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xmorav07", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xsevci06", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xmasek00", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xhrusk08", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xpavli00", "LongPassword", 0010);
            //_ = AddUserAsynchAsync("xjanda0a", "LongPassword", 0010);

            return View("Create");
        }
    }
}
