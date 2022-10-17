"""
uploader.py
Module uploads xml data to MonodBD database.
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 12.10.2022
"""

import sys
import tqdm
import pymongo
from src.cisjr import database_api

if __name__ == "__main__":
    import parser
else:
    from . import parser


class Uploader:
    def __init__(self) -> None:
        # TODO init with database class
        connection_string = f"mongodb://localhost:27017"
        self.db = database_api.Database("cisjr", connection_string)

    def close(self) -> None:
        self.db.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback) -> None:
        self.close()

    def _send_CZCanceledPTTMessage(self, message: dict) -> None:

        aggregation_string = [
            {
                "$match": {
                    "Identifiers.PlannedTransportIdentifiers.PA.Core": message["PlannedTransportIdentifiers"]["PA"][
                        "Core"],
                    "Identifiers.PlannedTransportIdentifiers.TR.Core": message["PlannedTransportIdentifiers"]["TR"][
                        "Core"]
                }
            }
        ]

        data_cursor = self.db.api_aggregate("CZPTTCISMessage", query=aggregation_string)

        # TODO Remove when functionality verified
        '''data_cursor = self.db["CZPTTCISMessage"].aggregate(
            [
                {
                    "$match": {
                        "Identifiers.PlannedTransportIdentifiers.PA.Core": message["PlannedTransportIdentifiers"]["PA"]["Core"],
                        "Identifiers.PlannedTransportIdentifiers.TR.Core": message["PlannedTransportIdentifiers"]["TR"]["Core"]
                    }
                }
            ]

        )'''

        for data in data_cursor:
            d = message["PlannedCalendar"]["ValidityPeriod"]["StartDateTime"].strftime("%Y%m%d")
            de = message["PlannedCalendar"]["ValidityPeriod"]["EndDateTime"].strftime("%Y%m%d")

            canceled_new = message["PlannedCalendar"]["Valid"]
            canceled_old = list(
                filter(lambda x: x < d or x > de, data["CZPTTInformation"]["PlannedCalendar"]["Canceled"]))
            canceled_update = canceled_old + canceled_new

            mongo_query = {
                "$set":
                    {
                        "CZPTTInformation.PlannedCalendar.Canceled": canceled_update
                    }
            }

            mongo_filter = {"_id": data["_id"]}
            self.db.api_update_one("CZPTTCISMessage", query=mongo_query, mongo_filter=mongo_filter)

            # TODO Remove when functionality verified
            '''self.db["CZPTTCISMessage"].update_one({"_id": data["_id"]},
                                                  {
                                                      "$set":
                                                          {
                                                              "CZPTTInformation.PlannedCalendar.Canceled": canceled_update
                                                          }
                                                  })'''

    def upload(self, xml_files: list, force=False, n_threads: int = 1) -> None:
        """Calls parser and uploads all file in xml_files to mongo database.

        Args:
            xml_files (list): List of xml_files.
            force (bool, optional): Parse only new files if False. Defaults False.
            n_threads (int, optional): If greater than 1, the parallel downloading with
                                       n_threads is performed. Defaults 1.
        """

        # Parsing (can be done in parallel)
        if n_threads == 1:
            parser_ob = parser.Parser()
            CZPTTCISMessage_list, CZCanceledPTTMessage_list, Location_list = parser_ob.parse(xml_files)
        else:
            from pqdm.processes import pqdm
            print("Parallel parsing...", file=sys.stderr)
            parallel_results = pqdm(xml_files, parser.Parser().parse_single, n_jobs=n_threads)
            CZPTTCISMessage_list = [x[0] for x in parallel_results if x[0] is not None]
            CZCanceledPTTMessage_list = [x[1] for x in parallel_results if x[1] is not None]
            Location_list = {y[0]: y[1] for x in parallel_results if x[2] is not None for y in x[2]}.items()
            print("Parallel parsing done.", file=sys.stderr)

        # Uploading Location and generating location _ids.
        # Uses database_api.
        Location_ids = dict()
        if Location_list:
            for key, location in tqdm.tqdm(Location_list,
                                           desc="Uploading Location...",
                                           ascii=False,
                                           file=sys.stderr):
                if location["LocationSubsidiaryIdentification"] is None:

                    mongo_query = {
                        "PrimaryLocationName": location["PrimaryLocationName"],
                        "LocationPrimaryCode": location["LocationPrimaryCode"],
                        "LocationSubsidiaryIdentification": location["LocationSubsidiaryIdentification"]
                    }

                    orig_location = self.db.api_find_one("Location", query=mongo_query)

                    # TODO Remove when functionality verified
                    '''orig_location = self.db["Location"].find_one(
                        {
                            "PrimaryLocationName": location["PrimaryLocationName"],
                            "LocationPrimaryCode": location["LocationPrimaryCode"],
                            "LocationSubsidiaryIdentification": location["LocationSubsidiaryIdentification"]
                        }
                    )'''
                else:
                    mongo_query = {
                        "PrimaryLocationName": location["PrimaryLocationName"],
                        "LocationPrimaryCode": location["LocationPrimaryCode"],
                        "LocationSubsidiaryIdentification.LocationSubsidiaryCode":
                            location["LocationSubsidiaryIdentification"]["LocationSubsidiaryCode"]
                    }

                    orig_location = self.db.api_find_one("Location", query=mongo_query)

                    # TODO Remove when functionality verified
                    '''orig_location = self.db["Location"].find_one(
                        {
                            "PrimaryLocationName": location["PrimaryLocationName"],
                            "LocationPrimaryCode": location["LocationPrimaryCode"],
                            "LocationSubsidiaryIdentification.LocationSubsidiaryCode":
                                location["LocationSubsidiaryIdentification"]["LocationSubsidiaryCode"]
                        }
                    )'''
                if orig_location is not None:
                    Location_ids[key] = orig_location["_id"]
                else:
                    # TODO Remove when functionality verified
                    # r = self.db["Location"].insert_one(location)
                    r = self.db.api_insert_one("Location", data=location)
                    Location_ids[key] = r.inserted_id

        # Uploading CZPTTCISMessage
        if CZPTTCISMessage_list:
            CZPTTCISMessage_list.sort(key=lambda x: x["CZPTTCreation"])
            # TODO Remove when functionality verified
            # result = self.db["DBInfo"].find_one({"_id": "LastUpdate"})

            mongo_query = {"_id": "LastUpdate"}
            result = self.db.api_find_one("DBInfo", query=mongo_query)
            last_update = None if result is None else result["Date"]
            for message in tqdm.tqdm(CZPTTCISMessage_list,
                                     desc="Uploading CZPTTCISMessage...",
                                     ascii=False,
                                     file=sys.stderr):

                for location in message["CZPTTInformation"]["CZPTTLocation"]:
                    old_id = location["Location"]["Location_id"]
                    location["Location"]["Location_id"] = Location_ids[old_id]

                if force or (last_update is not None and last_update >= message["CZPTTCreation"]):
                    # Update only newly created records or it the force is set.
                    # Uses database_api.

                    # TODO Remove when functionality verified
                    '''self.db["CZPTTCISMessage"].update_one(
                        {
                            "Identifiers": message["Identifiers"],
                            "CZPTTInformation.PlannedCalendar.ValidityPeriod":
                                message["CZPTTInformation"]["PlannedCalendar"]["ValidityPeriod"]
                        }, {
                            "$set": {},
                            "$setOnInsert": message
                        }, upsert=True
                    )'''
                    mongo_query = {
                                      "$set": {},
                                      "$setOnInsert": message
                                  }
                    mongo_filter = {
                                      "Identifiers": message["Identifiers"],
                                      "CZPTTInformation.PlannedCalendar.ValidityPeriod":
                                          message["CZPTTInformation"]["PlannedCalendar"]["ValidityPeriod"]
                                  }
                    self.db.api_update_one("CZPTTCISMessage", query=mongo_query, mongo_filter=mongo_filter, upsert=True)
                else:
                    last_update = message["CZPTTCreation"]
                    # TODO Remove when functionality validated
                    # self.db["CZPTTCISMessage"].insert_one(message)

                    self.db.api_insert_one("CZPTTCISMessage", data=message)

            # TODO Remove when functionality validated
            # self.db["DBInfo"].update_one({"_id": "LastUpdate"}, {"$set": {"Date": last_update}}, upsert=True)
            mongo_filter = {"_id": "LastUpdate"}
            mongo_query = {"$set": {"Date": last_update}}

            self.db.api_update_one("DBInfo", mongo_filter=mongo_filter, query=mongo_query, upsert=True)

        # Sending CZCanceledPTTMessage_list
        if CZCanceledPTTMessage_list:
            CZCanceledPTTMessage_list.sort(key=lambda x: x["CZPTTCancelation"])
            for message in tqdm.tqdm(CZCanceledPTTMessage_list,
                                     desc="Sending CZCanceledPTTMessage...",
                                     ascii=False,
                                     file=sys.stderr):
                self._send_CZCanceledPTTMessage(message)


def main():
    import downloader
    from argparse import ArgumentParser

    # Parse args
    opt_parser = ArgumentParser()
    opt_parser.add_argument("workdir",
                            help="Name of a working directory with a folder /zips to upload.")
    opt_parser.add_argument("-p", "--parallel", type=int, metavar="N_THR", default=1,
                            help=("Parse data in 0 < N_THREADS parallel threads.\n" +
                                  "WARNING high number can lead to the lag in QUEUEING TASKS"))
    opt_parser.add_argument("-f", "--force", action="store_true",
                            help="Updates already uploaded files.")
    args = opt_parser.parse_args()

    # Parsing + Uploading
    down = downloader.Downloader("https://portal.cisjr.cz/pub/draha/celostatni/szdc/2022/", workdir=args.workdir)
    xml_files = down.get()
    upl = Uploader()
    upl.upload(xml_files, force=args.force, n_threads=args.parallel)


if __name__ == "__main__":
    main()
