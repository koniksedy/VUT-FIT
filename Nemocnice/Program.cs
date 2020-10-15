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
            /*
             * TEST
             * new DatabaseContext() - umožòuje pracovat s databází
             */
           // using (var db = new DatabaseContext())
            //{
                /*
                 * Registrace nových tabulek do databáza. Pouze jednou pro tabulku.
                 * !!! Nejedná se o pøidávání øádku do tabulky, to se dìlá jinak.!!!
                 */
                //db.Add(new Table1 { ItemVal_Tab1 = 42 });
                //db.Add(new Table2 { ItemVal_Tab2 = 24 });
               // db.SaveChanges();

                // Test exitence hodnot v databázi.
              //  var a = db.Tables1.Where(s => s.Table1Id >= 1).ToList();
              //  var b = db.Tables2.Where(s => s.Table2Id >= 1).ToList();
          //  }

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
