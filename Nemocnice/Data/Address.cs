using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class Address
    {
        public int AddressId { get; set; }
        public int HouseNumber { get; set; }
        public string StreetName { get; set; }
        [Required]
        public string City { get; set; }
        public int ZIP { get; set; }
    }
}