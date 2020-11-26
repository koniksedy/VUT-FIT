using Microsoft.VisualBasic;
using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Nemocnice.Data
{
    public class Picture
    {
        [Required]
        public int PictureId { get; set; }
        [Required]
        public int NameInt { get; set; }
        [Required]
        public string Description { get; set; }
        [Required]
        public string SocialSecurityNum { get; set; }
        [Required]
        public DateTime CreateDate { get; set; }
    }
}