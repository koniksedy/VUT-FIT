"""
uploader.py
Module uploads xml data to MonodBD database.
UPA project
Authors: Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 05.10.2022
"""

import sys
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

        print("Uploading CZCanceledPTTMessage...  ", end="", file=sys.stderr)
        sys.stderr.flush()
        if CZCanceledPTTMessage_list:
            self.db["CZCanceledPTTMessage"].insert_many(CZCanceledPTTMessage_list)
        print("Done", file=sys.stderr)


if __name__ == "__main__":
    import downloader
    dwnl = downloader.Downloader("https://portal.cisjr.cz/pub/draha/celostatni/szdc/2022/", workdir="data")
    xml_files = dwnl.get()
    upl = Uploader()
    upl.upload(xml_files)
