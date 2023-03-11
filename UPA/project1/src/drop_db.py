"""
drop_db.py
Program that drops cisjr database from mongodb.
Data can be downloaded in parallel.
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 12.11.2022
"""

import sys
import pymongo


def main():
    if {"-h", "--help"}.intersection(sys.argv):
        print("usage: drop_db.py", file=sys.stderr)

    connection_string = "mongodb://localhost:27017"
    client = pymongo.MongoClient(connection_string)
    print("Dropping...    ", end="", file=sys.stderr)
    sys.stderr.flush()
    client.drop_database("cisjr")
    print("Done", file=sys.stderr)
    client.close()


if __name__ == "__main__":
    main()
