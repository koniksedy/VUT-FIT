"""
dataset_transformation.py
Program carries out outlier detection and removal, filling of missing data and
transforms the dataset into 2 different forms - categorical and numerical
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 19.11.2022
"""

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def remove_outliers(orig_df, columns):
    new_df = pd.DataFrame()
    for col in columns:
        lower_quantile = orig_df[col].quantile(0.25)
        upper_quantile = orig_df[col].quantile(0.75)
        interquartile_range = upper_quantile - lower_quantile

        fence_low = lower_quantile - 1.5 * interquartile_range
        fence_high = upper_quantile + 1.5 * interquartile_range

        new_df = orig_df.loc[(orig_df[col] > fence_low) & (orig_df[col] < fence_high)]
        orig_df = new_df

    return new_df

def fill_missing_values(orig_df, columns):
    new_df = orig_df.copy()
    for col in columns:
        new_df[col].fillna(new_df[col].mean(numeric_only=True), inplace=True)

    return new_df


def min_max_normalization(orig_df, column):
    new_df = orig_df.copy()
    for col in column:
        new_df[col] = (new_df[col] - new_df[col].min()) / (new_df[col].max() - new_df[col].min())
    return new_df

if __name__ == "__main__":
    penguin_df = pd.read_csv("../penguins_lter.csv")
    print(penguin_df.to_string())

    transform_df = penguin_df

    # Pandas settings
    pd.set_option('display.max_columns', None)
    pd.set_option('display.width', None)
    pd.set_option('display.max_colwidth', None)



    # Skrátenie dĺžok názvok druhov tučniakov
    penguin_df.replace("Adelie Penguin (Pygoscelis adeliae)", "Adelie penguin", inplace=True)
    penguin_df.replace("Gentoo penguin (Pygoscelis papua)", "Gentoo penguin", inplace=True)
    penguin_df.replace("Chinstrap penguin (Pygoscelis antarctica)", "Chinstrap penguin", inplace=True)

    # Odstranenie atributov, ktore nemaju vyznam pre dolovaciu ulohu
    transform_df.drop(inplace = True, columns=['studyName', 'Sample Number', 'Region', 'Stage', 'Individual ID',
                               'Date Egg', 'Comments'])


    # Rucne doplnenie hodnot
    transform_df['Sex'].fillna(value='MALE', limit=5, inplace=True)
    transform_df['Sex'].fillna(value='FEMALE', limit=5, inplace=True)
    transform_df.iloc[336, 7] = 'FEMALE'

    # Odstranenie odlahlych hodnot pri numerickych atributoch
    numeric_columns = ['Culmen Length (mm)', 'Culmen Depth (mm)', 'Flipper Length (mm)', 'Body Mass (g)',
                   'Delta 15 N (o/oo)', 'Delta 13 C (o/oo)']

    # Rozdelenie DF na viacero podla druhu tucniaka
    adelie_df = transform_df[penguin_df['Species'] == 'Adelie penguin']
    gentoo_df = transform_df[penguin_df['Species'] == 'Gentoo penguin']
    chinstrap_df = transform_df[penguin_df['Species'] == 'Chinstrap penguin']

    # Doplnenie chýbajúcich hodnôt pomocou strednej hodnoty
    # Median
    adelie_df = fill_missing_values(adelie_df, numeric_columns)
    gentoo_df = fill_missing_values(gentoo_df, numeric_columns)
    chinstrap_df = fill_missing_values(chinstrap_df, numeric_columns)

    # Remove outlier values from dataframes
    adelie_df = remove_outliers(adelie_df, numeric_columns)
    chinstrap_df = remove_outliers(chinstrap_df, numeric_columns)
    gentoo_df = remove_outliers(gentoo_df, numeric_columns)

    # Merge dataframes
    clean_df = pd.concat([adelie_df, chinstrap_df, gentoo_df])

    # Plot boxplots
    fig1, axes1 = plt.subplots(3, 1)

    sns.boxplot(ax=axes1[0], data=clean_df, x='Body Mass (g)', y='Species')
    sns.boxplot(ax=axes1[1], data=clean_df, x='Flipper Length (mm)', y='Species')
    sns.boxplot(ax=axes1[2], data=clean_df, x='Culmen Length (mm)', y='Species')
    plt.tight_layout()
    plt.show()
    plt.close()

    fig1.savefig("../plots/outlier_values1_transformed.pdf")

    fig2, axes2 = plt.subplots(3, 1)
    fig2.suptitle("Visualisation of outlier values")
    sns.boxplot(ax=axes2[0], data=clean_df, x='Culmen Depth (mm)', y='Species')
    sns.boxplot(ax=axes2[1], data=clean_df, x='Delta 13 C (o/oo)', y='Species')
    sns.boxplot(ax=axes2[2], data=clean_df, x='Delta 15 N (o/oo)', y='Species')
    plt.tight_layout()
    plt.show()
    plt.close()
    fig2.savefig("../plots/outlier_values2_transformed.pdf")

    # Diskretizácia numerických atribútov
    discrete_df = clean_df.copy()
    for col in numeric_columns:
        discrete_df[col] = pd.qcut(discrete_df[col], 5, labels=['A', 'B', 'C', 'D', 'E'])

    #print(discrete_df.to_string())

    # Transformácia numerických atribútov
    numeric_df = clean_df.copy()
    numeric_df['Species'].replace(['Adelie penguin', 'Chinstrap penguin', 'Gentoo penguin'], [0, 0.5, 1], inplace=True)
    numeric_df['Island'].replace(['Torgersen', 'Biscoe', 'Dream'], [0, 0.5, 1], inplace=True)
    numeric_df['Clutch Completion'].replace(['Yes', 'No'], [0, 1], inplace=True)
    numeric_df['Sex'].replace(['MALE', 'FEMALE'], [0, 1], inplace=True)

    #MinMax normalizácia
    numeric_df = min_max_normalization(numeric_df, numeric_columns)

    # Export datasets to .csv
    discrete_df.to_csv('../discrete_dataset.csv', index=False)
    numeric_df.to_csv('../numeric_normalized_dataset.csv', index=False)





