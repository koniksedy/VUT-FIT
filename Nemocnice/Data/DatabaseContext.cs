using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

/*
 * Jedná se o kontext databáze, který obsahuje všechny tabulky.
 * Pro reprezentaci tabulek použijte DbSet<TableT> Table.
 * ------------------------------------------------------------
 * Úpravy:
 * 14.10.2020 - vytvoření šablony - xsedym02
 */

namespace Nemocnice.Data
{
    public class DatabaseContext : DbContext
    {
        /*
         * Základní konstruktor.
         */
        public DatabaseContext() 
        { }

        /*
         * ConnectionString pro připojení k databázi Azure.
         * Snažil jsem se připojit databáhi pomocí :
         * 
         * 
         * services.AddDbContext<BloggingContext>(options =>
         *      options.UseSqlServer(Configuration.GetConnectionString("Development")));
         *
         * a
         * 
         * public BloggingContext(DbContextOptions<BloggingContext> options) : base(options)
         *   { }
         *   
         * ale marně. Nechte to jak to je!!!
         */
        private string connectionString = "Server=tcp:vut-fit-xpavla15.database.windows.net,1433;" +
                                          "Initial Catalog=IW5_eshop;Persist Security Info=False;" +
                                          "User ID=xpavla15;Password=Ondra1999;MultipleActiveResultSets=False;" +
                                          "Encrypt=True;TrustServerCertificate=False;Connection Timeout=30;";
        /*
         * Připojení k Azure databázi
         */
        protected override void OnConfiguring(DbContextOptionsBuilder options)
        {
            options.UseSqlServer(connectionString, null);
        }

        // Tabulka1 v databázi
        public DbSet<Table1> Tables1 { get; set; }

        // Tabulka2 v databázi
        public DbSet<Table2> Tables2 { get; set; }

    }
}
