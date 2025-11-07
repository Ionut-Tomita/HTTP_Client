
# 🌐 Client HTTP în C pentru interacțiunea cu un REST API

📘 **Descriere:**

Clientul este un program scris în C care acceptă comenzi de la tastatură (stdin)  
și trimite, în funcție de comandă, cereri către server. Scopul lui este de a funcționa  
ca o interfață în linia de comandă (CLI) cu biblioteca virtuală simulată de către server.  
Acesta va expune un API (Application Programmable Interface) de tip **REST**  
(Representational State Transfer) și va primi o serie de intrări reprezentate de rute **HTTP**.  
În urma cererilor HTTP, serverul efectuează o acțiune și returnează un răspuns HTTP.  
Clientul va interpreta răspunsul primit și va afișa un mesaj corespunzător.

---

## 🎯 Obiective:

- 🧠 Înțelegerea mecanismelor protocolului **HTTP**
- 🔗 Interacțiunea cu un **REST API**
- 📦 Înțelegerea conceptelor des folosite în web precum **JSON**, **sesiune**, **JWT**
- 🧰 Utilizarea unor biblioteci externe pentru manipularea obiectelor **JSON REST API**

---

## ⚙️ Mod de funcționare:

Clientul interpretează comenzile de la tastatură pentru a putea comunica cu serverul.  
În urma primirii comenzii, clientul va forma obiectul **JSON** (dacă e cazul), va executa  
cererea către server și va interpreta răspunsul primit. În funcție de răspuns, clientul  
va afișa mesajul corespunzător de succes sau de eroare.

---

## 💻 Comenzile implementate:

- 📝 `register` - efectuează înregistrarea  
- 🔐 `login` - efectuează autentificarea  
- 📚 `enter_library` - cere accesul la bibliotecă  
- 📖 `get_books` - cere lista de cărți  
- 📘 `get_book` - cere informații despre o carte  
- ➕ `add_book` - adaugă o carte  
- 🗑️ `delete_book` - șterge o carte  
- 🚪 `logout` - efectuează delogarea  
- ❌ `exit` - ieșirea din aplicație  

---

## 🧩 Implementare:

### 📂 Structura fișierelor:

Proiectul este structurat în mai multe fișiere, fiecare cu rolul său:

- `client.c` – fișierul principal al clientului; aici se află funcția principală **main**  
  și logica de procesare a comenzilor.  
- `helpers.c` și `helpers.h` – conțin funcții ajutătoare pentru parsarea răspunsurilor  
  de la și către server, precum și pentru deschiderea și închiderea conexiunii.  
- `requests.c` și `requests.h` – conțin funcțiile pentru construirea cererilor **HTTP**
  (**GET**, **POST**, **DELETE**) către server.  

---

### 🧠 Funcția principală:

Funcția **main** este responsabilă pentru bucla principală de execuție a programului.  
Aici se citește comanda de la tastatură, se deschide conexiunea cu serverul, se execută  
comanda corespunzătoare și se afișează rezultatele.

---

### 🧾 Comenzi detaliate:

- 📝 **`register`** – este apelată pentru a înregistra un nou utilizator. Aceasta colectează  
  numele de utilizator și parola, validează intrările și trimite o cerere **POST** la server.  
  În urma răspunsului, se afișează un mesaj corespunzător.

- 🔐 **`login`** – este apelată pentru a autentifica un utilizator. Aceasta colectează  
  numele de utilizator și parola, validează intrările și trimite o cerere **POST** la server.  
  În urma răspunsului, se afișează un mesaj corespunzător.

- 📚 **`enter_library`** – este apelată pentru a cere accesul la bibliotecă. Aceasta trimite  
  o cerere **GET** la server pentru a obține un token **JWT**. În urma răspunsului, se afișează  
  un mesaj corespunzător.

- 📖 **`get_books`** – este apelată pentru a cere lista de cărți. Trimite o cerere **GET**  
  la server și afișează lista obținută sau mesajul corespunzător de eroare.

- 📘 **`get_book`** – este apelată pentru a cere informații despre o carte. Colectează  
  ID-ul cărții, validează intrarea și trimite o cerere **GET** pentru a obține informațiile.  

- ➕ **`add_book`** – este apelată pentru a adăuga o carte. Colectează titlul, autorul,  
  genul, editorul și numărul de pagini, creează un obiect **JSON** și trimite o cerere  
  **POST** către server.  

- 🗑️ **`delete_book`** – este apelată pentru a șterge o carte. Colectează ID-ul, validează  
  intrarea și trimite o cerere **DELETE** la server.  

- 🚪 **`logout`** – este apelată pentru a deloga utilizatorul. Trimite o cerere **GET**  
  către server și afișează mesajul de confirmare.  

- ❌ **`exit`** – este apelată pentru a ieși din aplicație.  

---

### 🧮 Funcții de validare a intrărilor:

- ⚠️ `contain_spaces` – verifică dacă un string conține spații  
- ⚠️ `contain_only_spaces` – verifică dacă un string conține doar spații  
- ⚠️ `contains_digits` – verifică dacă un string conține cifre  

---

### 🚫 Mesaje de eroare:

- ❗ `error_message` – extrage și afișează mesajele de eroare din răspunsurile serverului  

Pentru parsarea răspunsurilor de la server, s-a folosit biblioteca **Parson**,  
deoarece are o interfață simplă și ușor de utilizat, chiar și pentru cineva nefamiliarizat  
cu manipularea **JSON** în limbajul C.  

După o scurtă documentare, am observat că oferă toate funcționalitățile necesare:
- Crearea, parsarea și modificarea obiectelor JSON  
- Serializarea acestora  
- Performanță bună, portabilitate și eficiență — ideale pentru aplicații care necesită  
parsare rapidă și frecventă a obiectelor JSON.  

---

### ▶️ Exemplu de rulare:

Pentru rularea automată a clientului, se poate folosi comanda:

```bash
python3 checkr/checker.py --script ALL ./client
