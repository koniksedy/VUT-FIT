using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Nemocnice.Data;
using Nemocnice.DatabaseDataCreator;


/*
 * Pøidána ukázka pro práci s databází.
 * ------------------------------------
 * Úpravy:
 * 14.10.2020 - vytvoøení šablony - xsedym02
 */


namespace Nemocnice
{
    public class Program
    {

        public static void Main(string[] args)
        {

            //DatabaseCreatorController Content = new DatabaseCreatorController();
            //Content.DatabaseContentCreate();

            
            
            // Test exitence hodnot v databázi.
            //var a = db.Tables1.Where(s => s.Table1Id >= 1).ToList();
            //var b = db.Tables2.Where(s => s.Table2Id >= 1).ToList();*/
           
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
