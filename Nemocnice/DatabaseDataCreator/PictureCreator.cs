using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class PictureCreator
    {
        public void PictureCreate(DatabaseContext db)
        {
           
            //db.PictureT.Add(PictureAdd());

            db.SaveChanges();
        }
        private Picture PictureAdd(string socialSecurityNum, string Description, int Name, DateTime createDate)
        {
            return new Picture { SocialSecurityNum = socialSecurityNum, Description = Description, NameInt = Name, CreateDate = createDate };
        }
    }
}
