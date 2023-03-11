"""
download_urls.sh
UPA (Project 3)
The program downloads all product URLs from https://playtech.co.nz
and saves results to the file specified as a program argument.
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 05.10.222
"""


import sys
from scraper import scraper


if __name__ == "__main__":
    if {"-h", "--help"}.intersection(sys.argv):
        print("USAGE: python3 download_urls.py <outputfile>", file=sys.stderr)
        print(file=sys.stderr)
        print("   outputfile    Output file with downloaded URLs.", file=sys.stderr)
        sys.exit(0)

    if len(sys.argv) != 2:
        raise ValueError("Bad program arguments. Try --help.")

    scr = scraper.Scraper("https://playtech.co.nz")
    scr.fetch_all()
    scr.save(sys.argv[1])
