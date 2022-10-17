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
        self.database.drop_collection(collection_name)

    def api_aggregate(self, collection_name: str, *args, **kwargs):
        query = kwargs['query']
        return self.database[collection_name].aggregate(query)

    def api_update_one(self, collection_name: str, *args, **kwargs):
        if 'mongo_filter' in kwargs:
            mongo_filter = kwargs['mongo_filter']
        query = kwargs['query']

        if 'upsert' in kwargs:
            upsert = kwargs['upsert']
        else:
            upsert = False

        self.database[collection_name].update_one(mongo_filter, query, upsert)

    def api_insert_one(self, collection_name: str, **kwargs):
        data = kwargs['data']
        return self.database[collection_name].insert_one(data)

    def api_find_one(self, collection_name: str, *args, **kwargs):
        query = kwargs['query']
        return self.database[collection_name].find_one(query)

    def api_find(self, collection_name: str, **kwargs):
        if 'query' in kwargs:
            query = kwargs['query']
        return self.database[collection_name].find()

    def api_insert_many(self, collection_name: str, **kwargs):
        data = kwargs['insert_data']
        self.database[collection_name].insert_many(data)

    def print(self, collection_name: str, **kwargs):
        collection = self.database[collection_name]
        print(collection.find_one())

    def close(self):
        self.client.close()
