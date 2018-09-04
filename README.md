# ===Simulator - ENG===

Each `requestor`/`provider` (`req`/`prov`) works in a separate thread.
Entering the network means that a new req/prov is created and launched in a separate thread.
Leaving the network means that the thread is stopped and the corresponding 'unemployed' req/prov is moved to the `escaped` list where they are stored in order to be displayed in the statistics at the end of simulation.

Requestors can choose providers through the `NetworkGateway`, which have a list of all 'work awaiting' providers.
Requestor who wishes to announce a task consisting of `m` subtasks receives pointers to `m` best (ex. cheapest) providers.
Next, Requestor assigns a task to each provider.
Having the task solved, the provider returns to the list of 'work awaiting' providers.
If there are no free providers, the requestor has to wait.

Compilation:\
x86, C++14, without any additional libraries.\
IDE - MSVS 2017 RC

## Task

Write a program (in a chosen by you language), which will launch the simulation. 
It should be possible to change the simulation parameters on the fly.

### Input(should have reasonable default values)

* n - Network size
* p - Percent of requestors
* m - Number providers needed to solve the next task (task consists of m subtasks, assign 1 provider/subtask)
* t - Probability Distribution Function of time required to calculate the task.
* z - Probability Distribution Function of time between tasks announcements.
* T - Simulation time
* provider_selection_function used by requestors to choose best providers

### Output

List of providers with their parameters and number of accomplished tasks.

We would like to simulate a simplified network with a given size 'n'.
A percentage 'p' of network nodes shall be requestors.

Providers have 3 attributes with random values:price [0.001, 5], efficiency [0.001, 10] and reputation [0, 1]. 
Providers wait until requestors announce a task, then they apply to solve it.
Being chosen to work on a given task, according to prescribed distribution function, they draw time 't' required to solve the it. During this time they do not apply for any other tasks. Each provider counts number of subtasks he solved.

Requestors, according to prescribed distribution function, draw time 'z' to announce next task.
They choose 'm' best providers and assign them a work.
Next, they draw another 'z' and the scheme is repeated.
If at particular time there is less then 'm' free providers the requestor has to wait.
Simulation ends after T seconds, then a list of providers' parameters and amount of solved tasks per provider is displayed.


### Example

n = 10\
p = 20\
m = 5\
t is from uniform distribution within [1, 3]\
z is from uniform distribution within [3, 5]\
T = 30\
Provider selection function = cheapest_first

We simulate a network of size 10, 20% of it are requestors, thus we have 8 providers and 2 requestors.
Requestor_A drawn z = 3, Requestor_B z = 4. After 3 seconds Requestor_A announces a task and 8 providers apply for it. 
Requestor_A chooses five cheapest providers. Each of them drawn a time t (ex. 1.2, 1.5, 1.7, 2, 2.8). 
At 4 seconds Requestor_B announces his task. Only 3 providers apply for it.
Requestor_B waits for 0.5 seconds until 2 providers finish working for Requestor_A. Then a task is assigned to the rest of 5 providers. 
Requestor_A drawn next value 'z' equal to 5, so he will announce next task at 8th second.
After 30 seconds simulation stop and the statistics are displayed:\
Provider1 (3, 2, 0.2) 4\
Provider2 (0.4, 5, 0.1) 6\
…\
Provider8 (4.2, 10, 0.9) 3\

ProviderName (price, efficiency, reputation), number of accomplished tasks.

### Remarks

Supported Distribution Functions: at least uniform distribution, there shall be 2-3 more to choose from.\
Provider selection function: at least 3 functions which assess single parameters, however there might be weighted average or some other original idea.


# ===Symulator - PL===

Każdy requestor/provider (req/prov) działa w osobnym wątku.
Wejście do sieci oznacza utworzenie nowego req/prov i uruchomienie go w nowym wątku.
Wyjście z sieci realizowane jest poprzez zatrzymanie wątku i przełożenie bezrobotnego req/prov do listy 'escaped', gdzie są przechowywani celem wyświetlenia w statystykach na koniec symulacji.

Requestorzy wybierają providerów poprzez NetworkGateway, który przechowuje listę wszyskich oczekujących na pracę providerów.
Requestor chcący ogłosić 'm' zadań otrzymuje z NetworkGateway wskazniki do m najlepszych (np najtańszych) providerów.
Następnie, Requestor przydziela każdemu providerowi zadanie.
Po rozwiązaniu zadania provider samodzielnie wraca do listy oczekujących na prace.
Jeżeli nie ma wolnych providerów, to requestor czeka.

Kompilacja:\
x86, C++14, bez dodatkowych bibliotek\
IDE - MSVS 2017 RC

## Zadanie

Napisz program (w wybranym przez siebie języku), który uruchamia przydział zadań z możliwością zmiany parametrów.

### Wejście (parametry powinny mieć sensowne wartości domyślne)

* n - Rozmiar sieci
* p - Procent requestorow
* m - Liczba liczących podzadanie
* t - Rozkład / parametry rozkładu czasu liczenia zadania
* z - Rozkład / parametry rozkładu czasu pomiędzy zlecaniem zadań
* T - Czas trwania symulacji
* Funkcja oceny stosowana przez requestorów do wyboru najlepszych providerów.

### Wyjście

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

### Przykład

n = 10\
p = 20\
m = 5\
t ma być wyznaczone z rozkładu jednostajnego z przedziału [1, 3]\
z ma być wyznaczone z rozkładu jednostajnego z przedziału [3, 5]\
T = 30\
Funkcja oceny providera = najlepsza cena

Symulujemy sieć wielkości 10, z której 20% stanowią requestorzy, zatem mamy 8 providerów i 2 requestorów.
Pierwszy requestor losuje z = 3, drugi z = 4. Po 3 sekundach pierwszy requestor ogłasza zadanie wszystkich 8 providerów zgłasza się do zadania.
Requestor 1 wybiera pięciu providerów z najlepszą ceną. Wszyscy losują sobie czas t (np. 1.2, 1.5, 1.7, 2, 2.8).
W 4 sekundzie 2 requestor ogłasza zadanie. Tylko 3 providerów zgłasza się w tym momencie do zadania. 
Requestor musi poczekać jeszcze 0.5 sekundy aż 2 providerzy skończą liczyć zadanie dla requestora 1. 
Wtedy przyznaje zadanie wszystkim 5 pozostałym providerom.
Requestor 1 wylosował kolejną wartość z po przydzieleniu pierwszego zadania równą 5, więc kolejne zadanie ogłosi w 8 sekundzie.
Po 30 sekundach symulacja zatrzymuje się a na wyjściu wypisywane jest:\
Provider1 (3, 2, 0.2) 4\
Provider2 (0.4, 5, 0.1) 6\
…\
Provider8 (4.2, 10, 0.9) 3

Czyli NazwaProvidera (cena, wydajność, reputacja) liczba policzonych zadań

### Uwagi

Wspierane rozkłady - wersja minimum to rozkład jednostajny, ale dobrze by było, gdyby były też 2-3 inne sensowne do wyboru.\
Funkcja oceny providera - wersja minimum to 3 funkcje, które oceniają pojedyncze parametry, 
ale równie dobrze można to zastąpić odpowiednio skonstruowaną funkcją z wagami lub jakimś innym autorskim pomysłem.
