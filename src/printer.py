import json
from airium import Airium


class Printer:
    def __init__(self, paths):
        self.paths = paths

    def print_one(self, path: list):
        """
        Iterates over list of one path format and print all stations and Time to stdout.

        Args:
            path(list): List of dictionaries with destionation and Time.

        """
        for station in path:
            if 'ALD' in station and station['ALD'] is not None:
                time = station['ALD']
            elif 'ALA' in station and station['ALA'] is not None:
                time = station['ALA']
            else:
                time = ''

            if station == path[0]:
                print('{: >25} {: >50}'.format(station['Name'], time))
            elif station == path[-1]:
                print('{: >25} {: >50}'.format(station['Name'], time))
            else:
                print('{: >50} {: >20}'.format(station['Name'], time))

    def print_all(self):
        for i in range(len(self.paths)):
            print('-' * 30, 'Option ', i, '-' * 36)
            self.print_one(self.paths[i]['Locations'])

            print('-' * 76)
            print('\n\n')

    @staticmethod
    def get_first(stations):
        return stations[0]

    @staticmethod
    def get_last(stations):
        return stations[-1]

    @staticmethod
    def get_interstations(stations):
        return stations[1:-1]

    def remove_duplicates(self):
        new_list = []
        for path in self.paths:
            if path not in new_list:
                new_list.append(path)
        self.paths = new_list
        return

    def get_html(self):
        a = Airium()
        a('<!DOCTYPE html>')
        with a.html(lang="sk"):
            with a.head():
                a.meta(charset="utf-8")
                a.title(_t="Vysledky")
                a.link(rel="stylesheet", type="text/css", href="style.css")

            with a.body():
                # Start station
                with a.div(klass="center"):
                    for path in self.paths:
                        with a.p():
                            with a.div(style="background:#a9d6eb"):
                                if len(path['Locations']) > 0:
                                    first = self.get_first(path['Locations'])
                                    with a.div(klass="row"):
                                        with a.div(klass="column"):
                                            a(first['Name'])
                                        with a.div(klass="column"):
                                            if 'ALD' in first and first['ALD'] is not None:
                                                a(first['ALD'])
                                            elif 'ALA' in first and first['ALA'] is not None:
                                                a(first['ALA'])
                                    with a.details(style="background:#9dc8ff"):
                                        with a.summary():
                                            pass
                                        interstations = self.get_interstations(path['Locations'])
                                        for station in interstations:
                                            with a.div(klass="row"):
                                                with a.div(klass="inner-column"):
                                                    a(station['Name'])
                                                with a.div(klass="inner-column"):
                                                    if 'ALD' in station and station['ALD'] is not None:
                                                        a(station['ALD'])
                                                    elif 'ALA' in station and station['ALA'] is not None:
                                                        a(station['ALA'])
                                    last = self.get_last(path['Locations'])
                                    with a.div(klass="row"):
                                        with a.div(klass="column"):
                                            a(last['Name'])
                                        with a.div(klass="column"):
                                            if 'ALA' in last and last['ALA'] is not None:
                                                a(last['ALA'])
                                            elif 'ALD' in last and last['ALD'] is not None:
                                                a(last['ALD'])

        return str(a)


if __name__ == "__main__":
    # TODO: use list of list of dicts, from result
    paths = []
    printer = Printer(paths)
    printer.remove_duplicates()
    printer.print_all()
    html = printer.get_html()

    with open("output.html", "w") as text_file:
        text_file.write(html)
