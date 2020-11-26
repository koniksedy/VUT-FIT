using Nemocnice.Data;

namespace Nemocnice.Models
{
    public class DoctorJoined 
    {
        public int UserId { get; set; }
        public string Login { get; set; }
        public string Name { get; set; }
        public string Surname { get; set; }
        public string Title { get; set; }
        public string Phone { get; set; }
        public string Email { get; set; }
        public int ICZ { get; set; }
        public Address WorkAddress { get; set; }
    }
}