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
        private Picture PictureAdd(long socialSecurityNum, byte[] currentPicture, string Description, Allergy toAllergy, MedicallReport toMedicalReport, CheckupTicket toCheckupTicket, DateTime createDate)
        {
            return new Picture { SocialSecurityNum = socialSecurityNum, CurrentPicture = currentPicture, Description = Description, ToAllergy = toAllergy, ToMedicalReport = toMedicalReport, ToCheckupTicket = toCheckupTicket, CreateDate = createDate };
        }
    }
}
