namespace Nemocnice.Data
{
    public class DiagnosisActivityPricing
    {
        public int DiagnosisActivityPricingId { get; set; }

        public Diagnosis CurrentDiagnosis { get; set; }

        public MedicallActivityPrice CurrentPricing { get; set; }
    }
}