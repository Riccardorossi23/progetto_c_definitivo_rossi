void imposta_gioco();
void gioca();
void termina_gioco();
// funzioni del gamelib
//inizio enum
enum tipo_oggetto_iniziale {EMF,spirit_box, videocamera, calmanti,sale};
enum tipo_oggetto_zona {nessun_oggetto, cento_dollari, coltello, adrenalina};
enum tipo_zona {cucina,soggiorno, camera, bagno, garage,seminterrato,caravan};
enum tipo_prova {nessuna_prova, prova_spirit_box, prova_videocamera,prova_EMF};

//inizio struct
struct Giocatore
{
    char nickname[20];
    short sanità_mentale;
    struct Zona_Mappa* posizione;
    unsigned char zaino[4];

};

struct Zona_Mappa
{
    enum tipo_zona zona;
    enum tipo_oggetto_iniziale tipo_oggetto;
    enum tipo_oggetto_zona oggetto_zona;
    enum tipo_prova prova;
    struct Zona_Mappa* prossima_zona;
};