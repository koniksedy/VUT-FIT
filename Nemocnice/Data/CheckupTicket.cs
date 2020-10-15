using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace Nemocnice.Data
{
    public class CheckupTicket
    {
        public int CheckupTicketId { get; set; }
        [Required]
        public DateTime CreateDate { get; set; }
        [Required]
        public Doctor CreatedBy { get; set; }
       // [Required]
        public Doctor ToDoctor { get; set; }
        [Required]
        public Patient Patient { get; set; }
        public virtual List<Text> Description { get; set; }
        public virtual List<Picture> Img { get; set; }
        //[Required]
       // public virtual List<Diagnosis> Diagnoses { get; set; }
    }
}