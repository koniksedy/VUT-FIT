"""
drop_db.py
Program that find trains going from the origin station FROM to the destination
station TO in a specified day.
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 18.11.2022
"""

import sys
from cisjr import database_api


def find(from_st: str, to_st: str, date: str) -> list:
    """Returns list of trains going from the origin station from to the destination
        station to in a specified date.

    Args:
        from_st (str): Origin station.
        to_st (str): Destination station.
        date (str): Departure date.

    Returns:
        list: List of trains.
    """

    db = database_api.Database("cisjr", "mongodb://localhost:27017")

    cursor = db.api_aggregate("CZPTTCISMessage", [
        {
            "$match": {
                "CZPTTInformation.CZPTTLocation.Location.Name": from_st
            }
        },
        {
            "$match": {
                "CZPTTInformation.CZPTTLocation.Location.Name": to_st
            }
        },
        {
            "$match": {
                "CZPTTInformation.PlannedCalendar.Valid": date
            }
        },
        {
            "$match": {
                "$expr": {
                    "$lt": [
                        {
                            "$indexOfArray": [
                                "$CZPTTInformation.CZPTTLocation.Location.Name", from_st
                            ]
                        },
                        {
                            "$indexOfArray": [
                                "$CZPTTInformation.CZPTTLocation.Location.Name", to_st
                            ]
                        }
                    ]
                }
            }
        },
        {
            "$project": {
                "_id": 0,
                "Locations": {
                    "$let": {
                        "vars": {
                            "a": {
                                "$indexOfArray": [
                                    "$CZPTTInformation.CZPTTLocation.Location.Name", from_st
                                ]
                            },
                            "b": {
                                "$indexOfArray": [
                                    "$CZPTTInformation.CZPTTLocation.Location.Name", to_st
                                ]
                            }
                        },
                        "in": {
                            "$slice" : [
                                {
                                    "$map": {
                                        "input": "$CZPTTInformation.CZPTTLocation",
                                        "as": "location",
                                        "in": {
                                            "Name": "$$location.Location.Name",
                                            "Activity": "$$location.TrainActivity",
                                            "ALD":  {
                                                "$dateToString": {
                                                    "format": "%H:%M:%S",
                                                    "date": "$$location.TimingAtLocation.ALD.Time"
                                                }
                                            },
                                            "ALA": {
                                                "$dateToString": {
                                                    "format": "%H:%M:%S",
                                                    "date": "$$location.TimingAtLocation.ALA.Time"
                                                }
                                            }
                                        }
                                    }
                                },
                                "$$a", {"$subtract": [{"$add": [1, "$$b"]}, "$$a"]}
                            ]
                        }
                    }
                }
            }
        }
    ])

    data = list(cursor)
    db.close()

    return data


def main():
    if {"-h", "--help"}.intersection(sys.argv):
        print("usage: find.py FROM TO DATE_TIME", file=sys.stderr)
        print("", file=sys.stderr)
        print("  FROM          A name of the origin station. A name with spaces must be", file=sys.stderr)
        print("                enclosed in quotation marks.", file=sys.stderr)
        print("  TO            A Name of the destination station. A name with spaces must be", file=sys.stderr)
        print("                enclosed in quotation marks.", file=sys.stderr)
        print("  DATE_TIME     Date and time in the format yyyymmddHHMMSS", file=sys.stderr)
        print("                or yyyymmdd", file=sys.stderr)
        sys.exit(0)

    if len(sys.argv) != 4:
        print("Bad program arguments. Try --help.", file=sys.stderr)
        print(list(sys.argv))
        sys.exit(1)

    if len(sys.argv[3]) < 8:
        print("Bad date format. Try --help.")
        sys.exit(1)


    from_st = sys.argv[1]
    to_st = sys.argv[2]
    date = sys.argv[3][:8]

    # TODO visualizer
    total_l = find(from_st, to_st, date)
    for l in total_l:
        for t in l["Locations"]:
            print(t)
        print("-"*80)


if __name__ == "__main__":
    main()
