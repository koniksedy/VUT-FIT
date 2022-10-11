"""
uploader.py
Module uploads xml data to MonodBD database.
UPA project
Authors: Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 05.10.2022
"""

import os
from symbol import term
import sys
import tqdm
import parser
import pymongo


class Uploader:
    def __init__(self) -> None:
        # TODO init with database class
        connection_string = f"mongodb://localhost:27017"
        self._client = pymongo.MongoClient(connection_string)
        self.db = self._client["cisjr"]

    def close(self) -> None:
        self._client.close()

    def __exit__(self) -> None:
        self.close()

    def send_CZCanceledPTTMessage(self, message: dict) -> None:
        data_cursor = self.db["CZPTTCISMessage"].aggregate(
            [
                {
                    "$match": {
                        "Identifiers.PlannedTransportIdentifiers": message["PlannedTransportIdentifiers"]
                    }
                }
            ]
        )

        for data in data_cursor:
            d = message["PlannedCalendar"]["ValidityPeriod"]["StartDateTime"].strftime("%Y%m%d")
            de = message["PlannedCalendar"]["ValidityPeriod"]["EndDateTime"].strftime("%Y%m%d")

            canceled_new = message["PlannedCalendar"]["Valid"]
            canceled_old = list(filter(lambda x: x < d or x > de, data["CZPTTInformation"]["PlannedCalendar"]["Canceled"]))
            canceled_update = canceled_old + canceled_new

            self.db["CZPTTCISMessage"].update_one({"_id": data["_id"]},
                                                  {
                                                      "$set":
                                                          {
                                                              "CZPTTInformation.PlannedCalendar.Canceled": canceled_update
                                                          }
                                                  })

    def upload(self, xml_files: list) -> None:
        """Calls parser and uploads all file in xml_files to mongo database.

        Args:
            xml_files (list): List of xml_files.
        """
        parser_ob = parser.Parser()

        CZPTTCISMessage_list, CZCanceledPTTMessage_list, Location_list = parser_ob.parse(xml_files)

        # Uploading Location
        print("Uploading Location...  ", end="", file=sys.stderr)
        sys.stderr.flush()
        if Location_list:
            self.db["Location"].insert_many(Location_list)
        print("Done", file=sys.stderr)

        # Uploading CZPTTCISMessage
        print("Uploading CZPTTCISMessage...  ", end="", file=sys.stderr)
        sys.stderr.flush()
        if CZPTTCISMessage_list:
            self.db["CZPTTCISMessage"].insert_many(CZPTTCISMessage_list)
        print("Done", file=sys.stderr)

        # Sending CZCanceledPTTMessage_list

        try:
            terminal_w = os.get_terminal_size().columns
        except Exception:
            terminal_w = 80

        if CZCanceledPTTMessage_list:
            CZCanceledPTTMessage_list.sort(key=lambda x: x["CZPTTCancelation"])
            for message in tqdm.tqdm(CZCanceledPTTMessage_list,
                                     desc="Sending CZCanceledPTTMessage...",
                                     ascii=False,
                                     ncols=terminal_w,
                                     file=sys.stderr):
                self.send_CZCanceledPTTMessage(message)


if __name__ == "__main__":
    import downloader
    dwnl = downloader.Downloader("https://portal.cisjr.cz/pub/draha/celostatni/szdc/2022/", workdir="data")
    xml_files = dwnl.get()
    upl = Uploader()
    upl.upload(xml_files)
