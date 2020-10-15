using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class User
    {
        public int UserId { get; set; }
        [Required]
        public Login Login { get; set; }
        [Required]
        public string Name { get; set; }
        [Required]
        public string Surname { get; set; }
        public string Title { get; set; }
        [Required]
        public string Phone { get; set; }
        public string Email { get; set; }
        [Required]
        public string SocialNumber { get; set; }
        public Address WorkAddress { get; set; }
        public Doctor Doctor { get; set; }
        public InsureEmp InsureEmp { get; set; }
        public Admin Admin { get; set; }
    }
}