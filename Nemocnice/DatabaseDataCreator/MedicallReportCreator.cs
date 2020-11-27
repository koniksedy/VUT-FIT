using Nemocnice.Data;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Nemocnice.DatabaseDataCreator
{
    public class MedicallReportCreator
    {
        public void MedicallReportCreate(DatabaseContext db)
        {
            /*
            Doctor DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            Patient PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9638521235").ToList().First();
            Doctor DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            DateTime datum = new DateTime(2016, 1, 1, 14, 50, 2);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum ));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9638521235").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2016, 1, 10, 9, 30, 24);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Sed molestie ipsum vel eleifend tincidunt. Vivamus auctor, orci quis semper mollis, ante metus vehicula est, imperdiet egestas nisi libero ut orci. Donec in lectus eu est pellentesque bibendum eu in purus. Aliquam pretium metus justo, vel feugiat nibh tincidunt at.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2568745120").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2016, 2, 2, 11, 5, 52);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Sed iaculis bibendum nibh, dictum euismod erat pharetra in. Curabitur iaculis, erat nec semper pellentesque, quam purus interdum nibh, at ornare est velit quis nisl. Vestibulum facilisis nisi a eros feugiat, id iaculis diam egestas. Vivamus a odio a orci sollicitudin condimentum ut nec nunc. Maecenas sit amet nunc non ex vestibulum sagittis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2568745120").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2016, 2, 21, 14, 15, 25);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Sed vitae fermentum nisl, vitae pulvinar leo. Praesent ornare laoreet iaculis. Quisque tristique rutrum nunc venenatis placerat. Ut efficitur sagittis nisi id blandit. Etiam quis volutpat eros. Sed neque risus, pharetra sit amet porta commodo, ultricies eu nulla. Etiam cursus libero a mi tempor euismod. Fusce sit amet fringilla massa, ac tincidunt lectus. Suspendisse porttitor turpis sed lorem facilisis blandit.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2568745120").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2016, 3, 24, 12, 52, 41);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Ut eget mauris et justo facilisis porttitor. Nunc fringilla justo odio, eget euismod mi facilisis eget. Cras venenatis, nibh ut consectetur hendrerit, metus erat ultricies sem, eu dignissim lorem urna nec nunc. Morbi a tempor ante. Nunc porta semper lacus, vel fermentum velit accumsan quis. Quisque vitae felis vel risus viverra auctor. Cras in enim blandit, ornare nulla id, rhoncus magna. Phasellus in nisi mauris. Maecenas ullamcorper dolor vel ligula tempus ornare.", true, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5698742103").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2016, 3, 3, 11, 15, 56);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Proin lacinia sem ex, quis ullamcorper ante dapibus id. Aliquam erat volutpat. Mauris lacinia vulputate nunc, malesuada rhoncus dui lobortis ac. Nullam id aliquet purus. Aliquam nisi dui, varius ut ornare non, vehicula eget erat. Sed ac ornare nibh. Ut feugiat consequat enim sodales tristique.", true, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5698742103").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            datum = new DateTime(2016, 3, 25, 7, 15, 6);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Nunc tristique turpis purus, vitae tristique tellus elementum ut. Integer consequat diam pharetra, semper velit sit amet, elementum felis. Maecenas eget tortor scelerisque, consequat ligula quis, tristique neque. Nullam hendrerit posuere ante, fringilla ultricies eros accumsan et. Curabitur ut dolor iaculis, bibendum urna quis, pulvinar arcu. ", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5698742103").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            datum = new DateTime(2016, 3, 31, 9, 55, 45);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Nam leo lectus, scelerisque eu euismod ut, condimentum ut dui. Curabitur mi est, sodales ac suscipit non, iaculis sed odio. Proin mattis, mi eget pellentesque congue, quam massa faucibus eros, pulvinar sagittis ex nisi mattis eros. Integer ornare, nisi et vulputate fermentum, metus nisi placerat massa, a fermentum sapien lorem a velit. Pellentesque sagittis hendrerit sollicitudin. Proin fermentum feugiat arcu quis venenatis. Vivamus vulputate ligula sed congue condimentum.", true, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2356987452").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            datum = new DateTime(2016, 4, 5, 12, 11, 19);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Proin egestas rhoncus augue ac placerat. Pellentesque sollicitudin iaculis lorem, nec maximus mauris tempus non. Donec posuere quis sapien efficitur vestibulum. Donec id arcu quis neque blandit bibendum non in augue. Nulla interdum vulputate malesuada. Etiam libero ante, pretium vitae scelerisque at, posuere viverra nunc. Vivamus molestie lacus at odio feugiat ornare.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2356987452").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            datum = new DateTime(2016, 4, 20, 13, 10, 49);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Aenean risus diam, faucibus ornare mauris nec, viverra volutpat ex. In a magna in nibh facilisis finibus. Aliquam placerat felis urna, sed semper ligula bibendum in. Nulla efficitur velit velit, eu semper ex lobortis nec.", true, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9638521235").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            datum = new DateTime(2016, 1, 1, 14, 50, 2);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Quisque molestie ligula leo. Phasellus eleifend, ante elementum venenatis sodales, ipsum enim interdum felis, sit amet convallis ligula mauris quis tortor. Phasellus auctor sem augue, at ornare lacus porta in. Curabitur cursus consequat nunc, quis aliquet ante. Pellentesque rutrum tellus a nisi blandit cursus. Proin sit amet elit enim. In at interdum elit.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8562101478").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            datum = new DateTime(2017, 5, 6, 13, 44, 28);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "In hac habitasse platea dictumst. Etiam fermentum lorem ut dignissim fermentum. Sed ut ligula non ligula dictum viverra. Sed in mattis nisi, vitae scelerisque lacus. Vestibulum tempor sapien a nunc condimentum interdum. Suspendisse sed turpis efficitur, malesuada augue non, rhoncus mi. Aenean in lorem mollis, imperdiet neque at, placerat sapien.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8562101478").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            datum = new DateTime(2017, 5, 28, 7, 4, 8);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Maecenas non elementum elit. Etiam tincidunt, enim malesuada finibus feugiat, dolor urna tempus nunc, quis tempor nisl velit id nibh. Nulla neque arcu, facilisis non tincidunt accumsan, interdum vel risus.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8963214568").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            datum = new DateTime(2017, 6, 7, 14, 42, 25);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Praesent gravida eget ligula ut rutrum. Vestibulum mollis, risus sit amet accumsan pharetra, enim erat pellentesque lacus, non lobortis sapien turpis non quam. Sed interdum arcu id ipsum auctor luctus. Quisque varius enim metus, id volutpat ex imperdiet eu. Mauris massa augue, ullamcorper eget ipsum eget, pharetra semper mauris. Vestibulum non justo eu nisl sodales ornare ut in eros. Nulla gravida metus non sagittis venenatis. Donec non scelerisque leo, vel laoreet turpis.", true, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7412563201").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            datum = new DateTime(2017, 8, 31, 14, 52, 38);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Proin viverra commodo justo ac tempus. Maecenas dolor odio, tempor luctus volutpat vitae, sollicitudin sit amet odio. Etiam interdum, neque lacinia porta euismod, mi nisl dictum sapien, rutrum commodo metus magna eget augue. Aliquam eget cursus urna. Fusce sit amet euismod enim. ", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8745213698").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            datum = new DateTime(2017, 8, 22, 10, 58, 39);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Sed aliquam faucibus sem. Aenean id nisi tincidunt leo feugiat volutpat. Quisque tincidunt erat risus. Aliquam et elementum nibh, quis tempus lectus. Donec nulla velit, auctor sed lobortis vitae, dignissim id magna. Phasellus convallis justo massa, a porta massa vulputate in. Praesent ac pretium nisi.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2014785630").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            datum = new DateTime(2017, 9, 17, 8, 20, 15);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Suspendisse non ante ac arcu pretium porttitor. Donec mollis aliquet massa sed efficitur. Nullam porta lacus a lacinia pellentesque. Duis consectetur rhoncus consectetur. Praesent mollis lectus lacus, consectetur placerat odio consectetur ac. Pellentesque in laoreet quam. Aenean congue tempor ipsum, a iaculis ipsum. Aliquam egestas sodales lectus, vel fringilla mi. ", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "2014785630").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            datum = new DateTime(2017, 9, 20, 9, 20, 15);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Phasellus blandit libero augue, vitae lacinia diam euismod in. In hac habitasse platea dictumst. Maecenas eget nulla ac quam porttitor gravida. Aenean et nibh sapien. Vivamus ut nunc ipsum. Duis in sapien et dolor ultrices feugiat. Suspendisse metus ligula, fringilla tempor ornare ut, commodo nec mauris.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9856321452").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            datum = new DateTime(2018, 12, 21, 12, 54, 52);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Curabitur lectus nisl, accumsan sed mauris a, posuere tristique mauris. Proin condimentum quam sed efficitur suscipit. Proin justo libero, lacinia vitae malesuada nec, ullamcorper placerat nibh.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8520147852").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            datum = new DateTime(2018, 1, 3, 12, 42, 12);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Nam fermentum non felis quis interdum. Duis ante ex, consectetur et sollicitudin a, lobortis a nunc. Fusce ultricies tortor augue. Aenean molestie ut felis ut pellentesque. Quisque consequat nibh et nunc ornare aliquet. Mauris mattis arcu odio, id facilisis metus tristique vitae. Nullam varius sollicitudin vestibulum. Nam cursus arcu vel ipsum commodo, bibendum lobortis turpis auctor. Aenean posuere ex eget cursus faucibus.", true, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8520147852").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            datum = new DateTime(2018, 1, 25, 9, 2, 1);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Praesent dapibus euismod mauris, ac viverra dui feugiat ac. Nulla accumsan varius metus, quis dictum est posuere et. Suspendisse in magna turpis. Duis rhoncus eget est non consectetur. Donec ultricies vulputate finibus. Suspendisse felis sem, vulputate id nibh at, vehicula fringilla odio.", true, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9236541258").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            datum = new DateTime(2019, 5, 27, 11, 45, 9);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Vestibulum sit amet metus ut mauris blandit semper. In vitae elit est. Suspendisse tincidunt aliquam lectus ut dignissim. Suspendisse vestibulum eleifend elit non sollicitudin. Donec est neque, facilisis et est eget, porttitor fermentum justo. Vivamus consectetur purus sed eros lacinia vehicula. Proin vitae arcu felis. Proin lacinia hendrerit viverra.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9236541258").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            datum = new DateTime(2019, 6, 17, 7, 5, 59);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Duis fringilla auctor efficitur. Sed ullamcorper non nunc nec placerat. Suspendisse tempus purus felis, vitae suscipit neque efficitur id. Ut non tincidunt quam. Morbi non nunc ut risus varius eleifend. Duis enim odio, luctus eu luctus id, porttitor in mi. Ut vitae accumsan felis. Cras pellentesque elementum tincidunt. Quisque eu faucibus est, sed fringilla dui.", true, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8523654124").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            datum = new DateTime(2019, 7, 14, 13, 28, 58);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Proin fringilla dictum vulputate. Etiam eget venenatis mauris. Curabitur nisl risus, rhoncus ut enim at, convallis iaculis felis. Vivamus tincidunt lobortis tincidunt. Nunc at hendrerit ligula. Vestibulum consequat turpis eu imperdiet ornare. Curabitur sed hendrerit ipsum.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5478512365").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            datum = new DateTime(2020, 9, 10, 15, 40, 13);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Nam at suscipit arcu. Nulla lacinia purus lorem, ut pulvinar metus tincidunt sed. Sed eu ex finibus, porta felis sit amet, congue felis. Sed at malesuada ante. Nam viverra posuere diam in sollicitudin.", false, DoctorOwn, datum));
            */

            Doctor DoctorId;
            Patient PatientId;
            Doctor DoctorOwn;
            DateTime datum;

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8605274117").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2016, 2, 1, 10, 20, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8605274117").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2016, 3, 7, 9, 12, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7204236798").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            datum = new DateTime(2017, 11, 25, 13, 34, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7204236798").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            datum = new DateTime(2017, 12, 12, 17, 55, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7204236798").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2018, 3, 10, 15, 47, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9409278736").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 966699).ToList().First();
            datum = new DateTime(2017, 4, 23, 14, 38, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9409278736").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 118799).ToList().First();
            datum = new DateTime(2018, 9, 7, 7, 0, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "9409278736").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2019, 10, 1, 10, 21, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7306261116").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2018, 3, 30, 14, 43, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7306261116").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2018, 4, 28, 13, 34, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5804249330").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2019, 5, 17, 17, 56, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310079755").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            datum = new DateTime(2018, 4, 9, 15, 47, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310079755").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 100021).ToList().First();
            datum = new DateTime(2019, 2, 11, 14, 39, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310079755").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            datum = new DateTime(2019, 8, 24, 7, 0, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5609101014").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2018, 6, 7, 11, 22, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5609101014").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2018, 8, 9, 15, 43, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "5407078259").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            datum = new DateTime(2019, 5, 19, 7, 5, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7952217779").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            datum = new DateTime(2020, 1, 12, 15, 48, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7952217779").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2020, 1, 27, 8, 10, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7952217779").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2020, 5, 30, 7, 1, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "6454221719").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2020, 4, 14, 11, 22, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "6454221719").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            datum = new DateTime(2020, 5, 10, 9, 14, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "6960249307").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2020, 7, 28, 8, 5, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8661094849").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 100024).ToList().First();
            datum = new DateTime(2020, 7, 13, 17, 57, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8661094849").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2020, 8, 21, 15, 48, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7262204400").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2020, 2, 15, 8, 10, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0254270401").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987588).ToList().First();
            datum = new DateTime(2018, 12, 17, 12, 31, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0254270401").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            datum = new DateTime(2019, 8, 17, 11, 23, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0254270401").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2020, 1, 20, 15, 44, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0261203349").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            datum = new DateTime(2019, 11, 14, 8, 6, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2017, 12, 30, 17, 57, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2018, 1, 27, 10, 19, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            datum = new DateTime(2018, 5, 12, 8, 10, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7561200394").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2019, 2, 10, 12, 32, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7356020210").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            datum = new DateTime(2018, 12, 8, 16, 53, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7356020210").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2018, 12, 25, 15, 45, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7356020210").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2019, 1, 5, 17, 58, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0110038379").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2019, 5, 22, 16, 49, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0110038379").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            datum = new DateTime(2019, 5, 30, 9, 11, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310190998").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            datum = new DateTime(2018, 7, 19, 7, 2, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7310190998").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            datum = new DateTime(2018, 12, 3, 16, 54, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7705156437").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2019, 11, 25, 9, 15, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0053229792").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 111245).ToList().First();
            datum = new DateTime(2018, 2, 27, 8, 7, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "0053229792").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            datum = new DateTime(2018, 3, 5, 12, 28, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987777).ToList().First();
            datum = new DateTime(2019, 5, 17, 10, 20, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2019, 7, 24, 9, 11, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2019, 7, 30, 7, 3, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8862026096").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 987478).ToList().First();
            datum = new DateTime(2020, 2, 14, 17, 54, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7951257193").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 159357).ToList().First();
            datum = new DateTime(2020, 4, 11, 9, 16, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "7951257193").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 254789).ToList().First();
            datum = new DateTime(2020, 8, 9, 8, 7, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8052047377").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 147852).ToList().First();
            datum = new DateTime(2020, 7, 19, 17, 59, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            DoctorId = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            PatientId = db.PatientT.Where(s => s.SocialSecurityNum == "8052047377").ToList().First();
            DoctorOwn = db.DoctorT.Where(s => s.ICZ == 174587).ToList().First();
            datum = new DateTime(2020, 8, 6, 7, 3, 0);
            db.MedicallReportT.Add(MedicallReportAdd(DoctorId, PatientId, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin lectus justo, lacinia sit amet urna et, sodales sollicitudin odio. Aliquam nec accumsan metus, eget vulputate nibh. Curabitur sit amet feugiat urna. Nam mattis est eu egestas sagittis. Nulla porttitor luctus tortor, sit amet mattis orci lacinia non. Nulla vel dictum arcu. Duis luctus nunc ut libero commodo, ac convallis enim accumsan. Quisque accumsan turpis sagittis sollicitudin venenatis.", false, DoctorOwn, datum));

            db.SaveChanges();
        }
        private MedicallReport MedicallReportAdd( Doctor author, Patient patient, string description, bool includePic, Doctor owner, DateTime createDate)
        {
            return new MedicallReport { Author = author, Patient = patient, Description = description, IncludePic = includePic, Owner = owner, CreateDate = createDate };
        }
    }
}
