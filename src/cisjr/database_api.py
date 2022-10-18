"""
database_api.py
Script provides a MongoDB API for database operations.
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 12.11.2022
"""

import pymongo


class Database:
    def __init__(self, database_name: str, host: str):
        self.client = pymongo.MongoClient(host)
        self.database = self.client[database_name]

    def api_drop_collection(self, collection_name: str):
        return self.database.drop_collection(collection_name)

    def api_aggregate(self, collection_name: str, *args, **kwargs):
        ##query = kwargs['query']
        return self.database[collection_name].aggregate(*args, **kwargs)

    def api_update_one(self, collection_name: str, *args, **kwargs):
        return self.database[collection_name].update_one(*args, **kwargs)

    def api_insert_one(self, collection_name: str, *args, **kwargs):
        return self.database[collection_name].insert_one(*args, **kwargs)

    def api_find_one(self, collection_name: str, *args, **kwargs):
        return self.database[collection_name].find_one(*args, **kwargs)

    def api_find(self, collection_name: str, *args, **kwargs):
        return self.database[collection_name].find(*args, **kwargs)

    def api_insert_many(self, collection_name: str, *args, **kwargs):
        return self.database[collection_name].insert_many(*args, **kwargs)

    def close(self):
        self.client.close()
