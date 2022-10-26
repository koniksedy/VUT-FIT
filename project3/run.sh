#!/bin/bash

# run.sh
# UPA (Project 3)
# The script downloads all product URLs from https://playtech.co.nz
# and finds names and prices of the first 20.
# Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
#          Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
#          Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 05.10.222

# Open environment
printf "Opening environment...  "
source upa_env/bin/activate
printf "Done\n"

# Download URLs
printf "Downloading... "
python3 download_urls.py urls.txt
printf "Done\n"

# Get first 20 Names and Prices
printf "Getting names and prices... "
head urls.txt -n 20 | python3 get_prices.py data.tsv
printf "Done\n"
