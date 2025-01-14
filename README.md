# sistema-domotico

Componenti:

- Daniele Piccolo
- Eros Menin
- Francesco Dotoli

## Funzionalità Aggiuntive

La politica di spegnimento dei dispositivi in caso di sovraccarico elettrico è stata gestita tramite lo spegnimento
dei dispositivi nell’ordine inverso rispetto all’accensione fino ad ottenere una potenza assorbita minore di quella prodotta (come da consegna).
In questo caso si è deciso di esonerare il dispositivo appena accesso dalla lista di quelli da spegnere, poiché si presume che sia nell'interesse dell'utente che l'ultimo dispositivo acceso rimanga a discapito di altri.

Esempio: Se nel momento in cui si esegue il comando `set time xx:xx` si accende un dispositivo, ad esempio "Lavatrice", e si verifica un sovraccarico della rete, il programma inizierà a spegnere i dispositivi in ordine inverso rispetto all'accensione, ma da tale conteggio il dispositivo appena acceso, nel caso attuale "Lavatrice", verrà escluso dalla lista dei dispositivi da spegnere (a meno che non sia strettamente necessario).

In aggiunta è stata implementata una "white list" che contiene i dispositivi che non vengono spenti in caso di sovraccarico (se possibile), poiché considerati importanti. I dispositivi appartenenti a tale lista sono contrassegnati da un flag booleano "WhiteList" all'interno del file "devices.json".
Se un dispositivo viene acceso e si ha un sovraccarico, pittosto che spegnere un dispositivo in white list, il programma spegne il dispositivo appena acceso (informando con un apposito messaggio).

Inoltre il comando `set {Device} {start} [{stop}]` se il dispositivo è già acceso ignora l'orario di accesione e, nel caso sia un dispositivo manuale, aggiunge solo l'orario di spegnimento.

## Modifiche ai dispositivi

Il numero dei dispositivi utilizzati ed il tipo non variano rispetto a quanto definito dal testo del progetto, tuttavia sono state apportate modifiche legate a priorità e orari. Tali modifiche sono state apportate tramite apposite voci nel file "devices.json".

Le modifiche legate all'orario riguardano alcuni dispositivi del tipo Manuale e consistono nell'aver già preimpostato un orario di accensione e spegnimento:

- Per l'"Impianto fotovoltaico" è stato definito un orario di accensione alle "08:00" e un orario di spegnimento alle "18:00";
- Per il "Frigorifero" è stato definito un orario di accensione alle "00:00" e un orario di spegnimento alle "23:59".
  Entrambi sono poi stati inseriti nella white list.
