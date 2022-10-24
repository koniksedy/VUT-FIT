"""
drop_db.py
Program that find trains going from the origin station FROM to the destination
station TO in a specified day.
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 24.10.2022
"""

import sys
from datetime import datetime
from cisjr import database_api



def find(from_st: str, to_st: str, date_time: datetime) -> list:
    """Returns list of trains going from the origin station from to the destination
        station to in a specified date.

    Args:
        from_st (str): Origin station.
        to_st (str): Destination station.
        date (datetime): Departure datetime.

    Returns:
        list: List of trains.
    """

    date = date_time.strftime("%Y%m%d")
    time = date_time.strftime("%H:%M:%S")

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
                    "$let": {
                        "vars": {
                            "locationFiltered": {
                                "$filter": {
                                    "input": "$CZPTTInformation.CZPTTLocation",
                                    "as": "locationArray" ,
                                    "cond": {"$eq": ["0001", "$$locationArray.TrainActivity"]}
                                    # "cond": {
                                    #     "$or": [
                                    #         {"$eq": ["0001", "$$locationArray.TrainActivity"]},
                                    #         {"$eq": ["0033", "$$locationArray.TrainActivity"]}
                                    #     ]
                                    # }
                                }
                            }
                        },
                        "in": {
                            "$and": [
                                {
                                    "$in": [from_st, "$$locationFiltered.Location.Name"]
                                },
                                {
                                    "$lt": [
                                        {
                                            "$indexOfArray": [
                                                "$$locationFiltered.Location.Name", from_st
                                            ]
                                        },
                                        {
                                            "$indexOfArray": [
                                                "$$locationFiltered.Location.Name", to_st
                                            ]
                                        }
                                    ]
                                }
                            ]
                        }
                    }
                }
            }
        },
        {
            "$project": {
                "_id": 1,
                "Locations": {
                    "$let": {
                        "vars": {
                            "locationFiltered": {
                                "$filter": {
                                    "input": "$CZPTTInformation.CZPTTLocation",
                                    "as": "locationArray" ,
                                    "cond": {"$eq": ["0001", "$$locationArray.TrainActivity"]}
                                    # "cond": {
                                    #     "$or": [
                                    #         {"$eq": ["0001", "$$locationArray.TrainActivity"]},
                                    #         {"$eq": ["0033", "$$locationArray.TrainActivity"]}
                                    #     ]
                                    # }
                                }
                            }
                        },
                        "in": {
                            "$let": {
                                "vars": {
                                    "a": {
                                        "$indexOfArray": [
                                            "$$locationFiltered.Location.Name", from_st
                                        ]
                                    },
                                    "b": {
                                        "$indexOfArray": [
                                            "$$locationFiltered.Location.Name", to_st
                                        ]
                                    }
                                },
                                "in": {
                                    "$slice" : [
                                        {
                                            "$map": {
                                                "input": "$$locationFiltered",
                                                "as": "location",
                                                "in": {
                                                    "Name": "$$location.Location.Name",
                                                    # "Activity": "$$location.TrainActivity",
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
            }
        }
    ])

    data = list(filter(lambda x: x["Locations"][0]["ALD"] >= time,
                       sorted(cursor, key=lambda x: x["Locations"][0]["ALD"])))
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
        print("  DATE_TIME     Date and time in the format yyyy-mm-ddTHH:MM:SS", file=sys.stderr)
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
    try:
        date_time = datetime.strptime(sys.argv[3], "%Y-%m-%dT%H:%M:%S")
    except ValueError:
        print("Bad date time format.", file=sys.stderr)
        exit(1)

    # TODO visualizer
    total_l = find(from_st, to_st, date_time)
    for l in total_l:
        # print(l["_id"])
        for t in l["Locations"]:
            print(t)
        print("-"*80)
    # print(len(total_l))


if __name__ == "__main__":
    main()
