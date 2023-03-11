# scraper.sh
# UPA (Project 3)
# The module with a class for web scrapping from https://playtech.co.nz
# Authors: Bc. Martina Chripková <xchrip01@stud.fit.vutbr.cz>
#          Bc. Martin Novotný Mlinárcsik <xnovot1r@stud.fit.vutbr.cz>
#          Bc. Michal Šedý <xsedym02@stud.fit.vutbr.cz>
# Last change: 05.10.222


import bs4
import requests


class Scraper:
    def __init__(self, url: str) -> None:
        self._server = url
        self._urls = set()

    def fetch_all(self) -> None:
        """Fetch all product URLs from a self._server
        """
        for page in self._get_catalog_pages():
            self._load_items(page)

    def _get_catalog_pages(self):
        """Yields all catalog pages of self._server

        Yields:
            generator: Links to catalog pages
        """
        rqst = requests.get(self._server).content
        bs = bs4.BeautifulSoup(rqst, "html.parser")
        links = bs.find_all("a", {"class": "site-nav__dropdown-link site-nav__dropdown-link--top-level"}, href=True)
        for link in links:
            yield link["href"]

    def _get_next_page_link(self, bs: bs4.BeautifulSoup) -> str:
        """Returns next page is the button next page exists.

        Args:
            bs (bs4.BeautifulSoup): Actual page.

        Returns:
            str: Ling of the next page.
        """
        next_span = bs.find("span", {"class": "next"})
        if next_span:
            next_link = next_span.findChild("a", recursive=False)
            return next_link["href"]
        else:
            return None

    def _load_items(self, page: str) -> None:
        """Loads all items/products from actual and next pages (if exists).

        Args:
            page (str): Link of the actual page.
        """
        rqst = requests.get(self._server + page).content
        bs = bs4.BeautifulSoup(rqst, "html.parser")
        links = bs.find_all("a", {"class": "grid-product__link"}, href=True)

        for link in links:
            self._urls.add(link["href"])

        # Test if the next page exists.
        next_page_link = self._get_next_page_link(bs)
        if next_page_link:
            self._load_items(next_page_link)

    def save(self, file_name: str) -> None:
        """Saves scraped data to the file.

        Args:
            file_name (str): Output file name.
        """
        with open(file_name, "w") as fd:
            for url in self._urls:
                print(self._server + url, file=fd)

    def print(self) -> None:
        """Prints scraped data.
        """
        for url in self._urls:
            print(self._server + url)

    @staticmethod
    def get_product_name_price(url: str) -> tuple:
        """Finds name and price of the product specified by a URL.

        Args:
            url (str): Product URL.

        Returns:
            tuple: Product name and price
        """
        rqst = requests.get(url).content
        bs = bs4.BeautifulSoup(rqst, "html.parser")

        name = bs.find("h1", {"class": "product-single__title"}).text.strip()
        price_sale = bs.find("span", {"class": "product__price on-sale"})
        if price_sale is None:
            price = bs.find("span", {"class": "product__price"}).text.strip()
        else:
            price = price_sale.text.strip()

        return name, price
