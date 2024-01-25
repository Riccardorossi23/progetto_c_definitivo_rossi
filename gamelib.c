#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
static void clear_buffer(void);
bool setImpostazioni; // set delle impostazioni, ci controlla che tutte le impostazioni siano tutte impostate correttamente
bool setDifficoltà;
bool setCreazioneGiocatori;
bool mappa_valida;        // la creazione della mappa sia avvenuta correttamente
bool setmappa;            // il settaggio della mappa sia avvenuto corettamente
bool passa_turno;         // ci permette di passare il turno al giocatore successivo
bool stop_tornacaravan;   // se compare il fantasma, con questa variabile quel determinato giocatore non potrà tornare al caravan
bool uso_sale;            // se un giocatore ha nello zaino l'oggetto sale e l'ho usa, questo bool diventerà true e gli permetterà di non prendere danno dal fantasma
bool marcato_spirit_box;  // quando torni al caravan e depositi la prova spirit_box, quello bool si attiverà
bool marcato_videocamera; // quando torni al caravan e depositi la prova videocamera, quello bool si attiverà
bool marcato_emf;         // quando torni al caravan e depositi la prova EMF, quello bool si attiverà
// variabili termina partita
int rigioco; // ci permette di scegliere se rigiocare, o impostare il gioco oppure se terminare la partita
// variabile difficoltà
int difficoltà_gioco;          // ci permette di settare la difficoltà del gioco, scegliendo tra 3 livelli (Dilettante, intermedio, incubo)
int decremento_sanità_mentale; // inbase al livello scelto, la sanità mentale verrà decrementata quando compare il fantasma
int apparizione_fantasma;      // inbase a livello scelto, il fantasma apparirirà con maggiore probabilità quando raccogliamo una prova
int opzioni;                   // opzioni di scelta del menu principale(imposta_gioco, gioca,termina_gioco)
// strutture giocatori
struct Giocatore *giocatore1;
struct Giocatore *giocatore2;
struct Giocatore *giocatore3;
struct Giocatore *giocatore4;
// giocatori creati
int n_players; // numero di giocatori
// char per identificare e dare i nomi ai giocatori
char nome_giocatore1[20];
char nome_giocatore2[20];
char nome_giocatore3[20];
char nome_giocatore4[20];
// oggetti zaino
int oggetti_zaino[5];
int oggetto_selezionato; // varabile che ci permette di selezionare un oggetto tra quelli genareti nell'oggetto_zaino e asseganrlo al giocatore
int len = 5;
int i, r, temp, r_ungiocatore;
// mappa
int random_oggetto;
struct Zona_Mappa *node;
int value_mappa; // comandi per "navigare" nel menu mappa
int zone;        // numero di zone che deve creare
int z;
int zone_mappa;          // genera in modo casuale le stanze della zona
int array_zone_mappa[7]; // numero stanze massimo per la mappa

// gioco
int scelta_gioco;        // comandi per "navigare" nel menu gioca
int random_oggettizaino; // utilizzo con cento_dollari, random casuali di selezione tra slae e calmanti
int turni_avanzati;      // costante, che non mi permette di avavnzare più di una volta in modo consecutivo
struct Zona_Mappa *pCenter;
struct Zona_Mappa *pNext;
int giocatori_ciclo = 1;

struct Zona_Mappa *pFirst = NULL;
struct Zona_Mappa *pLast = NULL;
// metodi
void difficoltà();
void imposta_gioco();
void creazione_giocatori();
void generatore_oggetti();
void generatore_oggettipl3_4();
void oggetti_giocatore();
void Mappa();
void gioca();
void stampa_giocatore();
void torna_caravan();
void raccogli_oggetto();
void utilizza_oggetto();
void avanza();
void stampa_zona();
void passa();
void menugioca_1nplayers();
void menugioca_2nplayers();
void menugioca_3nplayers();
void menugioca_4nplayers();
void menu_gioca();
void apparizionefantasma();

