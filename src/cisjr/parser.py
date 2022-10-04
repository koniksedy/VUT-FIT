"""
parser.py
Module parses xml data.
UPA project
Authors: Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 04.10.2022
"""

# TODO data types


import xml.etree.ElementTree as ET
from datetime import datetime


class Parser:
    def __init__(self) -> None:
        self.Locations_dict = dict()

    
    def parse(self, xml_files: str) -> dict:
        CZPTTCISMessage_list = list()
        CZCanceledPTTMessage_list = list()

        for xml_f in xml_files:
            tree = ET.parse(xml_f)
            root = tree.getroot()
            if root.tag == "CZPTTCISMessage":
                CZPTTCISMessage_list.append(self._parse_CZPTTCISMessage(root))
            elif root.tag == "CZCanceledPTTMessage":
                CZCanceledPTTMessage_list.append(self._parse_CZCanceledPTTMessage(root))
            else:
                raise ValueError(f"Unsupported xml tag {root.tag}")

        Location_list = list(self.Locations_dict.values())
        return CZPTTCISMessage_list, CZCanceledPTTMessage_list, Location_list

    def _parse_CZPTTCISMessage(self, xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "Identifiers":
                out["Identifiers"] = self._parse_Identifiers(child)
            elif child.tag == "CZPTTCreation":
                out["CZPTTCreation"] = self._parse_CZPTTCreation(child)
            elif child.tag == "CZPTTHeader":
                out["CZPTTHeader"] = self._parse_CZPTTHeader(child)
            elif child.tag == "CZPTTInformation":
                assert "CZPTTInformation" not in out
                out["CZPTTInformation"] = self._parse_CZPTTInformation(child)
            elif child.tag == "NetworkSpecificParameter":
                if "NetworkSpecificParameter" not in out:
                    out["NetworkSpecificParameter"] = list()
                out["NetworkSpecificParameter"].append(self._parse_NetworkSpecificParameter(child))
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out
    
    def _parse_CZCanceledPTTMessage(self, xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "PlannedTransportIdentifiers":
                if "PlannedTransportIdentifiers" not in out:
                    out["PlannedTransportIdentifiers"] = dict()
                key, val = self._parse_PlannedTransportIdentifiers(child)
                out["PlannedTransportIdentifiers"][key] = val
            elif child.tag == "CZPTTCancelation":
                out["CZPTTCancelation"] = self._parse_CZPTTCancelation(child)
            elif child.tag == "CZDeactivatedSection":
                out["CZDeactivatedSection"] = self._parse_CZDeactivatedSection(child)
            elif child.tag == "PlannedCalendar":
                out["PlannedCalendar"] = self._parse_PlannedCalendar(child)
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out
    
    

    @staticmethod
    def _parse_Identifiers(xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "PlannedTransportIdentifiers":
                if "PlannedTransportIdentifiers" not in out:
                    out["PlannedTransportIdentifiers"] = dict()
                key, val = Parser._parse_PlannedTransportIdentifiers(child)
                out["PlannedTransportIdentifiers"][key] = val
            elif child.tag == "RelatedPlannedTransportIdentifiers":
                assert "RelatedPlannedTransportIdentifiers" not in out
                out["RelatedPlannedTransportIdentifiers"] = Parser._parse_RelatedPlannedTransportIdentifiers(child)
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out
    
    @staticmethod
    def _parse_CZPTTCreation(xml_root: ET.Element) -> int:
        dt = datetime.strptime(xml_root.txt, "%Y-%m-%dT%H:%M:%S")
        ts = dt - datetime(1971, 1, 1)
        return int(ts.total_seconds())

    def _parse_CZPTTHeader(self, xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "CZForeignOriginLocation":
                out["CZForeignOriginLocation"] = self.pare_Location(child)
            elif child.tag == "CZForeignDestinationLocation":
                out["ForeignDestinationLocation"] = self.parse_Location(child)
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out

    def _parse_CZPTTInformation(self, xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "CZPTTLocation":
                if "CZPTTLocation" not in out:
                    out["CZPTTLocation"] = list()
                out["CZPTTLocation"].append(self._parse_CZPTTLocation(child))
            elif child.tag == "PlannedCalendar":
                out["PlannedCalendar"] = self._parse_PlannedCalendar(child)
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out
    
    @staticmethod
    def _parse_NetworkSpecificParameter(xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "Name":
                out["Name"] = child.text
            elif child.tag == "Value":
                out["Value"] = child.text
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out
    
    @staticmethod
    def _parse_PlannedTransportIdentifiers(xml_root: ET.Element) -> dict:
        out = dict()
        o_type = None
        for child in xml_root:
            if child.tag == "ObjectType":
                out["ObjectType"] = child.text
                o_type = child.text
            elif child.tag == "Company":
                out["Company"] = child.text
            elif child.tag == "Core":
                out["Core"] = child.text
            elif child.tag == "Variant":
                out["Variant"] = child.text
            elif child.tag == "TimetableYear":
                out["TimetableYear"] = child.text
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return o_type, out
