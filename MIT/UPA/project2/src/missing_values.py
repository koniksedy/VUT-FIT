"""
missing_values.py
Program calculates the number of missing values in columns.
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 19.11.2022
"""

import pandas as pd

if __name__ == "__main__":
    penguin_df = pd.read_csv("../penguins_lter.csv")

    # Removing Commnents column
    penguin_df.drop(['Comments'], axis=1, inplace=True)

    # Pandas settings
    pd.set_option('display.max_columns', None)
    pd.set_option('display.width', None)
    pd.set_option('display.max_colwidth', None)

    print(penguin_df[penguin_df.isna().any(axis=1)])

    for column in penguin_df:
        seriesObj = penguin_df[column]
        print("Missing values in column ", column, ": ", penguin_df[column].isna().sum())





