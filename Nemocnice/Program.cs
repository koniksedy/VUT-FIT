using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Mvc.ViewFeatures.Buffers;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Nemocnice.Data;

namespace Nemocnice
{
    public class Program
    {

        public static void Main(string[] args)
        {
            // TEST
            using (var db = new DatabaseContext())
            {
                db.Add(new Table1 { ItemVal_Tab1 = 42 });
                db.Add(new Table2 { ItemVal_Tab2 = 24 });
                db.SaveChanges();

                var a = db.Tables1.Where(s => s.Table1Id >= 1).ToList();
                var b = db.Tables2.Where(s => s.Table2Id >= 1).ToList();
            }

            CreateHostBuilder(args).Build().Run();
        }

        public static IHostBuilder CreateHostBuilder(string[] args) =>
            Host.CreateDefaultBuilder(args)
                .ConfigureWebHostDefaults(webBuilder =>
                {
                    webBuilder.UseStartup<Startup>();
                });
    }
}
