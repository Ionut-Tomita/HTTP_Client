325CD - Tomita Ionut


# Client HTTP in C pentru interactiunea cu un REST API

    Clientul este un program scris în C care accepta comenzi de la tastatura (stdin)
    şi trimite, in functie de comanda, cereri către server. Scopul lui este de a functiona
    ca o interfata in linia de comanda (CLI) cu biblioteca virtuala simulata de catre server.
    Acesta va expune un API (Application Programmable Interface) de tip REST
    (Representational State Transfer), si va primi o serie de intrări reprezentate de rute
    HTTP. În urma cererilor HTTP, serverul efectuează o acţiune si returnează un răspuns
    HTTP. Clientul va interpreta răspunsul primit si va afisa un mesaj corespunzător.

## Obiective:

- Intelegerea mecanismelor protocolului HTTP
- Interactiunea cu un REST API
- Intelegerea conceptelor des folosite in web precum JSON, sesiune, JWT
- Utilizarea unor biblioteci externe pentru manipularea obiectelor JSON REST API


## Mod de functionare:

    Clientul interpreteaza comenzile de la tastatura pentru a putea comunica cu serverul.
    In urma primirii comenzii, clientul va forma obiectul JSON (daca e cazul) vaexecuta
    cererea catre server si va interpreta raspunsul primit. In functie de raspuns, clientul
    va afisa mesajul coresponzator de succes sau de eroare.

## Comenzile implementate:

- `register` - efectueaza inregistrarea
- `login` - efectueaza autehntificarea
- `enter_library` - cere accesul la biblioteca
- `get_books` - cere lista de carti
- `get_book` - cere informatii despre o carte
- `add_book` - adauga o carte
- `delete_book` - sterge o carte
- `logout` - efectueaza delogarea
- `exit` - iesirea din aplicatie


## Implementare:

### Structura fisierelor:

Proiectul este structurat in mai multe fisiere, fiecare cu rolul sau:

- `client.c` - fisierul principal al clientului, aici se afla functia principala main
si logica de procesare a comenzilor
- `helpers.c` si `helpers.h` - contine functii ajutatoare pentru parsarea raspunsurilor 
de la si catre server, si de asemenea pentru deschiderea si inchiderea conexiunii catre
server
- `requests.c` si `requests.h` - contine functiile pentru construirea cererilor HTTP
  (GET, POST, DELETE) catre server


### Functia principala:
Functia main este responsabila pentru bucla principala de executie a programului.
Aici se citeste comanda de la tastatura, se deschide conexiunea cu serverul, se executa
comanda coresponzatoare si se afiseaza rezultatele.

### Comenzi:

- `register` - este apelată pentru a inregistra un nou utilizator. Aceasta colecteaza
numele de utilizator și parola, valideaza intrarile și trimite o cerere POST la server.
In urma raspunsului, se afiseaza un mesaj corespunzator.

- `login` - este apelată pentru a autentifica un utilizator. Aceasta colecteaza numele
de utilizator și parola, valideaza intrarile și trimite o cerere POST la server.
In urma raspunsului, se afiseaza un mesaj corespunzator.

- `enter_library` - este apelată pentru a cere accesul la biblioteca. Aceasta trimite
o cerere GET la server pentru a obtine un token JWT. In urma raspunsului, se afiseaza
un mesaj corespunzator.

- `get_books` - este apelată pentru a cere lista de carti. Aceasta trimite o cerere GET
la server pentru a obtine lista de carti. In urma raspunsului, se afiseaza un mesaj
corespunzator.

- `get_book` - este apelată pentru a cere informatii despre o carte. Aceasta colecteaza
id-ul cartii, valideaza intrarea si trimite o cerere GET la server pentru a obtine
informatii despre carte. In urma raspunsului, se afiseaza un mesaj corespunzator.

- `add_book` - este apelată pentru a adauga o carte. Aceasta colecteaza titlul, autorul,
genul, editorul si numarul de pagini al cartii, creaza un obiect JSON si trimite o cerere
POST la server pentru a adauga cartea. In urma raspunsului, se afiseaza un mesaj corespunzator.

- `delete_book` - este apelată pentru a sterge o carte. Aceasta colecteaza id-ul cartii,
valideaza intrarea si trimite o cerere DELETE la server pentru a sterge cartea. In urma
raspunsului, se afiseaza un mesaj corespunzator.

- `logout` - este apelată pentru a deloga utilizatorul. Aceasta trimite o cerere GET la
server pentru a deloga utilizatorul. In urma raspunsului, se afiseaza un mesaj corespunzator.

- `exit` - este apelată pentru a iesi din aplicatie.


### Functii de validare a intrarilor:

    contain_spaces - verifica daca un string contine spatii
    contain_only_spaces - verifica daca un string contine doar spatii
    contains_digits - verifica daca un string contine cifre

### Mesaje de eroare:

    error_message - extrage si afiseaza mesajele de eroare din raspunsurile serverului

Pentru parsarea raspunsurilor de la server, am folosit biblioteca parson deoarece
are o interfata simpla si usor de folosit, chiar daca nu eram familiarizat cu 
manipularea JSON in limbajul C. Dupa ce m-am documentat putin am inteles ca ofera
multe functionalitati necesare, cum ar fi crearea, parsarea, modificarea si 
serializarea obiectelor JSON. De asemenea este o biblioteca portabilă, eficienta
si rapida, ceea ce e esential pentru aplicatii care necesita parsarea rapida si
frecventa a obiectelor JSON.


### Exemplu de rulare:

Pentru rularea utomata a clientului, se poate folosi comanda:

    python3 checkr/checker.py --script ALL ./client
    
