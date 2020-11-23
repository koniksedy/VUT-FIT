using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    public class User
    {
        [Required]
        public int UserId { get; set; }
        [Required]
        public string Login { get; set; }
        [Required]
        public string Name { get; set; }
        [Required]
        public string Surname { get; set; }
        public string Title { get; set; }
        [Required]
        public string Phone { get; set; }
        [EmailAddress]
        public string Email { get; set; }
        public Address WorkAddress { get; set; }

        public string getFullName()
        {
            if (String.IsNullOrEmpty(Title))
            {
                return Name + " " + Surname;
            }
            else
            {
                return Name + " " + Surname + ", " + Title;
            }
        }

    }
}