/*
 * Hlavn� t��da pro projekt do p�edm�tu IIS.
 * Autor: Michal �ed� <xsedym02>, Ond�ej Pavlack� <xpavla15>, Katka Kunorzov� <xkunor00>
 */
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



namespace Nemocnice
{
    public class Program
    {

        public static void Main(string[] args)
        {

            //DatabaseCreatorController Content = new DatabaseCreatorController();
            //Content.DatabaseContentCreate();
            
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
