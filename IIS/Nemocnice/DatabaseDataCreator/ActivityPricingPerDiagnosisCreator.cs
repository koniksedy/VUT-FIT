using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class ActivityPricingPerDiagnosisCreator
    {
        public void ActivityPricingPerDiagnosisCreate(DatabaseContext db)
        {
            Diagnosis DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Lupénka").ToList().First();
            MedicallActivityPrice Price = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Malárie").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Cholera").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Cholera").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Tuberkulóza").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Tuberkulóza").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Vstupní vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zápal plic").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zápal plic").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Vstupní vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět středního ucha").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět nosohltanu").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět močového měchýře").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět průdušek").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "AIDS").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina prstu").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina krčku").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina krčku").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina patní kosti").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina předloktí").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina bérce").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny bérce").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina bérce").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina holenní kosti").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny holenní kosti").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Otřes mozku").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Vyšetření traumatologem").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět ledvin").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět ledvin").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět hrtanu").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět mozkových blan").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět slepého střeva").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Operace zánětu slepého střeva").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět slepého střeva").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Akutní bolest zad").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zánět dásní").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Preventivní prohlídka").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina klíční kosti").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny stehenní kosti").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina stehenní kosti").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina nártní kosti").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina zápěstí").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny zápěstí").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina zápěstí").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina pažní kosti").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Ošetření zlomeniny paže").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Zlomenina pažní kosti").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Rentgenové vyšetření").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Borelióza").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Kožní test").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina kůže").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Rakovina slinivky břišní").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Odběr krve").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Spála").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Srdeční infarkt").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Vyšetření traumatologem").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Srdeční infarkt").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Mozková mrtvice").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Vyšetření traumatologem").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Mozková mrtvice").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Hospitalizace").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));

            DiagnosisId = db.DiagnosisT.Where(s => s.Name == "Žloutenka").ToList().First();
            Price = db.MedicallActivityPriceT.Where(s => s.Name == "Krevní obraz").ToList().First();
            db.ActivityPricingPerDiagnosisT.Add(ActivityPricingPerDiagnosisAdd(DiagnosisId, Price));


            db.SaveChanges();
        }
        private ActivityPricingPerDiagnosis ActivityPricingPerDiagnosisAdd(Diagnosis diagnosis, MedicallActivityPrice medicallActivityPrice)
        {
            return new ActivityPricingPerDiagnosis { Diagnosis = diagnosis, MedicallActivityPrice = medicallActivityPrice };
        }
    }
}
