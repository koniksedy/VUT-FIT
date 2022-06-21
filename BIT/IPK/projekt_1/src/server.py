""" server.py
Server pro překlad doménových jmen.
Autor: Michal Šedý <xsedym02>
Spuštění: python3.6
Poslední změna: 25.2.2020
"""
import socket
import sys
import re


class RequestMsg:
    """ Třída HTTP žádosti k serveru"""

    def __init__(self, msg):
        """ Inicializace rozparsuje příchozí požadavek na důležité části.

        Params:
        -------
        msg - HTTP/1.1 příchozí požadavek k rozparsování
        """
        self.__method, self.__url, self.__body = self.parse_html_req(msg)

    def __repr__(self):
        return "RequestMsg({0}, {1}, {2})".format(self.__method, self.__url, self.__body)

    def __str__(self):
        return "({0}, {1}, {2})".format(self.__method, self.__url, self.__body)

    def parse_html_req(self, msg):
        """ Získá z HTTP požadvku jednolivé části (tělo, typ požadavku a url).

        Params:
        -------
        msg - příchozí HTTP požadavek

        Return:
        -------
        Funkce vrací rozparsovanou trojici, v případě neúspěchu trojici None.
        """
        msg_lines = msg.decode('utf-8').splitlines()
        is_body = False
        bd = list()  # Jednotlivé řádky těla (požadvky) budou uloženy v poli.
        try:
            for line, i in zip(msg_lines, range(len(msg_lines))):
                if i == 0:   # Parsování prvního řádku
                    req_word = line.split(' ')
                    met = req_word[0]    # První slovo označuje metodu GET/POST/...
                    url = req_word[1]    # Druhé slovo url
                elif not line:    # Nalezení prázdného řádku detekuje začátek těla
                    is_body = True
                elif is_body is True:
                    bd.append(line)
        except IndexError:    # Kontrola - pro jistotu
            return None, None, None
        return met, url, bd

    def method(self):
        """ Vrátí typ metady GET/POST"""
        return self.__method

    def url(self):
        """ Vrátí url požadavku."""
        return self.__url

    def body(self):
        """ Vrátí tělo požadavku."""
        return self.__body


def do_translation(req, ip_allow):
    """ Podle typu žádosti A/PTR provede získání odpovědi (jméno/ip addresa).

    Params:
    -------
    req - data požadavku na převod ip->host a naopak
    ip_allow - volba pořadavku A/PTR (A-přebod na ip, PTR - převod na jméno)

    Return:
    -------
    Funkce vrací převedenou adresu, nebo None pokud se převod nepovedl.
    """
    typ = 'A' if ip_allow else 'PTR'
    try:
        if ip_allow is True and re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$", req) is None:
            to_ret = socket.gethostbyname(req)
        elif ip_allow is False and re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$", req) is not None:
            to_ret = socket.gethostbyaddr(req)[0]
        else:
            return None
    except Exception as err:
        print(err, file=sys.stderr)
        return None
    return '{0}:{1}={2}\r\n'.format(req, typ, to_ret)


def do_post(body):
    """ Funkce provádí HTTP metodu POST.
    Pokud se převod ip/jména nepodaří, nebude ve výsledku adresa zahrnuta.

    Params:
    -------
    body - tělo HTTP obsahující jednotlivé požadavky na převod.

    Return:
    -------
    Funkce vrací získaný výstup i s hlavičkou.
    V případě chybného druhu požadavku A/PTR, přeskočí dotaz
    """
    out_line = None
    out_all = ''
    for line in body:
        if line.endswith(':A'):
            out_line = do_translation(line[:-2], True)
        elif line.endswith(':PTR'):
            out_line = do_translation(line[:-4], False)
        else:
            continue

        if out_line is not None:
            out_all += out_line

    if not out_all:
        return 'HTTP/1.1 404 Not Found\r\n\r\n'

    return 'HTTP/1.1 200 OK\r\n\r\n' + out_all


def do_get(req, ip_allow):
    """ Funkce provádí HTTP metodu GET převádějící ip na jméno a neopak.

    Params:
    -------
    req - jméno pro převod
    ip_alow - požadavek na převod (True - na ip, False - na jméno)

    Return:
    -------
    Funkce vrací získanou odpověď z převodu. Pokud nebyla odpověď nalezana,
    je výsledek 404 Not Found.
    """
    if((ip_allow is True and re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$", req) is not None) or
       (ip_allow is False and re.match(r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$", req) is None)):
        return 'HTTP/1.1 400 Bad Request\r\n\r\n'
    ans = do_translation(req, ip_allow)
    if ans is None:
        return 'HTTP/1.1 404 Not Found\r\n\r\n'
    else:
        return 'HTTP/1.1 200 OK\r\n\r\n{0}'.format(ans)


def main():
    HOST = '127.0.0.1'
    PORT = int(sys.argv[1])

    # Vytvoření socketu
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print("Socket vtvořen.", file=sys.stderr)

    # Připojení socketu na port
    try:
        s.bind((HOST, PORT))
    except Exception as err:
        print("Nepodařilo se nabindovat socket: {0}".format(err), file=sys.stderr)
        sys.exit(1)
    print("Socket se nabindovat HOST={0} PORT={1}.".format(HOST, PORT), file=sys.stderr)

    # Socket čeká na příjem
    s.listen()
    print("Socket poslouchá.", file=sys.stderr)

    # Zpracovávání požadavků klienta, detekce ctrl+c --> ukončení serveru (odpojení)
    try:
        while True:
            # Navázání komunikace s klientem
            conn, addr = s.accept()
            print("Navázáno spojení s {0}.".format(addr), file=sys.stderr)

            data = conn.recv(1024)
            # Parsování dat na jednotlivé části Metoda Uri Body
            data_http = RequestMsg(data)
            if data_http.method() == 'GET':
                # Metoda GET musí obsahovat resolve
                if data_http.url().startswith('/resolve?name='):
                    # Vykonání jednotlivých typů GET (A - získání ip, PTR - získání jména)
                    if data_http.url().endswith('type=A'):
                        msg_out = do_get(data_http.url()[14:-7], True)
                    elif data_http.url().endswith('type=PTR'):
                        msg_out = do_get(data_http.url()[14:-9], False)
                    else:  # Ostatní je chyba
                        print(data_http.url())
                        msg_out = 'HTTP/1.1 400 Bad Request\r\n\r\n'
                else:
                    msg_out = 'HTTP/1.1 400 Bad Request\r\n\r\n'
            elif data_http.method() == 'POST':
                if data_http.url() == '/dns-query':
                    msg_out = do_post(data_http.body())
                else:
                    msg_out = 'HTTP/1.1 400 Bad Request\r\n\r\n'
            else:    # Jimá metoda než GET a POST není dovolena
                msg_out = 'HTTP/1.1 405 Method Not Allowed\r\n\r\n'

            # Odeslání získané odpovědi klientovi a odpojení
            conn.sendall(msg_out.encode(encoding='UTF-8', errors='strict'))
            conn.close()

    except KeyboardInterrupt:
        print("Ukončení serveru na základě ctrl+C.", file=sys.stderr)
    finally:
        # odpojení socketu
        s.close()


if __name__ == '__main__':
    main()
