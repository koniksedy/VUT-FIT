/*
 * Model pro zobrazení Card (kartotéky)
 * Autor: Michal Šedý (xsedym02)
 * Poslední změna: 12.11.2020
 */

namespace Nemocnice.Models
{
    /*
     * Model uchovává informace o pacientovi.
     */
    public class CardModel
    {
        public string Name { get; set; }
        public string Surname { get; set; }
        public string Title { get; set; }
        public long SocialSecurityNum { get; set; }
        public int Insurance { get; set; }
    }
}
