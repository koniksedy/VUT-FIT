using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class PatientCardModel
    {
        public User User { get; set; }
        public Patient Patient { get; set; }
        public ResultOption StudentGender { get; set; }
        public int StId { get; set; }
        public IEnumerable<State> States { get; set; }
    }
}
