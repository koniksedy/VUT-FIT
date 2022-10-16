"""
downloader.py
Executable module for data downloading from cisjr.
Data can be downloaded in parallel.
UPA project
Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
         Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
         Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
Last change: 12.11.2022
"""

import os
import sys
import bs4
import shutil
import tempfile
import pathlib
import tqdm
import gzip
import zipfile
import requests
import urllib.request


class Downloader:
    def __init__(self, url: str, workdir: str = None) -> None:
        self.url = url
        self.portal = url.split("://")[0] + "://" + url.split("://")[1].split("/")[0]

        if workdir is None:
            self.workdir = tempfile.mkdtemp(prefix="cisjr")
            # If set to true, the data folder will be deleted.
            self._clear_at_exit = True
        else:
            pathlib.Path(workdir).mkdir(parents=True, exist_ok=True)
            self.workdir = workdir
            # If set to true, the data folder will be deleted.
            self._clear_at_exit = False

        self.folder_zip = f"{self.workdir}/data"
        if not os.path.exists(self.folder_zip):
            os.mkdir(self.folder_zip)
        self.folder_xml = f"{self.workdir}/zips"
        if not os.path.exists(self.folder_xml):
            os.mkdir(self.folder_xml)

        self.process_bar = None

    def __del__(self) -> None:
        if self._clear_at_exit:
            shutil.rmtree(self.workdir)

    @staticmethod
    def _get_files(dir: str) -> list:
        """Iterates recursively over directory and returns all founded file paths.

        Args:
            dir (str): Directory.

        Returns:
            list: Paths of the files in the dir.
        """
        files = list()
        for name in os.listdir(dir):
            full_name = f"{dir}/{name}"
            if os.path.isfile(full_name):
                files.append(full_name)
            else:
                files.extend(Downloader._get_files(full_name))
        return files

    def _get_links(self, url: str) -> set:
        """Finds all links on a page given by url.

        Args:
            url (str): Url of the page.
        Returns:
            set: A set of links.
        """
        links = set()
        rqst = requests.get(url).content
        bs = bs4.BeautifulSoup(rqst, "html.parser")
        content = bs.find_all("a", href=True)
        for link in content:
            if link["href"] in url:
                continue
            if link["href"].endswith("/"):
                links.update(self._get_links(self.portal + link["href"]))
            else:
                links.add(link["href"])

        return links

    def _unzip(self, files: set) -> None:
        """Unzips all given files from folder self.folder_zip
        and saves them into the folder self.folder_xml with
        the same directory hierarchy.

        Args:
            files (set): Name of the files to be unzipped.
        """
        for file_name in files:
            out_folder = f"{self.folder_xml}/{'/'.join(file_name.split('/')[:-1])}"
            pathlib.Path(out_folder).mkdir(parents=True, exist_ok=True)
            if file_name.endswith(".xml.zip"):
                # Zips can be of two different types, gzip or zip.
                # It is impossible to determine the exact type, therefore you need to try it.
                try:
                    out_name = f"{file_name.split('/')[-1][:-8]}.xml"
                    with gzip.open(f"{self.folder_zip}/{file_name}", "rb") as fd_in:
                        with open(f"{out_folder}/{out_name}", 'wb') as fd_out:
                            shutil.copyfileobj(fd_in, fd_out)
                except gzip.BadGzipFile:
                    with zipfile.ZipFile(f"{self.folder_zip}/{file_name}", "r") as zip_d:
                        zip_d.extractall(out_folder)
            else:
                with zipfile.ZipFile(f"{self.folder_zip}/{file_name}", "r") as zip_d:
                        zip_d.extractall(out_folder)

    def _parallel_download(self, link: list) -> None:
        file_name = link.split("/")[-1]
        folder_name = "/".join(link[32:].split("/")[:-1])
        if not os.path.exists(folder_name):
            pathlib.Path(f"{self.folder_zip}/{folder_name}").mkdir(parents=True, exist_ok=True)
            urllib.request.urlretrieve(self.portal + link, f"{self.folder_zip}/{folder_name}/{file_name}")

    def download(self, except_links: set = set(), n_threads: int = 1) -> set:
        """The method downloads and extracts *.xml.zip data from self.url.
        Raw data are in the folder specified by self.folder_zip.
        Extracted files are in the folder specified by self.folder_xml.

        Args:
            except_links (set, optional): A set of links that are already downloaded.
                                          Defaults to the empty set.
            n_threads (int, optional): If greater than 1, the parallel downloading with
                                       n_threads is performed. Defaults 1.
        Returns:
            set: A of set of links of downloaded files.
        """
        # Get remote file links
        print("Initializing...          ", file=sys.stderr, end="")
        sys.stderr.flush()
        downloaded_links = self._get_links(self.url).difference(except_links)
        print("Done", file=sys.stderr)

        # Downloading (can be done in parallel)
        if n_threads > 1:
            from pqdm.processes import pqdm
            print("Parallel downloading...", file=sys.stderr)
            pqdm(downloaded_links, self._parallel_download, n_jobs=n_threads)
            print("Downloading done.", file=sys.stderr)
        else:
            for link in tqdm.tqdm(downloaded_links,
                                desc="Downloading...",
                                ascii=False,
                                file=sys.stderr):
                file_name = link.split("/")[-1]
                folder_name = "/".join(link[32:].split("/")[:-1])
                if not os.path.exists(folder_name):
                    pathlib.Path(f"{self.folder_zip}/{folder_name}").mkdir(parents=True, exist_ok=True)
                    urllib.request.urlretrieve(self.portal + link, f"{self.folder_zip}/{folder_name}/{file_name}")

        # Unzip
        zip_paths = set(map(lambda x: x[32:], downloaded_links))
        originals = set(filter(lambda x: "oprava" not in x, zip_paths))
        updates = zip_paths.difference(originals)

        print("Unzipping originals...   ", end="", file=sys.stderr)
        sys.stderr.flush()
        self._unzip(originals)
        print("Done", file=sys.stderr)

        print("Unzipping updates...     ", end="", file=sys.stderr)
        sys.stderr.flush()
        self._unzip(updates)
        print("Done", file=sys.stderr)

        return downloaded_links

    def get(self) -> list:
        """Returns sorted list of paths to xml files from self.folder_xml.

        Returns:
            list: Sorted list of paths to xml files.
        """
        return list(sorted(filter(lambda x: x.endswith(".xml"), Downloader._get_files(self.folder_xml))))

    def set_clear_at_exit(self):
        # If set to true, the data folder will be deleted.
        self._clear_at_exit = True


