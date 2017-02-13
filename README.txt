
---Symulator---

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




---Zadanie---

Napisz program (w wybranym przez siebie języku), który uruchamia przydział zadań z możliwością zmiany parametrów.

Wejście (parametry powinny mieć sensowne wartości domyślne):
Rozmiar sieci (n)
Procent requestorow (p)
Liczba liczących podzadanie (m)
Rozkład / parametry rozkładu czasu liczenia zadania (t)
Rozkład / parametry rozkładu czasu pomiędzy zlecaniem zadań (z)
Czas trwania symulacji (T)
Funkcja oceny stosowana przez requestorów do wyboru najlepszych providerów.

Wyjście:
Lista providerów z parametrami i liczbą policzonych przez nich zadań

Chcemy symulować uproszczone działanie sieci o zadanym rozmiarze n. Ustalony procent sieci p ma “pełnić funkcję” requestorów. 
Providerzy mają przypisane losowe wartości 3 atrybutów: cena [0.001, 5], wydajność [0.001, 10] i reputacja [0, 1].
Providerzy czekają, aż requestorzy ogłoszą jakieś zadanie i zgłaszają się do policzenia. 
Jeśli zostaną wybrani to losują swój czas liczenia zadania (t) zgodnie z podanym rozkładem. 
Przez ustalony czas t nie zgłaszają się do innych zleceń. Provider odnotowuje ile zadań policzył.
Requestorzy losują czas ogłoszenia zadania (z) zgodnie z podanym rozkładem i po upłynięciu wylosowanego czasu z ogłaszają zadanie. 
Z ofert otrzymanych przez wolnych providerów wybierają m najlepszych i informują ich o przydzieleniu zadania.
Następnie losują kolejną wartość z i powtarzają operację. 
Jeśli w danym momencie zgłasza się mniej niż m providerów trzeba poczekać aż pojawią się kolejne oferty.
Symulacja kończy się po T sekundach, wtedy powinna zostać wypisana lista providerów z ich parametrami i liczbą policzonych zadań.


Przykład:
n = 10
p = 20
m = 5
t ma być wyznaczone z rozkładu jednostajnego z przedziału [1, 3]
z ma być wyznaczone z rozkładu jednostajnego z przedziału [3, 5]
T = 30
Funkcja oceny providera = najlepsza cena

Symulujemy sieć wielkości 10, z której 20% stanowią requestorzy, zatem mamy 8 providerów i 2 requestorów. 
Pierwszy requestor losuje z = 3, drugi z = 4. Po 3 sekundach pierwszy requestor ogłasza zadanie, 
wszystkich 8 providerów zgłasza się do zadania. 
Requestor 1 wybiera pięciu providerów z najlepszą ceną. Wszyscy losują sobie czas t (np. 1.2, 1.5, 1.7, 2, 2.8).
W 4 sekundzie 2 requestor ogłasza zadanie. Tylko 3 providerów zgłasza się w tym momencie do zadania. 
Requestor musi poczekać jeszcze 0.5 sekundy aż 2 providerzy skończą liczyć zadanie dla requestora 1. 
Wtedy przyznaje zadanie wszystkim 5 pozostałym providerom.
Requestor 1 wylosował kolejną wartość z po przydzieleniu pierwszego zadania równą 5, więc kolejne zadanie ogłosi w 8 sekundzie.
Po 30 sekundach symulacja zatrzymuje się a na wyjściu wypisywane jest:
Provider1 (3, 2, 0.2) 4
Provider2 (0.4, 5, 0.1) 6
…
Provider8 (4.2, 10, 0.9) 3

Czyli NazwaProvidera (cena, wydajność, reputacja) liczba policzonych zadań

Uwagi:
Wspierane rozkłady - wersja minimum to rozkład jednostajny, ale dobrze by było, gdyby były też 2-3 inne sensowne do wyboru.
Funkcja oceny providera - wersja minimum to 3 funkcje, które oceniają pojedyncze parametry, 
ale równie dobrze można to zastąpić odpowiednio skonstruowaną funkcją z wagami lub jakimś innym autorskim pomysłem.
