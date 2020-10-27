using Nemocnice.Data;
using Nemocnice.DatabaseDataCreator;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class DatabaseCreatorController
    {
        public void DatabaseContentCreate()
        {
            using (var db = new DatabaseContext())
            {
                /*AddressCreator address = new AddressCreator();
                address.AddressCreate(db); 

                AllergyCreator allergy = new AllergyCreator();
                allergy.AllergyCreate(db); 

                DiagnosisCreator diagnosis = new DiagnosisCreator();
                diagnosis.DiagnosisCreate(db); 

                HealthConditionCreator healthCondition = new HealthConditionCreator();
                healthCondition.HealthConditionCreate(db);

                MedicallActivityPriceCreator medicallActivityPrice = new MedicallActivityPriceCreator();
                medicallActivityPrice.MedicallActivityPriceCreate(db); 

                UserCreator user = new UserCreator();
                user.UserCreate(db); 

                AdminCreator admin = new AdminCreator();
                admin.AdminCreate(db); 

                InsureEmpCreator insureEmp = new InsureEmpCreator();
                insureEmp.InsureEmpCreate(db); 

                DoctorCreator doctor = new DoctorCreator();
                doctor.DoctorCreate(db);

                PatientCreator patient = new PatientCreator();
                patient.PatientCreate(db); 
               
                MedicallBillCreator medicallBill = new MedicallBillCreator();
                medicallBill.MedicallBillCreate(db);

                PatientTreatmentLogCreator patientTreatmentLog = new PatientTreatmentLogCreator();
                patientTreatmentLog.PatientTreatmentLogCreate(db);

                MedicallReportCreator medicallReport = new MedicallReportCreator();
                medicallReport.MedicallReportCreate(db);

                ActivityPricingPerDiagnosisCreator activityPricingPerDiagnosis = new ActivityPricingPerDiagnosisCreator();
                activityPricingPerDiagnosis.ActivityPricingPerDiagnosisCreate(db);

                AllergysOfPatientCreator allergysOfPatient = new AllergysOfPatientCreator();
                allergysOfPatient.AllergysOfPatientCreate(db);

                CheckUpTicketCreator checkUpTicket = new CheckUpTicketCreator();
                checkUpTicket.CheckUpTicketCreate(db);

                CureProgressCreator cureProgress = new CureProgressCreator();
                cureProgress.CureProgressCreate(db);

                TicketPerDiagnosisCreator ticketPerDiagnosis = new TicketPerDiagnosisCreator();
                ticketPerDiagnosis.TicketPerDiagnosisCreate(db);

                //dodelat
                //PictureCreator picture = new PictureCreator();
                //picture.PictureCreate(db);

                patient.PatientCreate(db);*/
            }
        }

    }
}
