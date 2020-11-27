using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.Data
{
    public class PictureOnTicket
    {
        public int PictureOnTicketId { get; set; }
        [Required]
        public Picture Picture { get; set; }
        [Required]
        public CheckupTicket Ticket { get; set; }
    }
}
