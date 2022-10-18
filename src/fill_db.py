"""
fill_db.py
Program that downloads, parses, and uploads data from
https://portal.cisjr.cz/pub/draha/celostatni/szdc/2022/ to mongodb.
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 18.11.2022
"""

from argparse import ArgumentParser
from cisjr import downloader, uploader
from cisjr import database_api


def main():
    # Parse args
    opt_parser = ArgumentParser()
    opt_parser.add_argument("--parallel-download", type=int, metavar="N_THR", default=1,
                            help=("Download data in 0 < N_THREADS parallel threads." +
                                  "WARNING high number can lead to the lag in QUEUEING TASKS"))
    opt_parser.add_argument("--parallel-parse", type=int, metavar="N_THR", default=1,
                            help=("Parse data in 0 < N_THREADS parallel threads." +
                                  "WARNING high number can lead to the lag in QUEUEING TASKS"))
    opt_parser.add_argument("-f", "--force", action="store_true",
                            help="Downloads and updates already processed files.")
    opt_parser.add_argument("workdir", nargs="?",
                            help="Name of a working directory. (default /tmp).")
    args = opt_parser.parse_args()


    # Connect to Database
    connection_string = f"mongodb://localhost:27017"
    db = database_api.Database("cisjr", connection_string)

    # Get existing links
    # Uses database_api
    except_links = set()
    if not args.force:
        for link in db.api_find("Downloaded"):
            except_links.add(link["Link"])
    else:
        db.api_drop_collection("Downloaded")

    # Download (can be done in parallel)
    donw = downloader.Downloader("https://portal.cisjr.cz/pub/draha/celostatni/szdc/2022/", workdir=args.workdir)
    downloaded_links = donw.download(except_links=except_links, n_threads=args.parallel_download)

    # Save downloaded links to DB
    if downloaded_links:
        db.api_insert_many("Downloaded", [{"Link": link} for link in downloaded_links])

    # Parse + Upload
    xml_paths = donw.get()
    up = uploader.Uploader()
    up.upload(xml_paths, force=args.force, n_threads=args.parallel_parse)

    # Close DB client
    db.close()

if __name__ == "__main__":
    main()
