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
        private string connectionString = "Server=tcp:xpavla15.database.windows.net,1433;" +
                                          "Initial Catalog=IIS_hospital;Persist Security Info=False;" +
                                          "User ID=xpavla15;Password=Ondra1999;MultipleActiveResultSets=False;" +
                                          "Encrypt=True;TrustServerCertificate=False;Connection Timeout=30;";
        /*
         * Připojení k Azure databázi
         */
        protected override void OnConfiguring(DbContextOptionsBuilder options)
        {
            options.UseSqlServer(connectionString, null);
        }

        public DbSet<User> UserT { get; set; }
        public DbSet<Address> AddressT { get; set; }
        public DbSet<HealthCondition> HealthConditionT { get; set; }
        public DbSet<Allergy> AllergyT { get; set; }
        public DbSet<MedicallBill> MedicallBillT { get; set; }
        public DbSet<CheckupTicket> CheckupTicketT { get; set; }
        public DbSet<MedicallReport> MedicallReportT { get; set; }
        public DbSet<MedicallActivityPrice> MedicallActivityPriceT { get; set; }
        public DbSet<Diagnosis> DiagnosisT { get; set; }
        public DbSet<Login> LoginT { get; set; }
        public DbSet<Patient> PatientT { get; set; }
        public DbSet<Admin> AdminT { get; set; }
        public DbSet<Doctor> DoctorT { get; set; }
        public DbSet<InsureEmp> InsureEmpT { get; set; }
        public DbSet<CureProgress> CureProgressT { get; set; }
        public DbSet<PatientTreatmentLog> PatientTreatmentLogT { get; set; }

        public DbSet<Role> RoleT { get; set; }

        public DbSet<Text> TextT { get; set; }

        public DbSet<Picture> PictureT { get; set; }

        public DbSet<DiagnosisTicket> DiagnosisTicketsT { get; set; }

        public DbSet<AllergyHealth> AllegyHealthsT { get; set; }

        public DbSet<DiagnosisActivityPricing> DiagnosisActivityPricingsT { get; set; }
    }
}
