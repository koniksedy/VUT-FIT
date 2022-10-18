from cisjr import database_api


def find(from_st: str, to_st: str, date: str):

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
    from_st = "Praha hl. n."
    to_st = "Pardubice hl. n."
    date = "20221018"

    total_l = find(from_st, to_st, date)
    for l in total_l:
        for t in l["Locations"]:
            print(t)
        print("-"*80)


if __name__ == "__main__":
    main()
