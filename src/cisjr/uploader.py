"""
uploader.py
Module uploads xml data to MonodBD database.
UPA project
Authors: Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 05.10.2022
"""

import os
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
                        "Identifiers.PlannedTransportIdentifiers": message["PlannedTransportIdentifiers"],
                        "CZPTTInformation.PlannedCalendar.ValidityPeriod.StartDateTime": {
                                "$lte": message["PlannedCalendar"]["ValidityPeriod"]["StartDateTime"]
                            },
                        "CZPTTInformation.PlannedCalendar.ValidityPeriod.EndDateTime": {
                                "$gte": message["PlannedCalendar"]["ValidityPeriod"]["EndDateTime"]
                            }
                    }
                }
            ]
        )

        for data in data_cursor:
            date_start = data["CZPTTInformation"]["PlannedCalendar"]["ValidityPeriod"]["EndDateTime"].date()
            date_cancel = message["PlannedCalendar"]["ValidityPeriod"]["StartDateTime"].date()
            date_offset = (date_cancel - date_start).days

            bitmap = data["CZPTTInformation"]["PlannedCalendar"]["BitmapDays"]
            bitmap_closed = message["PlannedCalendar"]["BitmapDays"]
            for i in range(len(bitmap_closed)):
                j = i + date_offset
                if bitmap[j] == "1" and bitmap_closed[i] == "1":
                    bitmap[j] = "2"
                elif bitmap[j] == "2" and bitmap_closed[i] == "0":
                    bitmap[j] = "1"

            self.db["CZPTTCISMessage"].update_one({"_id": data["_id"]},
                                                  {
                                                      "$set":
                                                          {
                                                              "CZPTTInformation.PlannedCalendar.BitmapDays": bitmap
                                                          }
                                                  })

    def upload(self, xml_files: list) -> None:
        """Calls parser and uploads all file in xml_files to mongo database.

        Args:
            xml_files (list): List of xml_files.
        """
        parser_ob = parser.Parser()

        CZPTTCISMessage_list, CZCanceledPTTMessage_list, Location_list = parser_ob.parse(xml_files)

        print("Uploading Location...  ", end="", file=sys.stderr)
        sys.stderr.flush()
        if Location_list:
            self.db["Location"].insert_many(Location_list)
        print("Done", file=sys.stderr)

        print("Uploading CZPTTCISMessage...  ", end="", file=sys.stderr)
        sys.stderr.flush()
        if CZPTTCISMessage_list:
            self.db["CZPTTCISMessage"].insert_many(CZPTTCISMessage_list)
        print("Done", file=sys.stderr)

        if CZCanceledPTTMessage_list:
            CZCanceledPTTMessage_list.sort(key=lambda x: x["CZPTTCancelation"])
            for message in tqdm.tqdm(CZCanceledPTTMessage_list,
                                     desc="Sending CZCanceledPTTMessage...",
                                     ascii=False,
                                     ncols=os.get_terminal_size().columns,
                                     file=sys.stderr):
                self.send_CZCanceledPTTMessage(message)


if __name__ == "__main__":
    import downloader
    dwnl = downloader.Downloader("https://portal.cisjr.cz/pub/draha/celostatni/szdc/2022/", workdir="data")
    xml_files = dwnl.get()
    upl = Uploader()
    upl.upload(xml_files)
