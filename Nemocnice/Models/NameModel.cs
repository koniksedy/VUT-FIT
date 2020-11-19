using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Models
{
    public class NameModel
    {
        public string Name { get; set; }
        public string Surname { get; set; }
        public string Title { get; set; }

        public string getFullName()
        {
            if(String.IsNullOrEmpty(Title))
            {
                return Name + Surname;
            }
            else
            {
                return Name + Surname + ", " + Title;
            }
        }
    }
}
