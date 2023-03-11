"""
attribute_distribution.py
Using matrix of graphs, program visualizes the dependencies of various numerical attributes on other attributes.
The plot is then saved in the plots folder.
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 19.11.2022
"""

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

if __name__ == "__main__":
    penguin_df = pd.read_csv("../penguins_lter.csv")
    x_vars = ['Culmen Length (mm)','Culmen Depth (mm)','Flipper Length (mm)','Body Mass (g)', 'Delta 15 N (o/oo)','Delta 13 C (o/oo)']
    y_vars = ['Culmen Length (mm)','Culmen Depth (mm)','Flipper Length (mm)','Body Mass (g)', 'Delta 15 N (o/oo)','Delta 13 C (o/oo)']

    # Odstranenie nepodtrebnych hodnot
    fig = sns.pairplot(penguin_df, x_vars=x_vars, y_vars=y_vars, hue='Species')
    sns.move_legend(fig, "upper center", bbox_to_anchor=(0.5, -0.01), ncol = 3, fontsize = 20)
    plt.tight_layout()
    plt.show()
    fig.savefig("../plots/distribution_plot.pdf")
    plt.close()