// Funzione per ripulire il buffer dopo un inserimento da tastiera
static void pulizia_buffer(void)
{
    while (getchar() != '\n')
        ;
}
// cancello il contenuto di tutte le variabili,strutture e setto a false booleani
void termina_gioco()
{
    setImpostazioni = false;
    setDifficoltà = false;
    setCreazioneGiocatori = false;
    mappa_valida = false;
    setmappa = false;
    switch (n_players)
    {
    case 1:
        printf("\x1b[33m%s\x1b[37m ha terminato il gioco\n", giocatore1->nickname);
        break;
    case 2:
        printf("\x1b[33m%s\x1b[37m ha terminato il gioco\n", giocatore1->nickname);
        printf("\x1b[32m%s\x1b[37m ha terminato il gioco\n", giocatore2->nickname);
        break;
    case 3:
        printf("\x1b[33m%s\x1b[37m ha terminato il gioco\n", giocatore1->nickname);
        printf("\x1b[32m%s\x1b[37m ha terminato il gioco\n", giocatore2->nickname);
        printf("\x1b[35m%s\x1b[37m ha terminato il gioco\n", giocatore3->nickname);
        break;
    case 4:
        printf("\x1b[33m%s\x1b[37m ha terminato il gioco\n", giocatore1->nickname);
        printf("\x1b[32m%s\x1b[37m ha terminato il gioco\n", giocatore2->nickname);
        printf("\x1b[35m%s\x1b[37m ha terminato il gioco\n", giocatore3->nickname);
        printf("\x1b[34m%s\x1b[37m ha terminato il gioco\n", giocatore4->nickname);
        break;
    default:
        printf("\x1b[31m errore \x1b[37m\n");
        break;
    }
    free(giocatore1);
    free(giocatore2);
    free(giocatore3);
    free(giocatore4);
    random_oggetto = 0;
    n_players = 0;
    difficoltà_gioco = 0;
    value_mappa = 0;
    opzioni = 0;
    oggetto_selezionato = 0;
    zone = 0;
    z = 0;
    zone_mappa = 0;
    free(pCenter);
    free(pNext);
    free(node);
    free(pLast);
    // chiedo se vuoi rigiocare o meno, se si ti manda al menù d'inizio gioco, se è no invece termina la partita
    printf("vuoi rigiocare si(1)|no(0)? ");
    scanf("%d", &rigioco);
    pulizia_buffer();
    if (rigioco == 1)
    {
        do
        {
            printf("\n");
            printf("|||||||||||||||||||MENU PRINICIPALE||||||||||||||||||||||||\n");
            printf("scegli tra imposta gioco, gioca, termina gioco\n"); // non funziona
            printf("per scegliere imposta gioco premere 1 \n");
            printf("per scgliere gioca premere 2 \n");
            printf("per scegliere termina gioco premere 3 \n");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            printf("seleziona le opzioni: ");
            scanf("%d", &opzioni);
            while (getchar() != '\n')
                ;
            switch (opzioni)
            {
            case 1:
                imposta_gioco(); // non funziona
                break;
            case 2:
                gioca();
                break;
            default:
                printf(" Attenzione! Opzione non valida, per favore inserisci numero da 1 a 3.\n");
                break;
            }
        } while (opzioni != 2);
        printf("selezioni un valore corretto \n");
        pulizia_buffer();
    }
    else
    {
        printf("termina partita\n");
    }
    // alla fine di gioca creare free di pulizia dati
}
void imposta_gioco() // imposta gioco, mi permetti di inizializzare le impostazione partendo da: difficoltà e poi crea giocatore
{
    if (setCreazioneGiocatori == false)
    {
        printf("Inserire il numero di giocatori tra 1 e 4: ");
        scanf("%d", &n_players); // prende da tastiera il numero dei giocatori
        pulizia_buffer();
        creazione_giocatori();
    }
    else if (setDifficoltà == false) // se l'impostatzioni non vengono settate o viene sbagliato il settaggio, quindi il set impostazioni=false, mi da l'opportunità di ri-settarle
    {
        difficoltà(); // chiamo metodo difficoltà
    }
    else if (setmappa == false) // se il set impostatzi è settato in true, mi va avanti e mi osserva se setmappa è settato in mainiera giousta o no, se non fosse settato in maniera giusta, mi da l'opportunità di ri-settarle
    {
        Mappa(); // chiamo metodo mappa()
    }
    else // invece se è settato in maniera giusta mi riporta al menu d'inzio per darmi la possibilità di scegliere se giocare, terminare la partita o ri-settarle le impostatzioni
    {
        printf("le impostazioni del gioco, sono avvenute correttamente, digita 2 per giocare\n");
        do
        {
            printf("\n");
            printf("|||||||||||||||||||MENU PRINICIPALE||||||||||||||||||||||||\n");
            printf("scegli tra imposta gioco, gioca, termina gioco\n");
            printf("per scegliere imposta gioco premere 1 \n");
            printf("per scgliere gioca premere 2 \n");
            printf("per scegliere termina gioco premere 3 \n");
            printf("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
            printf("seleziona le opzioni: ");
            scanf("%d", &opzioni);
            while (getchar() != '\n')
                ;
            switch (opzioni)
            {
            case 1:
                imposta_gioco();
                break;
            case 2:
                gioca();
                break;
            case 3:
                termina_gioco();
                break;
            default:
                printf(" Attenzione! Opzione non valida, per favore inserisci numero da 1 a 3.\n");
                break;
            }
        } while (opzioni != 3);
        printf("selezioni un valore corretto \n");
        pulizia_buffer();
        setImpostazioni = true;
    }
}
void difficoltà() // metodo che ti fa selezionare il livello di difficoltà del gioco, dopo averlo selizionato di setta setimpostazioni e setdifficoltà=true
{
    printf("\n***********************************\x1b[36m Set Difficoltà \x1b[37m*******************************************\n");
    printf("1:difficoltà dilettante\n");
    printf("2:difficoltà intermedio\n");
    printf("3:difficoltà incubo\n");
    printf("**********************************************************************************************\n");
    printf("inserire la difficoltà del gioco: ");
    scanf("%d/n", &difficoltà_gioco);
    pulizia_buffer();
    if (difficoltà_gioco == 1) // se il giocatore sceglie difficoltà dilettante, la sanità mentale decrescerebbe di 10 in 10, e l'apparizione fantasma sarebbe impostata su 10% di probabilità che può comparire il fantasma
    {
        printf("La difficoltà del gioco è stata impostato su:\x1b[32m dilettante \x1b[37m\n");
        setImpostazioni = true;
        setDifficoltà = true;
        decremento_sanità_mentale = 10; 
        apparizione_fantasma = 20;
        if (n_players == 1 || n_players == 2)
        {
            generatore_oggetti(); // dopo aver impostato il gioco in maniera coretta, vado a generare gli oggetti da mettere nello zaino
        }
        else if (n_players == 3 || n_players == 4)
        {
            generatore_oggettipl3_4(); // dopo aver impostato il gioco in maniera coretta, vado a generare gli oggetti da mettere nello zaino
        }
        else
        {
            printf("\x1b[31m errore \x1b[37m\n");
        }
    }
    else if (difficoltà_gioco == 2) // se il giocatore sceglie difficoltà dilettante, la sanità mentale decrescerebbe di 20 in 20, e l'apparizione fantasma sarebbe impostata su 25% di probabilità che può comparire il fantasma
    {
        printf("La difficoltà del gioco è stata impostato su:\x1b[33m intermedio \x1b[37m\n");
        setImpostazioni = true;
        setDifficoltà = true;
        decremento_sanità_mentale = 20;
        apparizione_fantasma = 25;
        if (n_players == 1 || n_players == 2)
        {
            generatore_oggetti(); // dopo aver impostato il gioco in maniera coretta, vado a generare gli oggetti da mettere nello zaino
        }
        else if (n_players == 3 || n_players == 4)
        {
            generatore_oggettipl3_4(); // dopo aver impostato il gioco in maniera coretta, vado a generare gli oggetti da mettere nello zaino
        }
        else
        {
            printf("\x1b[31m errore \x1b[37m\n");
        }
    }
    else if (difficoltà_gioco == 3) // se il giocatore sceglie difficoltà dilettante, la sanità mentale decrescerebbe di 30 in 30, e l'apparizione fantasma sarebbe impostata su 40% di probabilità che può comparire il fantasma
    {
        printf("La difficoltà del gioco è stata impostato su:\x1b[31m incubo \x1b[37m\n");
        setImpostazioni = true;
        setDifficoltà = true;
        decremento_sanità_mentale = 30;
        apparizione_fantasma = 40;
        if (n_players == 1 || n_players == 2)
        {
            generatore_oggetti(); // dopo aver impostato il gioco in maniera coretta, vado a generare gli oggetti da mettere nello zaino
        }
        else if (n_players == 3 || n_players == 4)
        {
            generatore_oggettipl3_4(); // dopo aver impostato il gioco in maniera coretta, vado a generare gli oggetti da mettere nello zaino
        }
        else
        {
            printf("\x1b[31m errore \x1b[37m\n");
        }
    }
    else // se si sbagliase a digitare, uscirebbe sullo schermo questo errore e poi ti farebbe iniziare dall'inizio del metodo
    {
        printf("\x1b[31m errore, inserire un numero tra 1 e 3\x1b[37m\n");
        setDifficoltà = false;
        difficoltà();
    }
}
void creazione_giocatori()
{
    do
    {
        switch (n_players) // switch(inbase al numero di giocatori), creo quest'ultimi, se lo switch va a buon fine setto setimpostazioni e setcrearegiocatori=true
        {
        case 1:
            giocatore1 = (struct Giocatore *)malloc(sizeof(struct Giocatore)); // vado a creare la struct di giocatore1 chiamando il malloc e la struct Giocatore
            {
                printf("inserire il nome del giocatore1: ");
                scanf("%s", nome_giocatore1);                  // imposto il nome
                strcpy(giocatore1->nickname, nome_giocatore1); // gli passo il nome a giocatore1
                giocatore1->sanità_mentale = 100;              // gli imposto sanità mentale
                printf("il primo giocatore \x1b[33m%s\x1b[37m avrà la sanità: %d\n", giocatore1->nickname, giocatore1->sanità_mentale);
                pulizia_buffer();
                setImpostazioni = true;
                setCreazioneGiocatori = true;
                imposta_gioco(); // richiamo metodo imposta_gioco, così da impostare i rimanenti comandi
            }
            break;
        case 2:
            giocatore1 = (struct Giocatore *)malloc(sizeof(struct Giocatore));
            giocatore2 = (struct Giocatore *)malloc(sizeof(struct Giocatore));
            {
                printf("inserire il nome del giocatore1: ");
                scanf("%s", nome_giocatore1);
                strcpy(giocatore1->nickname, nome_giocatore1);
                giocatore1->sanità_mentale = 100;

                printf("inserire il nome del giocatore2: ");
                scanf("%s", nome_giocatore2);
                strcpy(giocatore2->nickname, nome_giocatore2);
                giocatore2->sanità_mentale = 100;

                printf("il primo giocatore \x1b[33m%s\x1b[37m avrà la sanità: %d\n", giocatore1->nickname, giocatore1->sanità_mentale);
                printf("il secondo giocatore \x1b[32m%s\x1b[37m avrà la sanità: %d\n", giocatore2->nickname, giocatore2->sanità_mentale);
                pulizia_buffer();
                setImpostazioni = true;
                setCreazioneGiocatori = true;
                imposta_gioco();
            }
            break;
        case 3:
            giocatore1 = (struct Giocatore *)malloc(sizeof(struct Giocatore));
            giocatore2 = (struct Giocatore *)malloc(sizeof(struct Giocatore));
            giocatore3 = (struct Giocatore *)malloc(sizeof(struct Giocatore));
            {
                printf("inserire il nome del giocatore1: ");
                scanf("%s", nome_giocatore1);
                strcpy(giocatore1->nickname, nome_giocatore1);
                giocatore1->sanità_mentale = 100;

                printf("inserire il nome del giocatore2: ");
                scanf("%s", nome_giocatore2);
                strcpy(giocatore2->nickname, nome_giocatore2);
                giocatore2->sanità_mentale = 100;

                printf("inserire il nome del giocatore3: ");
                scanf("%s", nome_giocatore3);
                strcpy(giocatore3->nickname, nome_giocatore3);
                giocatore3->sanità_mentale = 100;

                printf("il primo giocatore \x1b[33m%s\x1b[37m avrà la sanità: %d\n", giocatore1->nickname, giocatore1->sanità_mentale);
                printf("il secondo giocatore \x1b[32m%s\x1b[37m avrà la sanità: %d\n", giocatore2->nickname, giocatore2->sanità_mentale);
                printf("il terzo giocatore \x1b[35m%s\x1b[37m avrà la sanità: %d\n", giocatore3->nickname, giocatore3->sanità_mentale);
                pulizia_buffer();
                setImpostazioni = true;
                setCreazioneGiocatori = true;
                imposta_gioco();
            }
            break;
        case 4:
            giocatore1 = (struct Giocatore *)malloc(sizeof(struct Giocatore));
            giocatore2 = (struct Giocatore *)malloc(sizeof(struct Giocatore));
            giocatore3 = (struct Giocatore *)malloc(sizeof(struct Giocatore));
            giocatore4 = (struct Giocatore *)malloc(sizeof(struct Giocatore));
            {
                printf("inserire il nome del giocatore1: ");
                scanf("%s", nome_giocatore1);
                strcpy(giocatore1->nickname, nome_giocatore1);
                giocatore1->sanità_mentale = 100;

                printf("inserire il nome del giocatore2: ");
                scanf("%s", nome_giocatore2);
                strcpy(giocatore2->nickname, nome_giocatore2);
                giocatore2->sanità_mentale = 100;

                printf("inserire il nome del giocatore3: ");
                scanf("%s", nome_giocatore3);
                strcpy(giocatore3->nickname, nome_giocatore3);
                giocatore3->sanità_mentale = 100;

                printf("inserire il nome del giocatore4: ");
                scanf("%s", nome_giocatore4);
                strcpy(giocatore4->nickname, nome_giocatore4);
                giocatore4->sanità_mentale = 100;

                printf("il primo giocatore \x1b[33m%s\x1b[37m avrà la sanità: %d\n", giocatore1->nickname, giocatore1->sanità_mentale);
                printf("il secondo giocatore \x1b[32m%s\x1b[37m avrà la sanità: %d\n", giocatore2->nickname, giocatore2->sanità_mentale);
                printf("il terzo giocatore \x1b[35m%s\x1b[37m avrà la sanità: %d\n", giocatore3->nickname, giocatore3->sanità_mentale);
                printf("il quarto giocatore \x1b[34m%s\x1b[37m avrà la sanità: %d\n", giocatore4->nickname, giocatore4->sanità_mentale);
                pulizia_buffer();
                setImpostazioni = true;
                setCreazioneGiocatori = true;
                imposta_gioco();
            }
            break;
        default:
            printf("\x1b[31m inserire un valore tra 1 e 4 \x1b[37m\n");
            setImpostazioni = false;
            setCreazioneGiocatori = false;
            imposta_gioco();
            break;
        }
    } while (n_players != 4 && n_players > 0);
}
void generatore_oggetti() // in maniera casulai, ma non li genera doppi(non genera lo stesso oggetto)
{
    time_t t;
    srand((unsigned)time(&t));
    int numero[n_players];

    // Riempi l'array con i numeri desiderati
    for (temp = 0, i = 0; temp < len; i++, temp++)
        numero[temp] = i;

    for (i = len - 1; i > 0; i--)
    {
        r_ungiocatore = rand() % 3; // seleziona un numero casuale
        r = 3 + rand() % 2;         // seleziona un numero casuale

        // se i numeri generati sono uguali l'ultimo inserito lo va a sostituire in modo casuale
        temp = numero[i];
        numero[i] = numero[r_ungiocatore];
        numero[r_ungiocatore] = temp;
        // r
        temp = numero[i];
        numero[i] = numero[r];
        numero[r] = temp;
    }
    printf("\n*******************************\x1b[36m Oggetti Generati \x1b[37m*********************************\n");
    switch (r_ungiocatore)
    {
    case 0:
    {
        printf("------------------------\n");
        printf("oggetto:0(EMF)         |\n");
        printf("------------------------\n");
        oggetti_zaino[i] = 0;
    }
    break;
    case 1:
    {
        printf("------------------------\n");
        printf("oggetto:1(spirit_box)  |\n");
        printf("------------------------\n");
        oggetti_zaino[i] = 1;
    }
    break;
    case 2:
    {
        printf("------------------------\n");
        printf("oggetto:2(videocamera) |\n");
        printf("------------------------\n");
        oggetti_zaino[i] = 2;
    }
    break;
    default:

        printf("\x1b[31m errore \x1b[37m\n");
        break;
    }
    if (n_players == 2)
    {
        switch (r)
        {
        case 3:
        {
            printf("------------------------\n");
            printf("oggetto:3(calmanti)    |\n");
            printf("------------------------\n");
            oggetti_zaino[i] = 3;
        }
        break;
        case 4:
        {
            printf("------------------------\n");
            printf("oggetto:4(sale)        |\n");
            printf("------------------------\n");
            oggetti_zaino[i] = 4;
        }
        break;
        default:
            printf("\x1b[31m errore \x1b[37m\n");
            break;
        }
    }
    printf("***********************************************************************************\n");
    setImpostazioni = true;
    oggetti_giocatore();
}

void generatore_oggettipl3_4()
{
    time_t t;
    srand((unsigned)time(&t));
    int r;
    int numero[n_players];
    // Riempi l'array con i numeri desiderati
    for (temp = 0, i = 0; temp < len; i++, temp++)
        numero[temp] = i;

    for (i = len - 1; i > 0; i--)
    {
        r = rand() % 5;
        // se i numeri generati sono uguali l'ultimo inserito lo va a sostituire in modo casuale
        temp = numero[i];
        numero[i] = numero[r];
        numero[r] = temp;
    }
    printf("\n*******************************\x1b[36m Oggetti Generati \x1b[37m*****************************\n");
    for (i = 0; i < n_players; ++i)
    {
        printf("n:%d\n", numero[i]);
        switch (numero[i])
        {
        case 0:
        {
            printf("------------------------\n");
            printf("oggetto:0(EMF)         |\n");
            printf("------------------------\n");
            oggetti_zaino[i] = 0;
        }
        break;
        case 1:
        {
            printf("------------------------\n");
            printf("oggetto:1(spirit_box)  |\n");
            printf("------------------------\n");
            oggetti_zaino[i] = 1;
        }
        break;
        case 2:
        {
            printf("------------------------\n");
            printf("oggetto:2(videocamera) |\n");
            printf("------------------------\n");
            oggetti_zaino[i] = 2;
        }
        break;
        case 3:
        {
            printf("------------------------\n");
            printf("oggetto:3(calmanti)    |\n");
            printf("------------------------\n");
            oggetti_zaino[i] = 3;
        }
        break;
        case 4:
        {
            printf("------------------------\n");
            printf("oggetto:4(sale)        |\n");
            printf("------------------------\n");
            oggetti_zaino[i] = 4;
        }
        break;
        default:
            printf("\x1b[31m errore \x1b[37m\n");
            break;
        }
    }
    printf("*******************************************************************************\n");
    setImpostazioni = true;
    oggetti_giocatore();
}

void oggetti_giocatore()
{
    time_t t;
    srand((unsigned)time(&t));
    int n_players2, n_players3, n_players4;
    int numero[n_players];

    // Riempi l'array con i numeri desiderati
    for (temp = 0, i = 0; temp < len; i++, temp++)
        numero[temp] = i;

    for (i = len - 1; i > 0; i--)
    {
        // random n_player2
        n_players2 = 1 + rand() % 2;
        temp = numero[i];
        numero[i] = numero[n_players2];
        numero[n_players2] = temp;

        // random n_player3
        n_players3 = 1 + rand() % 3;
        temp = numero[i];
        numero[i] = numero[n_players3];
        numero[n_players3] = temp;

        // random n_player4
        n_players4 = 1 + rand() % 4;
        temp = numero[i];
        numero[i] = numero[n_players4];
        numero[n_players4] = temp;
    }

    switch (n_players) // switch giocatori, setto il primo slot dello zaino del giocatore, imbase agli oggetti generati, prima di assegnare l'oggetto faccio diversi controlli così da essere sicuro che quando vado ad asseganre l'oggetto, venga assegnato correttamente
    {
    case 1: // n_player=giocatore1
    scelta_giocatore1:
        printf("l'oggetto selezionato da \x1b[33m%s\x1b[37m è: ", giocatore1->nickname);
        scanf("%d", &oggetto_selezionato);
        pulizia_buffer();
        if (r_ungiocatore == oggetto_selezionato)
        {
            giocatore1->zaino[0] = oggetto_selezionato;
            printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
        }
        else
        {
            printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
            goto scelta_giocatore1;
        }
        break;
    case 2: // n_player=giocatore2
    oggetti_giocatore2:
        switch (n_players2)
        {
        case 1:
            for (i = 0; i < n_players; ++i)
            {
                printf("zaino:%d\n", oggetti_zaino[i]);
            }
            printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (r_ungiocatore == oggetto_selezionato || r == oggetto_selezionato)
            {
                giocatore1->zaino[0] = oggetto_selezionato;
                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
            }
            else
            {
                printf("\x1b[31m errore, devi scegliere l'oggetto generato\x1b[37m\n");
                goto oggetti_giocatore2;
            }
        oggetti_giocatore2_1:
            printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (r_ungiocatore == oggetto_selezionato || r == oggetto_selezionato && oggetto_selezionato != giocatore1->zaino[0])
            {
                giocatore2->zaino[0] = oggetto_selezionato;
                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
            }
            else
            {
                printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dal primo giocatore \x1b[37m\n");
                goto oggetti_giocatore2_1;
            }
            break;
        case 2:
        oggetti_giocatore2_2:
            for (i = 0; i < n_players; ++i)
            {
                printf("zaino:%d\n", oggetti_zaino[i]);
            }
            printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (r_ungiocatore == oggetto_selezionato || r == oggetto_selezionato)
            {
                giocatore2->zaino[0] = oggetto_selezionato;
                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
            }
            else
            {
                printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                goto oggetti_giocatore2_2;
            }
        oggetti_giocatore1_2:
            printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (r_ungiocatore == oggetto_selezionato || r == oggetto_selezionato && oggetto_selezionato != giocatore2->zaino[0])
            {
                giocatore1->zaino[0] = oggetto_selezionato;
                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
            }
            else
            {
                printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                goto oggetti_giocatore1_2;
            }
            break;
        default:
            printf("\x1b[31m errore \x1b[37m\n");
            break;
        }
        break;

    case 3:
        switch (n_players3)
        {
        case 1:
        oggetto_assegnato1_3:
            printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
            {
                for (i = 0; i < n_players; i++)
                {
                    if (oggetti_zaino[i] == oggetto_selezionato)
                    {
                        giocatore1->zaino[0] = oggetto_selezionato;
                        printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                    }
                }
            }
            else
            {
                printf("\x1b[31m selezionare uno degli oggetti generati 2 \x1b[37m\n");
                goto oggetto_assegnato1_3;
            }

            switch (n_players2)
            {
            case 1:
            oggetto_assegnato3_3_1_2:
                printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore1->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore2->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori 2 \x1b[37m\n");
                        goto oggetto_assegnato3_3_1_2;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_3_1_2;
                }
            oggetto_assegnato3_1_2_3:
                printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore3->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_1_2_3;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_1_2_3;
                }
                break;
            case 2:
            oggetto_assegnato3_1_3:
                printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore1->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore3->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_1_3;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_1_3;
                }

            oggetto_assegnato3_1_3_2:
                printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore3->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore2->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_1_3_2;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_1_3_2;
                }
                break;
            default:
                printf("\x1b[31m errore \x1b[37m\n");
                break;
            }
            break;
        case 2:
        oggetto_assegnato3_2:
            printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
            {
                for (i = 0; i < n_players; ++i)
                {
                    if (oggetti_zaino[i] == oggetto_selezionato)
                    {
                        giocatore2->zaino[0] = oggetto_selezionato;
                        printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                    }
                }
            }
            else
            {
                printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                goto oggetto_assegnato3_2;
            }

            switch (n_players2)
            {
            oggetto_assegnato3_3_3_2_1:
            case 1:
                printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore2->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore1->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_3_3_2_1;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_3_3_2_1;
                }
            oggetto_assegnato3_2_1_3:
                printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore3->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_2_1_3;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_2_1_3;
                }
                break;
            case 2:
            oggetto_assegnato3_2_3:
                printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore2->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore3->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_2_3;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_2_3;
                }
            oggetto_assegnato3_2_3_1:
                printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore1->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_2_3_1;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_2_3_1;
                }

                break;
            default:
                printf("\x1b[31m errore \x1b[37m\n");
                break;
            }
            break;
        case 3:
        oggetto_assegnato3_3:
            printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
            {
                for (i = 0; i < n_players; ++i)
                {
                    if (oggetti_zaino[i] == oggetto_selezionato)
                    {
                        giocatore3->zaino[0] = oggetto_selezionato;
                        printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                    }
                }
            }
            else
            {
                printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                goto oggetto_assegnato3_3;
            }
            switch (n_players2)
            {
            case 1:
            oggetto_assegnato3_3_2:
                printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore3->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore2->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_3_2;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_3_2;
                }
            oggetto_assegnato3_3_2_1:
                printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore1->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_3_2_1;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_3_2_1;
                }
                break;
            case 2:
            oggetto_assegnato3_3_1:
                printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore3->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore1->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_3_1;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_3_1;
                }
            oggetto_assegnato3_3_3_1_2:
                printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore1->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore2->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_3_3_1_2;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_3_3_1_2;
                }
                break;
            default:
                printf("\x1b[31m errore \x1b[37m\n");
                break;
            }
            break;
        default:
            printf("\x1b[31m errore \x1b[37m\n");
            break;
        }
        break;
        // giocatori 4
    case 4:
        switch (n_players4)
        {

        case 1:
        oggetto_assegnato1_4:
            printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
            {
                for (i = 0; i < n_players; i++)
                {
                    if (oggetti_zaino[i] == oggetto_selezionato)
                    {
                        giocatore1->zaino[0] = oggetto_selezionato;
                        printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                    }
                }
            }
            else
            {
                printf("\x1b[31m selezionare uno degli oggetti generati 2 \x1b[37m\n");
                goto oggetto_assegnato1_4;
            }
            switch (n_players3)
            {
            case 1:
            oggetto_assegnato1_2_2_4:
                printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore1->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore2->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato1_2_2_4;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato1_2_2_4;
                }
                switch (n_players2)
                {
                case 1: // giocatore 3 e 4
                oggetto_assegnato1_2_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_2_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_2_3;
                    }
                oggetto_assegnato1_2_3_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore3->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_2_3_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_2_3_4;
                    }
                    break;
                case 2: // giocatore 4 e 3
                oggetto_assegnato1_2_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_2_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_2_4;
                    }
                oggetto_assegnato1_2_4_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_2_4_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_2_4_3;
                    }
                    break;
                default:
                    printf("\x1b[31m errore\x1b[37m\n");
                    break;
                }
                break;
            case 2:
            oggetto_assegnato1_3_3_4:
                printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore1->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore3->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato1_3_3_4;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato1_3_3_4;
                }
                switch (n_players2)
                {
                case 1: // giocatore 2 e 4
                oggetto_assegnato1_3_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore3->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_3_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_3_2;
                    }
                oggetto_assegnato1_3_2_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore3->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_3_2_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_3_2_4;
                    }
                    break;
                case 2: // giocatore 4 e 2
                oggetto_assegnato1_3_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore3->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_3_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_3_4;
                    }
                oggetto_assegnato1_3_4_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_3_4_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_3_4_2;
                    }
                    break;
                default:
                    printf("\x1b[31m errore stampa mappa\x1b[37m\n");
                    break;
                }
                break;
            case 3:
            oggetto_assegnato1_4_4_4:
                printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore1->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore4->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato1_4_4_4;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato1_4_4_4;
                }
                switch (n_players2)
                {
                case 1: // giocatore 2 e 3
                oggetto_assegnato1_4_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_4_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_4_2;
                    }
                oggetto_assegnato1_4_2_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_4_2_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_4_2_3;
                    }
                    break;
                case 2: // giocatore 3 e 2
                oggetto_assegnato1_4_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_4_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_4_3;
                    }
                oggetto_assegnato1_4_3_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato1_4_3_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato1_4_3_2;
                    }
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
                break;
            default:
                printf("\x1b[31m errore \x1b[37m\n");
                break;
            }
            break;
        case 2:
        oggetto_assegnato2_4:
            printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
            {
                for (i = 0; i < n_players; i++)
                {
                    if (oggetti_zaino[i] == oggetto_selezionato)
                    {
                        giocatore2->zaino[0] = oggetto_selezionato;
                        printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                    }
                }
            }
            else
            {
                printf("\x1b[31m selezionare uno degli oggetti generati 2 \x1b[37m\n");
                goto oggetto_assegnato2_4;
            }
            switch (n_players3)
            {
            case 1:
            oggetto_assenato2_1_4:
                printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore2->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore1->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato2_1_4;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato2_1_4;
                }
                switch (n_players2)
                {
                case 1: // giocatore 3 e 4
                oggetto_assegnato2_1_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_1_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_1_3;
                    }
                oggetto_assegnato2_1_3_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_1_3_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_1_3_4;
                    }
                    break;
                case 2: // giocatore 4 e 3
                oggetto_assegnato2_1_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_1_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_1_4;
                    }
                oggetto_assegnato2_1_4_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_1_4_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_1_4_3;
                    }
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                }
                break;
            case 2:
            oggetto_assegnato2_3_3_4:
                printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore2->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore3->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato2_3_3_4;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato2_3_3_4;
                }
                switch (n_players2)
                {
                case 1: // giocatore 1 e 4
                oggetto_assegnato2_3_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_3_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_3_1;
                    }
                oggetto_assegnato2_3_1_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore3->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_3_1_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_3_1_4;
                    }
                    break;
                case 2: // giocatore 4 e 1
                oggetto_assegnato2_3_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore3->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_3_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_3_4;
                    }
                oggetto_assegnato2_3_4_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_3_4_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_3_4_1;
                    }
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
                break;
            case 3:
            oggetto_assegnato4_2_4:
                printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore2->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore4->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato4_2_4;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato4_2_4;
                }
                switch (n_players2)
                {
                case 1: // giocatore 1 e 3
                oggetto_assegnato2_4_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_4_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_4_1;
                    }
                oggetto_assegnato2_4_1_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_4_1_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_4_1_3;
                    }
                    break;
                case 2: // giocatore 3 e 1
                oggetto_assegnato2_4_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_4_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_4_3;
                    }
                oggetto_assegnato2_4_3_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato2_4_3_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato2_4_3_1;
                    }
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
                break;
            default:
                printf("\x1b[31m errore \x1b[37m\n");
                break;
            }
            break;
        case 3:
        oggetto_assegnato3_4:
            printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
            {
                for (i = 0; i < n_players; i++)
                {
                    if (oggetti_zaino[i] == oggetto_selezionato)
                    {
                        giocatore3->zaino[0] = oggetto_selezionato;
                        printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                    }
                }
            }
            else
            {
                printf("\x1b[31m selezionare uno degli oggetti generati 2 \x1b[37m\n");
                goto oggetto_assegnato3_4;
            }
            switch (n_players3)
            {
            case 1:
            oggetto_assegnato3_3_1_4:
                printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore3->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore1->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_3_1_4;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_3_1_4;
                }
                switch (n_players2)
                {
                case 1: // giocatore 2 e 4
                oggetto_assegnato3_1_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore1->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_1_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_1_2;
                    }
                oggetto_assegnato3_1_2_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_1_2_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_1_2_4;
                    }
                    break;
                case 2: // giocatore 4 e 2
                oggetto_assegnato3_1_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore1->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_1_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_1_4;
                    }
                oggetto_assegnato3_1_4_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_1_4_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_1_4_2;
                    }
                    break;
                default:
                    printf("\x1b[31m errore\x1b[37m\n");
                    break;
                }
                break;
            case 2:
            oggetto_assegnato3_3_2_4:
                printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore3->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore2->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_3_2_4;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_3_2_4;
                }
                switch (n_players2)
                {
                case 1: // giocatore 1 e 4
                oggetto_assegnato3_2_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_2_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_2_1;
                    }
                oggetto_assegnato3_2_1_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore1->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_2_1_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_2_1_4;
                    }
                    break;
                case 2: // giocatore 4 e 1
                oggetto_assegnato3_2_4:
                    printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore4->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_2_4;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_2_4;
                    }
                oggetto_assegnato3_2_4_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_2_4_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_2_4_1;
                    }
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
                break;
            case 3:
            oggetto_assegnato3_4_3:
                printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore3->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore4->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato3_4_3;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato3_4_3;
                }
                switch (n_players2)
                {
                case 1: // giocatore 1 e 2
                oggetto_assegnato3_4_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_4_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_4_1;
                    }
                oggetto_assegnato3_4_1_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore1->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_4_1_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_4_1_2;
                    }
                    break;
                case 2: // giocatore 2 e 1
                oggetto_assegnato3_4_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_4_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_4_2;
                    }
                oggetto_assegnato3_4_2_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato3_4_2_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato3_4_2_1;
                    }
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
                break;
            default:
                printf("\x1b[31m errore \x1b[37m\n");
                break;
            }
            break;
        case 4:
        oggetto_assegnato4_4:
            printf("Il giocatore \x1b[34m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore4->nickname);
            scanf("%d", &oggetto_selezionato);
            pulizia_buffer();
            if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
            {
                for (i = 0; i < n_players; i++)
                {
                    if (oggetti_zaino[i] == oggetto_selezionato)
                    {
                        giocatore4->zaino[0] = oggetto_selezionato;
                        printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore4->zaino[0]);
                    }
                }
            }
            else
            {
                printf("\x1b[31m selezionare uno degli oggetti generati 2 \x1b[37m\n");
                goto oggetto_assegnato4_4;
            }
            switch (n_players3) // n_players3
            {
            case 1:
            oggetto_assegnato4_1:
                printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore4->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore1->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato4_1;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato4_1;
                }
                switch (n_players2)
                {
                case 1: // giocatore 2 e 3
                oggetto_assegnato4_1_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_1_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_1_2;
                    }
                oggetto_assegnato4_1_2_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_1_2_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_1_2_3;
                    }
                    break;
                case 2: // giocatore 3 e 2
                oggetto_assegnato4_1_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_1_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_1_3;
                    }
                oggetto_assegnato4_1_3_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore3->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_1_3_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_1_3_2;
                    }
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
                break;
            case 2:
            oggetto_assegnato4_3:
                printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore4->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore3->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato4_3;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato4_3;
                }
                switch (n_players2)
                {
                case 1: // giocatore 2 e 1
                oggetto_assegnato4_3_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_3_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_3_2;
                    }
                oggetto_assegnato4_3_2_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_3_2_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_3_2_1;
                    }
                    break;
                case 2: // giocatore 1 e 2
                oggetto_assegnato4_3_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_3_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_3_1;
                    }
                oggetto_assegnato4_3_1_2:
                    printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore1->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore2->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_3_1_2;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_3_1_2;
                    }
                    break;
                default:
                    printf("\x1b[31m errore  \x1b[37m\n");
                    break;
                }
                break;
            case 3:
            oggetto_assegnato4_2:
                printf("Il giocatore \x1b[32m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore2->nickname);
                scanf("%d", &oggetto_selezionato);
                pulizia_buffer();
                if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                {
                    if (oggetto_selezionato != giocatore4->zaino[0])
                    {
                        for (i = 0; i < n_players; ++i)
                        {
                            if (oggetti_zaino[i] == oggetto_selezionato)
                            {
                                giocatore2->zaino[0] = oggetto_selezionato;
                                printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore2->zaino[0]);
                            }
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                        goto oggetto_assegnato4_2;
                    }
                }
                else
                {
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    goto oggetto_assegnato4_2;
                }
                switch (n_players2)
                {
                case 1: // giocatore 1 e 3
                oggetto_assegnato4_2_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_2_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_2_1;
                    }
                oggetto_assegnato4_2_1_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore1->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_2_1_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_2_1_3;
                    }
                    break;
                case 2: // giocatore 3 e 1
                oggetto_assegnato4_2_3:
                    printf("Il giocatore \x1b[35m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore3->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore2->zaino[0] && oggetto_selezionato != giocatore4->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore3->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore3->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_2_3;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_2_3;
                    }
                oggetto_assegnato4_2_3_1:
                    printf("Il giocatore \x1b[33m%s\x1b[37m selezioni l'oggetto tra quelli generati: ", giocatore1->nickname);
                    scanf("%d", &oggetto_selezionato);
                    pulizia_buffer();
                    if (oggetto_selezionato == 0 || oggetto_selezionato == 1 || oggetto_selezionato == 2 || oggetto_selezionato == 3 || oggetto_selezionato == 4)
                    {
                        if (oggetto_selezionato != giocatore3->zaino[0] && oggetto_selezionato != giocatore4->zaino[0] && oggetto_selezionato != giocatore2->zaino[0])
                        {
                            for (i = 0; i < n_players; ++i)
                            {
                                if (oggetti_zaino[i] == oggetto_selezionato)
                                {
                                    giocatore1->zaino[0] = oggetto_selezionato;
                                    printf("l'oggetto che a scelto il giocatore è: %d\n", giocatore1->zaino[0]);
                                }
                            }
                        }
                        else
                        {
                            printf("\x1b[31m errore, devi scegliere l'oggetto generato o diverso dagli altri giocatori \x1b[37m\n");
                            goto oggetto_assegnato4_2_3_1;
                        }
                    }
                    else
                    {
                        printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                        goto oggetto_assegnato4_2_3_1;
                    }
                    break;
                default:
                    printf("\x1b[31m errore, devi scegliere l'oggetto generato \x1b[37m\n");
                    break;
                }
                break;
            default:
                printf("\x1b[31m errore \x1b[37m\n");
                break;
            }
            break;
        default:
            printf("\x1b[31m errore \x1b[37m\n");
            break;
        }
        break;
    default:
        printf("\x1b[31m errore \x1b[37m\n");
        break;
    }
    Mappa();
}
void inserisci_zona(z) // questo metodo, mi permette di inserire le zone, e di generare per ogli zona creata, la probabilità di trovare un'oggetto
{
    time_t t;
    srand((unsigned)time(&t));
    node = (struct Zona_Mappa *)malloc(sizeof(struct Zona_Mappa)); // mi permette di inserire le zone come pfirst
    random_oggetto = (rand() % 4);
    node->oggetto_zona = random_oggetto;
    if (pLast == NULL)
    {
        node->zona = z;
        node->prossima_zona = node;
        pLast = node;
    }
    else
    {
        node->zona = z;
        node->prossima_zona = pLast->prossima_zona;
        pLast->prossima_zona = node;
    }
}
void stampa_mappa()
{
    int i = 1;
    if (pLast == NULL)
    {
        printf("non sono presenti zone\n");
    }
    else
    {
        /*struct Zona_Mappa *caravan;
        caravan = (struct Zona_Mappa *)malloc(sizeof(struct Zona_Mappa));
        caravan->zona = 0;*/

        node = pLast->prossima_zona; // mi permette di stampare le zone della mappa, imbase al loro enum
        do
        {

            switch (node->zona)
            {
            case 0:
                printf("zona %d cucina\n", i);
                break;
            case 1:
                printf("zona %d soggiorno\n", i);
                break;
            case 2:
                printf("zona %d camera\n", i);

                break;
            case 3:
                printf("zona %d bagno\n", i);

                break;
            case 4:
                printf("zona %d garage\n", i);

                break;
            case 5:
                printf("zona %d seminterrato\n", i);
                break;
            default:
                printf("\x1b[31m errore stampa mappa\x1b[37m\n");
            }

            i++;
            node = node->prossima_zona;
        } while (node != pLast->prossima_zona);
    }
}

