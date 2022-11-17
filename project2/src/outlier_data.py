import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

if __name__ == "__main__":
    penguin_df = pd.read_csv("../penguins_lter.csv")

    # Skrátenie dĺžok názvok druhov tučniakov
    penguin_df.replace("Adelie Penguin (Pygoscelis adeliae)", "Adelie penguin", inplace=True)
    penguin_df.replace("Gentoo penguin (Pygoscelis papua)", "Gentoo penguin", inplace=True)
    penguin_df.replace("Chinstrap penguin (Pygoscelis antarctica)", "Chinstrap penguin", inplace=True)

    fig1, axes1 = plt.subplots(3, 1)
    fig1.suptitle("Visualisation of outlier values")

    sns.boxplot(ax=axes1[0], data=penguin_df, x='Body Mass (g)', y='Species')
    sns.boxplot(ax=axes1[1], data=penguin_df, x='Flipper Length (mm)', y='Species')
    sns.boxplot(ax=axes1[2], data=penguin_df, x='Culmen Length (mm)', y='Species')
    plt.tight_layout()

    plt.show()
    plt.close()

    fig1.savefig("../plots/outlier_values1.png")

    fig2, axes2 = plt.subplots(3, 1)
    fig2.suptitle("Visualisation of outlier values")

    sns.boxplot(ax=axes2[0], data=penguin_df, x='Culmen Depth (mm)', y='Species')
    sns.boxplot(ax=axes2[1], data=penguin_df, x='Delta 13 C (o/oo)', y='Species')
    sns.boxplot(ax=axes2[2], data=penguin_df, x='Delta 15 N (o/oo)', y='Species')
    plt.tight_layout()

    plt.show()
    plt.close()

    fig2.savefig("../plots/outlier_values2.png")

