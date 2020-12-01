using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class AddressCreator
    {
        public void AddressCreate(DatabaseContext db)
        {
            db.AddressT.Add(AddressAdd(2003, "Božetěchova", "Brno", 61200));
            db.AddressT.Add(AddressAdd(4568, "Hradecká", "Brno", 61200));
            db.AddressT.Add(AddressAdd(125, "Purkyňova", "Brno", 61200));
            db.AddressT.Add(AddressAdd(4563, "Edisonova", "Brno", 61200));
            db.AddressT.Add(AddressAdd(7859, "Herčíkova", "Brno", 61200));
            db.AddressT.Add(AddressAdd(785, "Technická", "Brno", 61200));
            db.AddressT.Add(AddressAdd(456, "Srbská", "Brno", 61200));
            db.AddressT.Add(AddressAdd(7895, "Bystřinova", "Brno", 61200));
            db.AddressT.Add(AddressAdd(1235, "Netušilova", "Brno", 61400));
            db.AddressT.Add(AddressAdd(22, "Elgartova", "Brno", 61400));
            db.AddressT.Add(AddressAdd(45, "Hálkova", "Brno", 61400));
            db.AddressT.Add(AddressAdd(478, "Musilova", "Brno", 61400));
            db.AddressT.Add(AddressAdd(77, "Rotalova", "Brno", 61400));
            db.AddressT.Add(AddressAdd(44, "Merhautova", "Brno", 61400));
            db.AddressT.Add(AddressAdd(895, "Provazníkova", "Brno", 61400));
            db.AddressT.Add(AddressAdd(562, "Dukelská třída", "Brno", 61400));
            db.AddressT.Add(AddressAdd(354, "Údolní", "Brno", 60200));
            db.AddressT.Add(AddressAdd(4521, "Pekařská", "Brno", 60200));
            db.AddressT.Add(AddressAdd(4560, "Úvoz", "Brno", 60200));
            db.AddressT.Add(AddressAdd(55, "Koliště", "Brno", 60200));
            db.AddressT.Add(AddressAdd(965, "Husova", "Brno", 60200));
            db.AddressT.Add(AddressAdd(852, "Milady Horákové", "Brno", 60200));
            db.AddressT.Add(AddressAdd(741, "Gorkého", "Brno", 60200));
            db.AddressT.Add(AddressAdd(963, "Foerstrova", "Brno", 61600));
            db.AddressT.Add(AddressAdd(7410, "Mozolky", "Brno", 61600));
            db.AddressT.Add(AddressAdd(8520, "Luční", "Brno", 61600));
            db.AddressT.Add(AddressAdd(9630, "Poznaňská", "Brno", 61600));
            db.AddressT.Add(AddressAdd(20, "Lidická", "Slavkov u Brna", 68401));
            db.AddressT.Add(AddressAdd(541, "Tyršova", "Slavkov u Brna", 68401));
            db.AddressT.Add(AddressAdd(50, "Špitálská", "Slavkov u Brna", 68401));
            db.AddressT.Add(AddressAdd(80, "Slavkovská", "Rousínov", 68301));
            db.AddressT.Add(AddressAdd(75, "U Kapličky", "Rousínov", 68301));
            db.AddressT.Add(AddressAdd(1258, "Komenského", "Adamov", 67904));
            db.AddressT.Add(AddressAdd(235, "Osvobození", "Adamov", 67904));
            db.SaveChanges();
        }
        private Address AddressAdd(int houseNumber, string streetName, string city, int zip)
        {
            return new Address {HouseNumber = houseNumber, StreetName = streetName, City = city, ZIP = zip};
        }
    }
}
