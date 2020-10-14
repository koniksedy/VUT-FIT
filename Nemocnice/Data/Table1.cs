using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

/*
 * Soubor reprezentujísí data tabulky2
 * -----------------------------------
 * Úpravy:
 * 14.10.2020 - vytvoření šablony - xsedym02
 */

namespace Nemocnice.Data
{
    public class Table1
    {
        // Primární klíč. Musí být vždy ve tvaru <class-name>Id.
        public int Table1Id { get; set; }

        public int ItemVal_Tab1 { get; set; }
    }
}
