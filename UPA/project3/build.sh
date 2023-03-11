#!/bin/bash

# build.sh
# UPA (Project 3)
# This script creates python environment and installs all necessary packages
# listed in the file requirements.txt
# Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
#          Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
#          Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 05.10.222

# Creating environment
printf "Creating environment...  "
python3 -m venv upa_env
source upa_env/bin/activate
printf "Done\n"

# Installing requirements
printf "Installing requirements... "
python3 -m pip install -r requirements.txt
printf "Done\n"
