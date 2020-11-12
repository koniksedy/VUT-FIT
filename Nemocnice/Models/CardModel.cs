using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class CardModel
    {
        public List<CardPatientModel> OldPatients { get; set; } = new List<CardPatientModel>();

        public NewPatientModel NewPatient { get; set; }

    }
}
