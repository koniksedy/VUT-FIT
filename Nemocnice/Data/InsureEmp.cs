using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class InsureEmp
    {
        public int InsureEmpId { get; set; }
        [Required]
        public int InsuranceEmployeeId { get; set; }
        [Required]
        public string WorkPhone { get; set; }
        [Required]
        public string Possition { get; set; }
    }
}