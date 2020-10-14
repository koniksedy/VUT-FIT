using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Data
{
    public class DatabaseContext : DbContext
    {

        public DatabaseContext() 
        { }

        private string connectionString = "Server=tcp:vut-fit-xpavla15.database.windows.net,1433;" +
                                          "Initial Catalog=IW5_eshop;Persist Security Info=False;" +
                                          "User ID=xpavla15;Password=Ondra1999;MultipleActiveResultSets=False;" +
                                          "Encrypt=True;TrustServerCertificate=False;Connection Timeout=30;";
        protected override void OnConfiguring(DbContextOptionsBuilder options)
        {
            options.UseSqlServer(connectionString, null);
        }

        public DbSet<Table1> Tables1 { get; set; }

        public DbSet<Table2> Tables2 { get; set; }

    }
}
