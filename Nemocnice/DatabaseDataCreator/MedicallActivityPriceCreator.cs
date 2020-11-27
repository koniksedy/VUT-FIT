using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class MedicallActivityPriceCreator
    {
        public void MedicallActivityPriceCreate(DatabaseContext db)
        {
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Vstupní vyšetření", 300));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Ošetření zlomeniny bérce", 2500));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Ošetření zlomeniny zápěstí", 1920));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Ošetření zlomeniny paže", 3200));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Ošetření zlomeniny stehenní kosti", 4500));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Ošetření zlomeniny holenní kosti", 3000));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Preventivní prohlídka", 620));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Rentgenové vyšetření", 390));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Odběr krve", 50));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Kožní test", 214));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Krevní obraz", 18));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Vyšetření traumatologem", 208));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Hospitalizace", 2000));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Operace zánětu slepého střeva", 4200));
            db.MedicallActivityPriceT.Add(MedicallActivityPriceAdd("Ošetření zánětu spojivek", 920));
            db.SaveChanges();
        }
        private MedicallActivityPrice MedicallActivityPriceAdd(string name, decimal amount)
        {
            return new MedicallActivityPrice { Name = name, Amount = amount };
        }
    }
}
