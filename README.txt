
Symulator - info:

Każdy requestor/provider (req/prov) działa w osobnym wątku.
Wejście do sieci oznacza utworzenie nowego req/prov i uruchomienie go w nowym wątku.
Wyjście z sieci realizowane jest poprzez zatrzymanie wątku i przełożenie bezrobotnego req/prov do listy 'escaped', 
gdzie są przechowywani celem wyświetlenia w statystykach na koniec symulacji.

Requestorzy wybierają providerow poprzez NetworkGateway, ktory przechowuje listę wszyskich oczekujących na pracę providerów.
Requestor chcący ogłosić 'm' zadań otrzymuje z NetworkGateway wskazniki do m najlepszych (np najtańszych) providerów.
Następnie, Requestor przydziela każdemu providerowi zadanie. Po rozwiązaniu zadania provider samodzielnie wraca do listy oczekujących na prace.
Jeżeli nie ma wolnych providerów, to requestor czeka.


Kompilacja:
C++14, bez dodatkowych bibliotek
IDE - MSVS 2017 RC

