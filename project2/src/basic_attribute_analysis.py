import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def generateHistPlot(df, column_name: str, hue: str, legend: bool, remove_x: bool, multiple: str):
        plot = sns.histplot(df, x=column_name, hue=hue, legend=legend, multiple=multiple)

        if legend:
            sns.move_legend(plot, "upper left")

        if remove_x:
            plot.set(xticklabels=[])
            plot.set(xlabel=None)

        plt.tight_layout()
        return plot

def generateViolinPlot(df, column_name: str, y_axis: str, legend: bool):
    plot = sns.violinplot(df, x=column_name, y=y_axis, legend=legend)
    if legend:
        sns.move_legend(plot, "upper left")
    plt.tight_layout()
    return plot

def generateScatterPlot(df, column_name: str, y_axis: str, legend: bool, hue: str):
    plot = sns.scatterplot(df, x=column_name, y=y_axis, legend=legend, hue=hue)
    if legend:
        sns.move_legend(plot, "upper left")
    plt.tight_layout()
    return plot


def savePlot(plot, name):
    fig = plot.get_figure()
    fig.savefig("../plots/" + name + "_plot.pdf")


if __name__ == "__main__":
    penguin_df = pd.read_csv("../penguins_lter.csv")

    # Pandas settings
    pd.set_option('display.max_columns', None)
    pd.set_option('display.width', None)
    pd.set_option('display.max_colwidth', None)

    # Rozdelenie DF na viacero podla druhu pingvina
    adelie_df = penguin_df[penguin_df['Species'] == 'Adelie Penguin (Pygoscelis adeliae)']
    gentoo_df = penguin_df[penguin_df['Species'] == 'Gentoo penguin (Pygoscelis papua)']
    chinstrap_df = penguin_df[penguin_df['Species'] == 'Chinstrap penguin (Pygoscelis antarctica)']

    # Skrátenie dĺžok názvok druhov tučniakov
    penguin_df.replace("Adelie Penguin (Pygoscelis adeliae)", "Adelie penguin", inplace=True)
    penguin_df.replace("Gentoo penguin (Pygoscelis papua)", "Gentoo penguin", inplace=True)
    penguin_df.replace("Chinstrap penguin (Pygoscelis antarctica)", "Chinstrap penguin", inplace=True)

    # Table studyName
    # Tabulka studyName obsahuje nazvy studii, v ktorych boli vykonane merania. Nazov studie vsak nie je dolezity pre
    # dolovacie ulohy, a tak nebude brany v uvahu
    plot = generateHistPlot(penguin_df, "studyName", "studyName", False, False, "layer")
    savePlot(plot, "studyName")
    plt.close()


    #Table Species
    # Tabulka species delí záznamy do 3 druhov pingvinov. Táto hodnota bude používaná pre kategorizovanie záznamov.
    plot = generateHistPlot(penguin_df, "Species", "Species", False, False, "layer")
    savePlot(plot, "Species")
    plt.close()

    # Table Region
    plot = generateHistPlot(penguin_df, "Island", "Species", False, False, "dodge")
    #plt.setp(plot.get_legend().get_texts())
    savePlot(plot, "Island")
    plt.close()

    # Skipping Stage, individual ID tables

    # Clutch Completition table
    plot = generateHistPlot(penguin_df, "Clutch Completion", "Species", False, False, "dodge")
    savePlot(plot, "Clutch_completion")
    plt.close()


    # Culmen Length (mm)
    plot = generateViolinPlot(penguin_df, "Culmen Length (mm)", "Species", False)
    savePlot(plot, "Culmen_Length")
    plt.close()

    # Culmen Depth (mm)
    plot = generateViolinPlot(penguin_df, "Culmen Depth (mm)", "Species", False)
    savePlot(plot, "Culmen_Depth")
    plt.close()

    #Flipper Length (mm)
    plot = generateViolinPlot(penguin_df, "Flipper Length (mm)", "Species", False)
    savePlot(plot, "Flipper_Length")
    plt.close()

    #Body Mass (g)
    plot = generateViolinPlot(penguin_df, "Body Mass (g)", "Species", False)
    savePlot(plot, "Body_Mass")
    plt.close()

    #Delta 15 N (o/oo)
    plot = generateViolinPlot(penguin_df, "Delta 15 N (o/oo)", "Species", False)
    savePlot(plot, "Delta_15_N")
    plt.close()

    #Delta 13 C (o/oo)
    plot = generateViolinPlot(penguin_df, "Delta 13 C (o/oo)", "Species", False)
    savePlot(plot, "Delta_13_C")
    plt.close()

    # Overall statistics for Dataset
    #print(penguin_df.info())
    #print(penguin_df.describe())

    # Statistics for categorical data
    newlist = [penguin_df[column] for column in penguin_df if penguin_df[column].dtype == 'object']
    #print([item.value_counts() for item in newlist])

    # Date Egg















