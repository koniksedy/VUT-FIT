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
        public string StreetName { get; set; }
        public string City { get; set; }
        public int ZIP { get; set; }
        public int HouseNumber { get; set; }

    }
}