namespace Nemocnice.Data
{
    public class AllergyHealth
    {
        public int AllergyHealthId { get; set; }

        public Allergy Allergy { get; set; }

        public HealthCondition HealthCondition {get; set;}

    }
}