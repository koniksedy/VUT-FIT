using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class NewPatientModel
    {
        [Required]
        [Display(Name = "Jméno")]
        public string Name { get; set; }
        [Required]
        [Display(Name = "Příjmení")]
        public string Surname { get; set; }
        [Display(Name = "Titul")]
        public string Title { get; set; }
        [Display(Name = "Rodné číslo")]
        public long SocialSecurityNumber { get; set; }
        [Required]
        public int InsuranceCompany { get; set; }
        [Display(Name = "Telefon")]
        public string Tel { get; set; }
        [Display(Name = "Email")]
        [EmailAddress]
        public string Email { get; set; }
        [Display(Name = "Ulice")]
        public string Street { get; set; }
        [Display(Name = "Číslo popisné")]
        public int HouseNumber { get; set; }
        [Display(Name = "Město")]
        public string City { get; set; }
        [Display(Name = "Směrovací číslo")]
        public int Zip { get; set; }
    }
}
