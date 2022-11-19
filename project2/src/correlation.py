import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

if __name__ == "__main__":
    penguin_df = pd.read_csv("../penguins_lter.csv")
    x_vars = ['Culmen Length (mm)','Culmen Depth (mm)','Flipper Length (mm)','Body Mass (g)', 'Delta 15 N (o/oo)','Delta 13 C (o/oo)']
    y_vars = ['Culmen Length (mm)','Culmen Depth (mm)','Flipper Length (mm)','Body Mass (g)', 'Delta 15 N (o/oo)','Delta 13 C (o/oo)']

    # Odstranenie nepodtrebnych hodnot
    fig = sns.pairplot(penguin_df, x_vars=x_vars, y_vars=y_vars, hue='Species', plot_kws={'line_kws':{'color':'red'}}, kind='reg')
    sns.move_legend(fig, "upper center", bbox_to_anchor=(0.5, -0.01), ncol = 3, fontsize = 20)
    plt.tight_layout()
    plt.show()
    fig.savefig("../plots/distribution_correlation_plot.pdf")
    plt.close()
