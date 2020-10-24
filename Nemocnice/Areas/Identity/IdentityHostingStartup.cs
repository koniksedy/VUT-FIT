using System;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Identity.UI;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Nemocnice.Data;

[assembly: HostingStartup(typeof(Nemocnice.Areas.Identity.IdentityHostingStartup))]
namespace Nemocnice.Areas.Identity
{
    public class IdentityHostingStartup : IHostingStartup
    {
        public void Configure(IWebHostBuilder builder)
        {
            builder.ConfigureServices((context, services) => {
                services.AddDbContext<DatabaseContext>(options =>
                    options.UseSqlServer(
                        context.Configuration.GetConnectionString("IISdb")));

                services.AddDefaultIdentity<NemocniceUser>(options => options.SignIn.RequireConfirmedAccount = true)
                    .AddRoles<IdentityRole>()
                    .AddEntityFrameworkStores<DatabaseContext>();
            });
        }
    }
}