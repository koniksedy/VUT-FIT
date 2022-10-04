"""
uploader.py
Module uploads xml data to MonodBD database.
UPA project
Authors: Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 03.10.2022
"""

# from . import parser
import parser
import pymongo
import xml.etree.ElementTree as ET


class Uploader:
    def __init__(self) -> None:
        connection_string = f"mongodb://localhost:27017"
        self._client = pymongo.MongoClient(connection_string)
        self.db = self.client["cisjr"]
        pass
    
    def close(self) -> None:
        self._client.close()

    def upload(self, xml_files: list) -> None:
        parser_ob = parser.Parser()
                
        CZPTTCISMessage_list, CZCanceledPTTMessage_list, Location_list = parser_ob.parse(xml_files)

if __name__ == "__main__":
    upl = Uploader()
    upl.upload(["data/zips/PA_0054_KT----10156A_00_2022.xml"])

