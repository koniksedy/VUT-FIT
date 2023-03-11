"""
uploader.py
Module uploads xml data to MonodBD database.
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 19.10.2022
"""

import sys
import tqdm
from . import database_api
from . import parser


class Uploader:
    def __init__(self) -> None:
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
                    "Identifiers.PlannedTransportIdentifiers.TR": message["PlannedTransportIdentifiers"]["TR"]
                },
                "$match": {
                    "Identifiers.PlannedTransportIdentifiers.PA": message["PlannedTransportIdentifiers"]["PA"]
                }
            }
        ]

        data_cursor = self.db.api_aggregate("CZPTTCISMessage", aggregation_string)

        for data in data_cursor:
            ds = message["PlannedCalendar"]["ValidityPeriod"]["StartDateTime"].strftime("%Y%m%d")
            de = message["PlannedCalendar"]["ValidityPeriod"]["EndDateTime"].strftime("%Y%m%d")

            cancel_message = set(message["PlannedCalendar"]["Valid"])
            cancel = set(data["CZPTTInformation"]["PlannedCalendar"]["Canceled"])
            valid = set(data["CZPTTInformation"]["PlannedCalendar"]["Valid"])
            focused_cancel = set(filter(lambda x: ds <= x and x <= de, cancel))

            if "Generated" not in message:
                # Reset cancel data in a period
                valid.update(focused_cancel)
                cancel.difference_update(focused_cancel)

            valid.difference_update(cancel_message)
            cancel.update(cancel_message)

            mongo_query = {
                "$set":
                    {   "CZPTTInformation.PlannedCalendar.Valid": list(valid),
                        "CZPTTInformation.PlannedCalendar.Canceled": list(cancel)
                    }
            }

            mongo_filter = {"_id": data["_id"]}
            self.db.api_update_one("CZPTTCISMessage", mongo_filter, mongo_query)

    def _generate_CZCanceledPTTMessage(self, message: dict) -> list:
        canceled_messages = list()
        if "RelatedPlannedTransportIdentifiers" not in message["Identifiers"]:
            return canceled_messages

        mongo_query = {
            "Identifiers.PlannedTransportIdentifiers.PA": message["Identifiers"]["RelatedPlannedTransportIdentifiers"]["PA"]
        }

        for data in self.db.api_find("CZPTTCISMessage", mongo_query):
            cancel_message = {
                "PlannedTransportIdentifiers": data["Identifiers"]["PlannedTransportIdentifiers"],
                "Generated": True,
                "CZPTTCancelation": message["CZPTTCreation"],
                "PlannedCalendar": {
                    "Valid": message["CZPTTInformation"]["PlannedCalendar"]["Valid"],
                    "ValidityPeriod": message["CZPTTInformation"]["PlannedCalendar"]["ValidityPeriod"]
                }
            }
            canceled_messages.append(cancel_message)

        return canceled_messages

    def upload(self, xml_files: list, n_threads: int = 1) -> None:
        """Calls parser and uploads all file in xml_files to mongo database.

        Args:
            xml_files (list): List of xml_files.
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

                    orig_location = self.db.api_find_one("Location", mongo_query)
                else:
                    mongo_query = {
                        "PrimaryLocationName": location["PrimaryLocationName"],
                        "LocationPrimaryCode": location["LocationPrimaryCode"],
                        "LocationSubsidiaryIdentification.LocationSubsidiaryCode":
                            location["LocationSubsidiaryIdentification"]["LocationSubsidiaryCode"]
                    }

                    orig_location = self.db.api_find_one("Location", mongo_query)

                if orig_location is not None:
                    Location_ids[key] = orig_location["_id"]
                else:
                    r = self.db.api_insert_one("Location", location)
                    Location_ids[key] = r.inserted_id

        # Create CZPTTCISMessage indexes
        print("Creating CZPTTCISMessage indexes... ", file=sys.stderr, end="")
        sys.stderr.flush()
        self.db.api_create_index("CZPTTCISMessage", "Identifiers.PlannedTransportIdentifiers.TR")
        self.db.api_create_index("CZPTTCISMessage", "Identifiers.PlannedTransportIdentifiers.PA")
        print("Done", file=sys.stderr)

        # Uploading CZPTTCISMessage
        if CZPTTCISMessage_list:
            CZPTTCISMessage_list.sort(key=lambda x: x["CZPTTCreation"])
            for message in tqdm.tqdm(CZPTTCISMessage_list,
                                     desc="Uploading CZPTTCISMessages...",
                                     ascii=False,
                                     file=sys.stderr):

                for location in message["CZPTTInformation"]["CZPTTLocation"]:
                    old_id = location["Location"]["Location_id"]
                    location["Location"]["Location_id"] = Location_ids[old_id]

                mongo_filter = {
                                    "Identifiers.PlannedTransportIdentifiers.TR": message["Identifiers"]["PlannedTransportIdentifiers"]["TR"],
                                    "Identifiers.PlannedTransportIdentifiers.PA": message["Identifiers"]["PlannedTransportIdentifiers"]["PA"]
                                }
                self.db.api_replace_one("CZPTTCISMessage", mongo_filter, message, upsert=True)

        # Generate missing CZCanceledPTTMessages
        for message in tqdm.tqdm(CZPTTCISMessage_list,
                                 desc="Generating CZCanceledPTTMessages...",
                                 ascii=False,
                                 file=sys.stderr):
            cancel_messages = self._generate_CZCanceledPTTMessage(message)
            if cancel_messages:
                CZCanceledPTTMessage_list.extend(cancel_messages)

        # Sending CZCanceledPTTMessage_list
        if CZCanceledPTTMessage_list:
            CZCanceledPTTMessage_list.sort(key=lambda x: x["CZPTTCancelation"])
            for message in tqdm.tqdm(CZCanceledPTTMessage_list,
                                        desc="Sending CZCanceledPTTMessages...",
                                        ascii=False,
                                        file=sys.stderr):
                self._send_CZCanceledPTTMessage(message)

        # Create CZPTTCISMessage location indexes
        print("Creating CZPTTCISMessage location indexes... ", file=sys.stderr, end="")
        sys.stderr.flush()
        self.db.api_create_index("CZPTTCISMessage", "CZPTTInformation.CZPTTLocation.Location.Name")
        print("Done", file=sys.stderr)


def main():
    from . import downloader
    from argparse import ArgumentParser

    # Parse args
    opt_parser = ArgumentParser()
    opt_parser.add_argument("workdir",
                            help="Name of a working directory with a folder /zips to upload.")
    opt_parser.add_argument("-p", "--parallel", type=int, metavar="N_THR", default=1,
                            help=("Parse data in 0 < N_THREADS parallel threads.\n" +
                                  "WARNING high number can lead to the lag QUEUEING TASKS"))
    args = opt_parser.parse_args()

    # Parsing + Uploading
    down = downloader.Downloader("https://portal.cisjr.cz/pub/draha/celostatni/szdc/2022/", workdir=args.workdir)
    xml_files = down.get()
    upl = Uploader()
    upl.upload(xml_files, n_threads=args.parallel)


if __name__ == "__main__":
    main()
