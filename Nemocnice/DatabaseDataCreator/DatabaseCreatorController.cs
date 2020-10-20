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
                LoginCreator login = new LoginCreator();
                //login.LoginCreate(db);
            }
        }

    }
}
