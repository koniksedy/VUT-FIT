using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class DiagnosisCreator
    {
        public void DiagnosisCreate(DatabaseContext db)
        {
            db.DiagnosisT.Add(DiagnosisAdd("Lupénka"));
            db.DiagnosisT.Add(DiagnosisAdd("Neštovice"));
            db.DiagnosisT.Add(DiagnosisAdd("Malárie"));
            db.DiagnosisT.Add(DiagnosisAdd("Cholera"));
            db.DiagnosisT.Add(DiagnosisAdd("Tuberkulóza"));
            db.DiagnosisT.Add(DiagnosisAdd("Zápal plic"));
            db.DiagnosisT.Add(DiagnosisAdd("Zánět středního ucha"));
            db.DiagnosisT.Add(DiagnosisAdd("Zánět nosohltanu"));
            db.DiagnosisT.Add(DiagnosisAdd("Zánět močového měchýře"));
            db.DiagnosisT.Add(DiagnosisAdd("Zánět průdušek"));
            db.DiagnosisT.Add(DiagnosisAdd("AIDS"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina prstu"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina krčku"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina patní kosti"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina předloktí"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina bérce"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina holenní kosti"));
            db.DiagnosisT.Add(DiagnosisAdd("Otřes mozku"));
            db.DiagnosisT.Add(DiagnosisAdd("Zánět ledvin"));
            db.DiagnosisT.Add(DiagnosisAdd("Zánět hrtanu"));
            db.DiagnosisT.Add(DiagnosisAdd("Zánět mozkových blan"));
            db.DiagnosisT.Add(DiagnosisAdd("Zánět slepého střeva"));
            db.DiagnosisT.Add(DiagnosisAdd("Akutní bolest zad"));
            db.DiagnosisT.Add(DiagnosisAdd("Zánět dásní"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina klíční kosti"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina stehenní kosti"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina nártní kosti"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina zápěstí"));
            db.DiagnosisT.Add(DiagnosisAdd("Zlomenina pažní kosti"));
            db.DiagnosisT.Add(DiagnosisAdd("Borelióza"));
            db.DiagnosisT.Add(DiagnosisAdd("Rakovina kůže"));
            db.DiagnosisT.Add(DiagnosisAdd("Rakovina slinivky břišní"));
            db.DiagnosisT.Add(DiagnosisAdd("Spála"));
            db.DiagnosisT.Add(DiagnosisAdd("Srdeční infarkt"));
            db.DiagnosisT.Add(DiagnosisAdd("Žloutenka"));
            db.DiagnosisT.Add(DiagnosisAdd("Mozková mrtvice"));
            db.SaveChanges();
        }
        private Diagnosis DiagnosisAdd(string name)
        {
            return new Diagnosis { Name = name };
        }
    }
}