void delete_zona() // mi permette di cancellare una zona della mappa, più precisamente la prima zona inserita
{
    node = pLast->prossima_zona;
    while (node->prossima_zona != pLast)
    {
        node = node->prossima_zona;
    }
    node->prossima_zona = pLast->prossima_zona;
    pLast = node;
}
void Mappa() // menu di mappa
{
    do
    {
        time_t t;
        srand((unsigned)time(&t));
        printf("\n***********************************\x1b[36m Crea Mappa \x1b[37m*******************************************\n");
        printf("1: inserisci zona\n");
        printf("2: cancella zona\n");
        printf("3: stampa mappa\n");
        printf("4: chiudi mappa\n");
        printf("******************************************************************************************\n");
        printf("digitare il codice per impostare la mappa: ");
        scanf("%d", &value_mappa);
        pulizia_buffer();
        if (!(value_mappa == 1 || value_mappa == 2 || value_mappa == 3 || value_mappa == 4))
        {
            printf("\x1b[31m errore nell'inserimento, ripetere\x1b[37m\n");
        }
        else
        {
            if (zone == 0)
            {
                setmappa = false;
            }
            switch (value_mappa)
            {
            case 1:
                if (zone >= 7)
                {
                    printf("hai raggiunto il massimo di zone\n");
                }
                else // incremento di 1 il numero di zone,generando a caso la stanza(enum) della nuova zona
                {
                    zone++;
                    printf("zona creata:%d\n", zone);
                    zone_mappa = (rand() % 6);
                    inserisci_zona(zone_mappa);
                    setmappa = true;
                    break;
                case 2:
                    if (setmappa == true)
                    {
                        if (zone == 1)
                        {
                            printf("non puoi eliminare la zona, è la prima zona creata\n");
                        }
                        else // cancello le zone
                        {
                            zone--;
                            delete_zona();
                            printf("zona rimanenti:%d\n", zone);
                        }
                    }
                    else
                    {
                        printf("prima di eliminare la zona, devi almeno crearla una\n");
                    }

                    break;
                case 3:
                    if (setmappa == true)
                    {
                        stampa_mappa();
                    }
                    else
                    {
                        printf("prima di stampare la mappa devi almeno creare una zona\n");
                    }

                    break;
                case 4: // posso salvare la mappa solo se le zone create sono 3 o maggiori
                    if (zone >= 3)
                    {
                        printf("mappa salvata\n");
                        mappa_valida = true;
                    }
                    else
                    {
                        printf("prima di stampare la mappa devi almeno creare tre zone\n");
                    }
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        }
    } while (mappa_valida != true);
    imposta_gioco();
}
void diminuzione_sanità(struct Giocatore *g) // metodo per la diminuzione della sanità mentale
{
    time_t t;
    srand((unsigned)time(&t));
    int sanità = (rand() % 100) + 1; // per ogni turno che fa un giocatore c'è la possibilità del 20% che gli venga decrementata la sanità mentale, imbase alla variabile sanità che se è minore o uguale a 20, diminuisce la sanità di 15 punti
    if (sanità <= 20)
    {
        g->sanità_mentale -= 15;
        printf("decremento in corso\n");
    }
}
void apparizionefantasma(struct Giocatore *g) // metodo aparizione fantasma, nel raccogliere le prove
{
    time_t t;
    srand((unsigned)time(&t));
    int random_fantasma = (rand() % 100) + 1; // a diversi livelli di apparizione
    if (random_fantasma <= apparizione_fantasma)
    {
        if (uso_sale == true) // se il giocatore ha il sale come oggetto e l'ho usa quando compare il fantasma non prnede danno
        {
            g->sanità_mentale -= 0;
            printf("hai utilizzato il sale, quindi la tua sanità mentale non è decrementata\n");
        }
        else
        {
            g->sanità_mentale -= decremento_sanità_mentale;
            printf("decremento in corso, apparizione fantasma\n");
            stop_tornacaravan = true;
            apparizione_fantasma += 10; // incremento l'apparizione del fantasma del 10% ogni volta che raccoglie una prova
        }
    }
}
void torna_caravan(struct Giocatore *g) // mi permette di marcare(lasciare), le prove trovate se trovo tutte le prove i giocatori hanno vinto la partita
{
    if (stop_tornacaravan == true)
    {
        printf("non puoi tornare al caravan\n");
    }
    else // lascio la prova marcata, e gli assegna l'oggetto in maniera sequenziale
    {
        g->posizione = pLast->prossima_zona;
        if (g->zaino[2] == 1)
        {
            printf("hai marcato la prova:spirit_box\n");
            marcato_spirit_box = true;
            if (marcato_emf == true && marcato_spirit_box == true && marcato_videocamera == true)
            {
                if (n_players == 1)
                {
                    printf("hai vinto\n");
                }
                else if (n_players <= 2)
                {
                    printf("avete vinto\n");
                }
                else
                {
                    printf("\x1b[31m errore \x1b[37m\n");
                }
                printf("partita finita\n");
                termina_gioco();
            }
            else
            {
                g->zaino[2] = 0;
                g->zaino[0] = 2;
                printf("hai ottenuto oggetto:prova_videocamera\n");
            }
        }
        else if (g->zaino[2] == 2)
        {
            printf("hai marcato la prova:prova_videocamera\n");
            marcato_videocamera = true;
            if (marcato_emf == true && marcato_spirit_box == true && marcato_videocamera == true)
            {
                if (n_players == 1)
                {
                    printf("hai vinto\n");
                }
                else if (n_players <= 2)
                {
                    printf("avete vinto\n");
                }
                else
                {
                    printf("\x1b[31m errore \x1b[37m\n");
                }
                printf("partita finita\n");
                termina_gioco();
            }
            else
            {
                g->zaino[2] = 0;
                g->zaino[0] = 0;
                printf("hai ottenuto oggetto:EMF\n");
            }
        }
        else if (g->zaino[2] == 3)
        {
            printf("hai marcato la prova:prova_EMF\n");
            marcato_emf = true;
            if (marcato_emf == true && marcato_spirit_box == true && marcato_videocamera == true)
            {
                if (n_players == 1)
                {
                    printf("hai vinto\n");
                }
                else if (n_players <= 2)
                {
                    printf("avete vinto\n");
                }
                else
                {
                    printf("\x1b[31m errore \x1b[37m\n");
                }
                printf("partita finita\n");
                termina_gioco();
            }
            else
            {
                g->zaino[2] = 0;
                g->zaino[0] = 1;
                printf("hai ottenuto oggetto:prova_spirit_box\n");
            }
        }
        else
        {
            printf("non puoi marcare nessuna prova\n");
        }
    }
}

void stampa_giocatore(struct Giocatore *g) // stampo tutti i dati del giocatore 
{
    if (g->nickname == giocatore1->nickname)
    {
        printf("il giocatore \x1b[33m%s\x1b[37m, la sua sanità mentale è %d\n", g->nickname, g->sanità_mentale);
    }
    else if (g->nickname == giocatore2->nickname)
    {
        printf("il giocatore \x1b[32m%s\x1b[37m, la sua sanità mentale è %d\n", g->nickname, g->sanità_mentale);
    }
    else if (g->nickname == giocatore3->nickname)
    {
        printf("il giocatore \x1b[35m%s\x1b[37m, la sua sanità mentale è %d\n", g->nickname, g->sanità_mentale);
    }
    else if (g->nickname == giocatore4->nickname)
    {
        printf("il giocatore \x1b[34m%s\x1b[37m, la sua sanità mentale è %d\n", g->nickname, g->sanità_mentale);
    }
    else
    {
        printf("errore colorazione\n");
    }
    switch (g->zaino[0])
    {
    case 0:
        printf("EMF\n");
        break;
    case 1:
        printf("spirit box\n");
        break;
    case 2:
        printf("Videocamera\n");
        break;
    case 3:
        printf("Calmanti\n");
        break;
    case 4:
        printf("Sale\n");
        break;
    case 6:
        printf("nessuno oggetto\n");
        break;
    default:
        printf("\x1b[31m errore \x1b[37m\n");
        break;
    }
    switch (g->zaino[1])
    {
    case 0:
        printf("oggetto: nessun oggetto\n");
        break;
    case 1:
        printf("oggetto: cento dollari\n");
        break;
    case 2:
        printf("oggetto: coltello\n");
        break;
    case 3:
        printf("oggetto: adrenalina\n");
        break;
    case 4:
        printf("oggetto: calmanti\n");
        break;
    case 5:
        printf("oggetto: sale\n");
        break;
    default:
        printf("\x1b[31m errore \x1b[37m\n");
        break;
    }
    switch (g->zaino[2])
    {
    case 0:
        printf("prova: nessuna prova\n");
        break;
    case 1:
        printf("prova: spirit box\n");
        break;
    case 2:
        printf("prova: videocamera\n");
        break;
    case 3:
        printf("prova: EMF\n");
        break;
    default:
        printf("\x1b[31m errore \x1b[37m\n");
        break;
    }
}

void stampa_zona(struct Giocatore *g) // stampo tutti i dati della zona dove si trova quel determinato giocatore
{
    switch (g->posizione->zona)
    {
    case 0:
        printf("zona cucina\n");
        break;
    case 1:
        printf("zona soggiorno\n");
        break;
    case 2:
        printf("zona camera\n");

        break;
    case 3:
        printf("zona bagno\n");

        break;
    case 4:
        printf("zona garage\n");

        break;
    case 5:
        printf("zona seminterrato\n");

        break;
    default:
        printf("\x1b[31m errore stampa mappa\x1b[37m\n");
        break;
    }
    switch (g->posizione->prova)
    {
    case 0:
        printf("prova: nessuna prova\n");
        break;
    case 1:
        printf("prova: spirit box\n");
        break;
    case 2:
        printf("prova: videocamera\n");
        break;
    case 3:
        printf("prova: EMF\n");
        break;
    default:
        printf("\x1b[31m errore \x1b[37m\n");
        break;
    }
    switch (g->posizione->oggetto_zona)
    {
    case 0:
        printf("oggetto: adrenalina\n");
        break;
    case 1:
        printf("oggetto: cento dollari\n");
        break;
    case 2:
        printf("oggetto: coltello\n");
        break;
    case 3:
        printf("oggetto: nessun oggetto\n");
        break;
    default:
        printf("\x1b[31m errore \x1b[37m\n");
        break;
    }
}
void avanza(struct Giocatore *g) // il giocatore avanza per ogni turno massimo una volta
{
    time_t t;
    srand((unsigned)time(&t));
    if (turni_avanzati == 0 || n_players == 1)
    {
        struct Zona_Mappa *node = g->posizione;
        int random_prova = (rand() % 100); // random prova, genera per ogni zona in maniera casuale la prova che si può trovare
        g->posizione = node->prossima_zona;
        printf("giocatore avanzato\n");
        if (random_prova <= 40) // se il random_prova è minore o uguale a 40, in quella zona non ci sarà nessuna prova
        {
            g->posizione->prova = 0;
        }
        else if (random_prova <= 60) // incremento di 20 in 20 quindi se il random_prova è minore o uguale a 60, troverò la prova spirit_EMF
        {
            g->posizione->prova = 3;
        }
        else if (random_prova <= 80) // incremento di 20 in 20 quindi se il random_prova è minore o uguale a 60, troverò la prova videocamera
        {
            g->posizione->prova = 2;
        }
        else if (random_prova <= 100) // incremento di 20 in 20 quindi se il random_prova è minore o uguale a 60, troverò la prova spirit_box
        {
            g->posizione->prova = 1;
        }
        else
        {
            printf("\x1b[31m errore \x1b[37m\n");
        }
    }
    else
    {
        printf("\x1b[31m non puoi avanzare 2 volte di fila \x1b[37m\n");
    }
    turni_avanzati++;
}

void raccogli_prova(struct Giocatore *g) // mi permette di raccoglie la prova se quel determinato giocatore a l'oggetto nello zaino[0] uguale a alla prova trovata, se no lancerà un errore
{
    switch (g->posizione->prova)
    {
    case 1:
        if (g->zaino[0] == 1)
        {
            g->zaino[2] = 1;
            printf("prova raccolto\n");
            apparizionefantasma(g);
        }
        else
        {
            printf("\x1b[31m errore, devi prende la prova che corrisponde all'ogetto che ha nello ziano il giocatore \x1b[37m\n");
        }
        break;
    case 2:
        if (g->zaino[0] == 2)
        {
            g->zaino[2] = 2;
            printf("prova raccolto\n");
            apparizionefantasma(g);
        }
        else
        {
            printf("\x1b[31m errore, devi prende la prova che corrisponde all'ogetto che ha nello ziano il giocatore \x1b[37m\n");
        }
        break;
    case 3:
        if (g->zaino[0] == 0)
        {
            g->zaino[2] = 3;
            printf("prova raccolto\n");
            apparizionefantasma(g);
        }
        else
        {
            printf("\x1b[31m errore, devi prende la prova che corrisponde all'ogetto che ha nello ziano il giocatore \x1b[37m\n");
        }
        break;
    default:
        printf("errore\n");
        break;
    }
}

void raccogli_oggetto(struct Giocatore *g) // raccogli oggetto, imabase all'oggetto che è stato generato im quella zona
{
    if (g->posizione->oggetto_zona != 0)
    {
        g->zaino[1] = g->posizione->oggetto_zona;
        printf("oggetto raccolto\n");
    }
    else if (g->zaino[1] == g->posizione->oggetto_zona)
    {
        printf("non puoi prendere l'oggetto\n");
    }
    else
    {
        printf("non è possibile raccogliere niente\n");
    }
}

void usa_oggetto(struct Giocatore *g)
{
    time_t t;
    srand((unsigned)time(&t));
    switch (g->zaino[0])
    {
    case 3:
        g->sanità_mentale += 40;
        printf("hai usato calmanti\n");
        g->zaino[0] = 6;
        break;
    case 4:
        uso_sale = true;
        printf("hai usato sale \n");
        g->zaino[0] = 6;
        break;
    default:
        printf("nessun oggetto da utilizzare\n");
        break;
    }
    switch (g->zaino[1])
    {
    case 1:
        random_oggettizaino = (rand() % 2);
        if (random_oggettizaino == 0)
        {
            g->zaino[1] = 4;
            printf("hai ottenuto calmante\n");
        }
        else
        {
            g->zaino[1] = 5;
            printf("hai ottenuto sale\n");
        }
        break;
    case 2:
        printf("hai usato il coltello\n");
        if (g->sanità_mentale <= 30)
        {
            if (g->nickname != giocatore1->nickname)
            {
                if (g->posizione == giocatore1->posizione)
                {
                    printf("hai ucciso \x1b[33m%s\x1b[37m", giocatore1->nickname);
                    giocatore1->sanità_mentale = 0;
                }
            }
            if (g->nickname != giocatore2->nickname)
            {
                if (g->posizione == giocatore2->posizione)
                {
                    printf("hai ucciso \x1b[32m%s\x1b[37m", giocatore2->nickname);
                    giocatore2->sanità_mentale = 0;
                }
            }
            if (g->nickname != giocatore3->nickname)
            {
                if (g->posizione == giocatore3->posizione)
                {
                    printf("hai ucciso \x1b[35m%s\x1b[37m", giocatore3->nickname);
                    giocatore3->sanità_mentale = 0;
                }
            }
            if (g->nickname != giocatore4->nickname)
            {
                if (g->posizione == giocatore4->posizione)
                {
                    printf("hai ucciso x1b[34m%s\x1b[37m", giocatore4->nickname);
                    giocatore4->sanità_mentale = 0;
                }
            }
        }
        g->zaino[1] = 0;
        break;
    case 3:
        turni_avanzati++;
        break;
    default:
        printf("nessun oggetto da utilizzare\n");
        break;
    }
}

void passa(struct Giocatore *g) // passo il turno al giocatore successivo //non funziona
{
    printf("turno passato\n");
    uso_sale = false;
    stop_tornacaravan = false;
    turni_avanzati = 0;
}
void menugioca_1nplayers()
{
    do
    {
        do
        {
            if (giocatore1->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[33m%s\x1b[37m è morto\n", giocatore1->nickname);
                termina_gioco();
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore1);
                    break;
                case 1:
                    stampa_giocatore(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 2:
                    stampa_zona(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 3:
                    avanza(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 4:
                    passa(giocatore1);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 6:
                    raccogli_oggetto(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 7:
                    usa_oggetto(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;
    } while (!(marcato_emf == true && marcato_spirit_box == true && marcato_videocamera == true));
}
void menugioca_2nplayers()
{
    do
    {
        do
        {
            if (giocatore1->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[33m%s\x1b[37m è morto\n", giocatore1->nickname);
                break;
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore1);
                    break;
                case 1:
                    stampa_giocatore(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 2:
                    stampa_zona(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 3:
                    avanza(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 4:
                    passa(giocatore1);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 6:
                    raccogli_oggetto(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 7:
                    usa_oggetto(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;

        do
        {
            if (giocatore2->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[32m%s\x1b[37m è morto\n", giocatore2->nickname);
                break;
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore2);
                    break;
                case 1:
                    stampa_giocatore(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 2:
                    stampa_zona(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 3:
                    avanza(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 4:
                    passa(giocatore2);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 6:
                    raccogli_oggetto(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 7:
                    usa_oggetto(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;
    } while (!(marcato_emf == true && marcato_spirit_box == true && marcato_videocamera == true));
}
void menugioca_3nplayers()
{
    do
    {
        do
        {
            if (giocatore1->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[33m%s\x1b[37m è morto\n", giocatore1->nickname);
                break;
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore1);
                    break;
                case 1:
                    stampa_giocatore(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 2:
                    stampa_zona(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 3:
                    avanza(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 4:
                    passa(giocatore1);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 6:
                    raccogli_oggetto(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 7:
                    usa_oggetto(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;

        do
        {
            if (giocatore2->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[32m%s\x1b[37m è morto\n", giocatore2->nickname);
                break;
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore2);
                    break;
                case 1:
                    stampa_giocatore(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 2:
                    stampa_zona(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 3:
                    avanza(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 4:
                    passa(giocatore2);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 6:
                    raccogli_oggetto(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 7:
                    usa_oggetto(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;
        do
        {
            if (giocatore3->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[35m%s\x1b[37m è morto\n", giocatore3->nickname);
                break;
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore3);
                    break;
                case 1:
                    stampa_giocatore(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 2:
                    stampa_zona(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 3:
                    avanza(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 4:
                    passa(giocatore3);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 6:
                    raccogli_oggetto(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 7:
                    usa_oggetto(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;
    } while (!(marcato_emf == true && marcato_spirit_box == true && marcato_videocamera == true));
}
void menugioca_4nplayers()
{
    do
    {
        do
        {
            if (giocatore1->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[33m%s\x1b[37m è morto\n", giocatore1->nickname);
                break;
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore1);
                    break;
                case 1:
                    stampa_giocatore(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 2:
                    stampa_zona(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 3:
                    avanza(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 4:
                    passa(giocatore1);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 6:
                    raccogli_oggetto(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                case 7:
                    usa_oggetto(giocatore1);
                    diminuzione_sanità(giocatore1);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;

        do
        {
            if (giocatore2->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[32m%s\x1b[37m è morto\n", giocatore2->nickname);
                break;
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore2);
                    break;
                case 1:
                    stampa_giocatore(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 2:
                    stampa_zona(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 3:
                    avanza(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 4:
                    passa(giocatore2);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 6:
                    raccogli_oggetto(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                case 7:
                    usa_oggetto(giocatore2);
                    diminuzione_sanità(giocatore2);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;
        do
        {
            if (giocatore3->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[35m%s\x1b[37m è morto\n", giocatore3->nickname);
                break;
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore3);
                    break;
                case 1:
                    stampa_giocatore(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 2:
                    stampa_zona(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 3:
                    avanza(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 4:
                    passa(giocatore3);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 6:
                    raccogli_oggetto(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                case 7:
                    usa_oggetto(giocatore3);
                    diminuzione_sanità(giocatore3);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;
        do
        {
            if (giocatore4->sanità_mentale <= 0)
            {
                printf("Il giocatore \x1b[34m%s\x1b[37m è morto\n", giocatore4->nickname);
                break;
            }
            else
            {
                printf("\n***********************************\x1b[35m Gioca \x1b[37m*******************************************\n");
                printf("\x1b[36m 0\x1b[37m : torna al caravan\n");
                printf("\x1b[36m 1\x1b[37m : stampa giocatore\n");
                printf("\x1b[36m 2\x1b[37m : stampa zona\n");
                printf("\x1b[36m 3\x1b[37m :  avanza_giocatori\n");
                printf("\x1b[36m 4\x1b[37m : passa turno\n");
                printf("\x1b[36m 5\x1b[37m : raccogli prova\n");
                printf("\x1b[36m 6\x1b[37m : raccogli oggetto\n");
                printf("\x1b[36m 7\x1b[37m : usa oggetto\n");
                printf("*************************************************************************************\n");
                printf("digitare un comando tra quelli elencati: ");
                do
                {
                    scanf("%d", &scelta_gioco);
                    if (!(scelta_gioco >= 0 && scelta_gioco <= 7))
                    {
                        printf("\x1b[31m errore, digitare un numero del menù gioca \x1b[37m\n");
                    }
                } while (!(scelta_gioco >= 0 && scelta_gioco <= 7));
                switch (scelta_gioco)
                {
                case 0:
                    torna_caravan(giocatore4);
                    break;
                case 1:
                    stampa_giocatore(giocatore4);
                    diminuzione_sanità(giocatore4);
                    break;
                case 2:
                    stampa_zona(giocatore4);
                    diminuzione_sanità(giocatore4);
                    break;
                case 3:
                    avanza(giocatore4);
                    diminuzione_sanità(giocatore4);
                    break;
                case 4:
                    passa(giocatore4);
                    passa_turno = true;
                    break;
                case 5:
                    raccogli_prova(giocatore4);
                    diminuzione_sanità(giocatore4);
                    break;
                case 6:
                    raccogli_oggetto(giocatore4);
                    diminuzione_sanità(giocatore4);
                    break;
                case 7:
                    usa_oggetto(giocatore4);
                    diminuzione_sanità(giocatore4);
                    break;
                default:
                    printf("\x1b[31m errore \x1b[37m\n");
                    break;
                }
            }
        } while (passa_turno == false);
        passa_turno = false;
    } while (!(marcato_emf == true && marcato_spirit_box == true && marcato_videocamera == true));
}
void gioca() // gioca, controlla se le impostazioni e il set mappa sia andato a buon fine se no mi rimanda al metdo imposta_gioco, e mette i giocatore nella prima zona della mappa;
{
    switch (n_players)
    {
    case 1:
        giocatore1->posizione = pLast->prossima_zona;
        break;
    case 2:
        giocatore1->posizione = pLast->prossima_zona;
        giocatore2->posizione = pLast->prossima_zona;
        break;
    case 3:
        giocatore1->posizione = pLast->prossima_zona;
        giocatore2->posizione = pLast->prossima_zona;
        giocatore3->posizione = pLast->prossima_zona;
        break;
    case 4:
        giocatore1->posizione = pLast->prossima_zona;
        giocatore2->posizione = pLast->prossima_zona;
        giocatore3->posizione = pLast->prossima_zona;
        giocatore4->posizione = pLast->prossima_zona;
        break;
    default:
        printf("errore\n");
        break;
    }

    if (setImpostazioni == true && mappa_valida == true)
    {

        printf("tutte le impostazioni di gioco sono state settate in maniera giusta\n");
        printf("ora puoi GIOCARE...BUONA FORTUNA\n");
        menu_gioca();
    }
    else
    {
        imposta_gioco();
    }
}
void menu_gioca()
{
    switch (n_players)
    {
    case 1:
        menugioca_1nplayers();
        break;
    case 2:
        menugioca_2nplayers();
        break;
    case 3:
        menugioca_3nplayers();
        break;
    case 4:
        menugioca_4nplayers();
        break;
    default:
        printf("\x1b[31m errore \x1b[37m\n");
        break;
    }
}
