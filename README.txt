
Symulator - info:

Kazdy requestor/provider (req/prov) dziala w osobnym watku.
Wejscie do sieci oznacza utworzenie nowego req/prov i uruchomienie go w nowym watku.
Wyjscie z sieci realizowane jest poprzez zatrzymanie watku i przelozenie bezrobotnego req/prov do listy 'escaped', 
gdzie sa przechowywani celem wyswietlenia w statystykach na koniec symulacji.

Requestorzy wybieraja providerow poprzez NetworkGateway, ktory przechowuje liste wszyskich oczekujacych na prace providerow.
Requestor chcacy oglosic 'm' zadan otrzymuje z NetworkGateway wskazniki do m najlepszych (np najtanszych) providerow.
Nastepnie, Requestor przydziela kazdemu providerowi zadanie. Po rozwiazaniu zadania provider samodzielnie wraca do listy oczekujacych na prace.
Jezeli nie ma wolnych providerow, to requestor czeka.


Kompilacja:
C++14, bez dodatkowych bibliotek
Srodowisko - uzywalem MSVS 2017 RC

