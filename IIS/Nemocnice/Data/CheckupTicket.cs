using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Diagnostics.CodeAnalysis;

namespace Nemocnice.Data
{
    public class CheckupTicket
    {
        [Required]
        public int CheckupTicketId { get; set; }
        [Required]
        public Doctor CreatedBy { get; set; }
        public Doctor ToDoctor { get; set; }
        [Required]
        public Patient Patient { get; set; }    
        public string Description { get; set; }

        [DefaultValue(false)]
        public bool IncludePicDescript { get; set; }
        public string Result { get; set; }
        [DefaultValue(false)]
        public bool IncludePicRes { get; set; }
        public string State { get; set; }
        [Required]
        public DateTime CreateDate { get; set; }
        public DateTime FinishDate { get; set; }
    }
}