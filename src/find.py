from cisjr import database_api


db = database_api.Database("cisjr", "mongodb://localhost:27017")

from_st = "Praha hl. n."
to_st = "Praha-Holešovice"
date = "20221018"

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
    {"$match": {
            "$expr": {
                "$eq": [
                    {
                        "$let": {
                            "vars": {
                                "a": {
                                    "$arrayElemAt": [
                                        {
                                            "$filter": {
                                                "input": "$CZPTTInformation.CZPTTLocation",
                                                "cond": {"$and": [{"$eq": ["$$this.Location.Name", from_st]}, {"$eq": ["$$this.TrainActivity", "0001"]}]}
                                            }
                                        },
                                        0
                                    ]
                                },
                                "b": {
                                    "$arrayElemAt": [
                                        {
                                            "$filter": {
                                                "input": "$CZPTTInformation.CZPTTLocation",
                                                "cond": {"$and": [{ "$eq": ["$$this.Location.Name", to_st]}, {"$eq": ["$$this.TrainActivity", "0033"]}]}
                                            }
                                        },
                                        0
                                    ]
                                }
                            },
                            "in": {
                                "$or": [
                                    {
                                        "$lt": ["$$a.TimingAtLocation.ALD.Time", "$$b.TimingAtLocation.ALA.Time"]
                                    },
                                    {
                                        "$lt": ["$$a.TimingAtLocation.ALD.Offset", "$$b.TimingAtLocation.ALA.Offset"]
                                    }
                                ]
                            }
                        }
                    },
                    True
                ]
            }
        }
    },
    {
        "$project": {
            # "_id": 1
            "CZPTTInformation.CZPTTLocation.Location.Name": 1,
            "CZPTTInformation.CZPTTLocation.TimingAtLocation.ALD.Time": 1
        }
    }
])
total_l = list(cursor)
l = total_l[0]
praha = list()
for l in total_l:
    for i in range(len(l["CZPTTInformation"]["CZPTTLocation"])):
        if l["CZPTTInformation"]["CZPTTLocation"][i]["Location"]["Name"] == "Praha hl. n.":
            praha.append((l["CZPTTInformation"]["CZPTTLocation"][i]["TimingAtLocation"]["ALD"]["Time"], l["_id"]))
        print(i, l["CZPTTInformation"]["CZPTTLocation"][i]["Location"]["Name"], end=" ")
        if "ALD" in l["CZPTTInformation"]["CZPTTLocation"][i]["TimingAtLocation"]:
            print(l["CZPTTInformation"]["CZPTTLocation"][i]["TimingAtLocation"]["ALD"]["Time"])
        else:
            print()
    print("-"*60)
for t in sorted(praha, key=lambda x: x[0]):
    print(t)
print(len(total_l))

db.close()