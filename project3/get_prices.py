"""
get_prices.sh
UPA (Project 3)
The program finds names and prices (at https://playtech.co.nz)
for a list of product URLs in an input file or from the STDIN.
The results are saved to the output file specified as a program parameter.
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 05.10.222
"""


import sys
from scraper import scraper


if __name__ == "__main__":
    # Test for Help
    if {"-h", "--help"}.intersection(sys.argv):
        print("USAGE: python3 get_prices.py [inputfile] <outputfile>", file=sys.stderr)
        print(file=sys.stderr)
        print("   inputfile     File with URLs. If is not specified, STDIN will be used.", file=sys.stderr)
        print("   outputfile    Output file, with data in the format: URL<tab>NAME<tab>PRICE.", file=sys.stderr)
        sys.exit(0)

    if len(sys.argv) < 3:
        # Reading from STDIN
        with open(sys.argv[1], "w") as out_file:
            for url in sys.stdin.readlines():
                url = url.strip()
                name, price = scraper.Scraper.get_product_name_price(url)
                print(url, name, price, sep="\t", file=out_file)
    elif len(sys.argv) == 3:
        # Reading from inputfile.
        with open(sys.argv[1], "r") as in_file:
            with open(sys.argv[2], "w") as out_file:
                for url in in_file.readlines():
                    url = url.strip()
                    name, price = scraper.Scraper.get_product_name_price(url)
                    print(url, name, price, sep="\t", file=out_file)
    else:
        raise ValueError("Bad program arguments. Try --help.")