def main():
    import pymongo
    from argparse import ArgumentParser

    # Parse args
    opt_parser = ArgumentParser()
    opt_parser.add_argument("-p", "--parallel", type=int, metavar="N_THR", default=1,
                            help=("Download data in 0 < N_THREADS parallel threads.\n" +
                                  "WARNING high number can lead to the lag in QUEUEING TASKS"))
    opt_parser.add_argument("-f", "--force", action="store_true",
                            help="Downloads already downloaded files.")
    opt_parser.add_argument("workdir", nargs="?",
                            help="Name of a working directory. (default /tmp).")
    args = opt_parser.parse_args()

    # Connect to DB
    connection_string = f"mongodb://localhost:27017"
    client = pymongo.MongoClient(connection_string)
    db = client["cisjr"]

    # Get existing links
    except_links = set()
    if not args.force:
        for link in db["Downloaded"].find():
            except_links.add(link["Link"])
    else:
        db.drop_collection("Downloaded")

    # Downloading
    down = Downloader("https://portal.cisjr.cz/pub/draha/celostatni/szdc/2022/", workdir=args.workdir)
    downloaded_links = down.download(except_links=except_links, n_threads=args.parallel)

    # Save downloaded links to DB
    if downloaded_links:
        db["Downloaded"].insert_many([{"Link": link} for link in downloaded_links])

    client.close()


if __name__ == "__main__":
    main()
