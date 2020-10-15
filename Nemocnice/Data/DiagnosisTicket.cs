namespace Nemocnice.Data
{
    public class DiagnosisTicket
    {
        public int DiagnosisTicketId { get; set; }

        public Diagnosis Diagnosis { get; set; }

        public CheckupTicket CheckupTicket { get; set; }
    }
}