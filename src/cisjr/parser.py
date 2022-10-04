"""
parser.py
Module parses xml data.
UPA project
Authors: Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 04.10.2022
"""

# TODO data types

import sys
import tqdm
import xml.etree.ElementTree as ET
# from datetime import datetime


class Parser:
    def __init__(self) -> None:
        self.Locations_dict = dict()

    
    def parse(self, xml_files: str) -> dict:
        CZPTTCISMessage_list = list()
        CZCanceledPTTMessage_list = list()

        for xml_f in tqdm.tqdm(xml_files, desc="Parsing...", ascii=False, ncols=75, file=sys.stderr):
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
                out["CZPTTCreation"] = self._parse_date_time(child.text)
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
                key, val = self._parse_TransportIdentifiers(child)
                out["PlannedTransportIdentifiers"][key] = val
            elif child.tag == "CZPTTCancelation":
                out["CZPTTCancelation"] = self._parse_date_time(child.text)
            elif child.tag == "CZDeactivatedSection":
                out["CZDeactivatedSection"] = self._parse_CZDeactivatedSection(child)
            elif child.tag == "PlannedCalendar":
                out["PlannedCalendar"] = self._parse_PlannedCalendar(child)
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out
    
    def _parse_CZPTTHeader(self, xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "CZForeignOriginLocation":
                out["CZForeignOriginLocation"] = self._parse_Location(child)
            elif child.tag == "CZForeignDestinationLocation":
                out["ForeignDestinationLocation"] = self._parse_Location(child)
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        if not out:
            return None
        return out

    @staticmethod
    def _parse_Identifiers(xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "PlannedTransportIdentifiers":
                if "PlannedTransportIdentifiers" not in out:
                    out["PlannedTransportIdentifiers"] = dict()
                key, val = Parser._parse_TransportIdentifiers(child)
                out["PlannedTransportIdentifiers"][key] = val
            elif child.tag == "RelatedPlannedTransportIdentifiers":
                assert "RelatedPlannedTransportIdentifiers" not in out
                out["RelatedPlannedTransportIdentifiers"] = dict()
                key, val = Parser._parse_TransportIdentifiers(child)
                out["RelatedPlannedTransportIdentifiers"][key] = val
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out

    @staticmethod
    def _parse_TransportIdentifiers(xml_root: ET.Element) -> dict:
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

    def _parse_CZPTTLocation(self, xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "Location":
                out["Location"] = self._parse_Location(child)
            elif child.tag == "TimingAtLocation":
                out["TimingAtLocation"] = self._parse_TimingAtLocation(child)
            elif child.tag == "ResponsibleRU":
                out["ResponsibleRU"] = child.text
            elif child.tag == "ResponsibleIM":
                out["ResponsibleIM"] = child.text
            elif child.tag == "TrainType":
                out["TrainType"] = child.text
            elif child.tag == "TrafficType":
                out["TrafficType"] = child.text
            elif child.tag == "CommercialTrafficType":
                out["CommercialTrafficType"] = child.text
            elif child.tag == "TrainActivity":
                out["TrainActivity"] = child.find("TrainActivityType").text
            elif child.tag == "OperationalTrainNumber":
                out["OperationalTrainNumber"] = child.text
            elif child.tag == "NetworkSpecificParameter":
                if "NetworkSpecificParameter" not in out:
                    out["NetworkSpecificParameter"] = list()
                out["NetworkSpecificParameter"].append(self._parse_NetworkSpecificParameter(child))
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out

    def _parse_Location(self, xml_root: ET.Element) -> int:
        data = dict()
        for child in xml_root:
            if child.tag == "CountryCodeISO":
                data["CountryCodeISO"] = child.text
            elif child.tag == "LocationPrimaryCode":
                data["LocationPrimaryCode"] = child.text
            elif child.tag == "PrimaryLocationName":
                data["PrimaryLocationName"] = child.text
            elif child.tag == "LocationSubsidiaryIdentification":
                data["LocationSubsidiaryIdentification"] = self._parse_LocationSubsidiaryIdentification(child)
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        
        name = data["PrimaryLocationName"]
        code = data["LocationPrimaryCode"]
        track = data["LocationSubsidiaryIdentification"]["LocationSubsidiaryCode"] if "LocationSubsidiaryIdentification" in data else None
        if (name, code, track) not in self.Locations_dict:
            data["_id"] = len(self.Locations_dict)
            self.Locations_dict[(name, code, track)] = data

        return self.Locations_dict[(name, code, track)]["_id"]

    @staticmethod
    def _parse_PlannedCalendar(xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "BitmapDays":
                out["BitmapDays"] = child.text
            elif child.tag == "ValidityPeriod":
                out["ValidityPeriod"] = Parser._parse_ValidityPeriod(child)
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

    def _parse_CZDeactivatedSection(self, xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "StartLocation":
                out["StartLocation"] = self._parse_Location(child)
            elif child.tag == "EndLocation":
                out["EndLocation"] = self._parse_Location()
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out
    
    @staticmethod
    def _parse_ValidityPeriod(xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "StartDateTime":
                out["StartDateTime"] = Parser._parse_date_time(child.text)
            elif child.tag == "EndDateTime":
                out["EndDateTime"] = Parser._parse_date_time(child.text)
  
    @staticmethod
    def _parse_LocationSubsidiaryIdentification(xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "LocationSubsidiaryCode":
                out["LocationSubsidiaryCode"] = child.text
                out["LocationSubsidiaryTypeCode"] = child.attrib["LocationSubsidiaryTypeCode"]
            elif child.tag == "AllocationCompany":
                out["AllocationCompany"] = child.text
            elif child.tag == "LocationSubsidiaryName":
                out["LocationSubsidiaryName"] = child.text
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out

    @staticmethod
    def _parse_TimingAtLocation(xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "Timing" and child.attrib["TimingQualifierCode"] == "ALA":
                out["ALA"] = Parser._parse_Timing(child)
            elif child.tag == "Timing" and child.attrib["TimingQualifierCode"] == "ALD":
                out["ALD"] = Parser._parse_Timing(child)
            elif child.tag == "DwellTime":
                out["DwellTime"] = child.text
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out

    @staticmethod
    def _parse_Timing(xml_root: ET.Element) -> dict:
        out = dict()
        for child in xml_root:
            if child.tag == "Time":
                # dt = datetime.strptime(child[:8], "%H:%M:%S")
                # ts = dt - datetime(1971, 1, 1)
                # out["Time"] = int(ts.total_seconds())
                out["Time"] = child.text
            elif child.tag == "Offset":
                out["Offset"] = child.text
            else:
                raise ValueError(f"Unsupported xml tag {child.tag}")
        return out

    @staticmethod
    def _parse_date_time(string: str) -> str:
        # dt = datetime.strptime(string, "%Y-%m-%dT%H:%M:%S")
        # ts = dt - datetime(1971, 1, 1)
        # return int(ts.total_seconds())
        return string