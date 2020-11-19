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
        public NameModel PatientFullName { get; set; }
        public long SocialSecurityNum { get; set; }
        public int Insurance { get; set; }
    }
}
