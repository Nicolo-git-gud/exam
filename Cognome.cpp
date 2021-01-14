#include "contact.h"

float dist(Gprmc r1, Gprmc r2)
{
    float d = (r1.lat - r2.lat) * (r1.lat - r2.lat) + (r1.lon - r2.lon) * (r1.lon - r2.lon);
    return d;
}
void leggiGps(std::string nomefile, ContactList &t)
{
    std::ifstream f;
    f.open(nomefile, std::ifstream::in);

    if (!f.good())
        throw std::string("Non posso aprire il file dati");

    // contenitore temporaneo per leggere un record
    Gprmc tmp;

    t = nullptr;
    ContactList current;
    while (readGpsRecord(f, tmp))
    {
        if (t == nullptr)
        {
            Cell *aux = new Cell;
            aux->info = tmp;
            aux->next = t;
            t = aux;
            current = aux;
        }
        else
        {
            Cell *aux = new Cell;
            aux->info = tmp;
            aux->next = nullptr;
            current->next = aux;
            current = current->next;
        }
    }
    f.close();
}

void scriviGps(ContactList &t)
{
    ContactList scorrimento = t;
    while (scorrimento != nullptr)
    {
        writeGpsRecord(scorrimento->info);
        scorrimento = scorrimento->next;
    }
}

void convDateTime(Gprmc &r)
{
    r.gg = r.date / 10000;
    r.mm = r.date / 100 - r.gg * 100;
    r.aaaa = 2000 + r.date - r.gg * 10000 - r.mm * 100;
    r.h = r.time / 10000;
    r.m = r.time / 100 - r.h * 100;
    r.s = r.time - r.h * 10000 - r.m * 100;
}

int compare(Gprmc r1, Gprmc r2)
{
    return 0;
}

void sort(ContactList &c)
{
    if (c == nullptr)
        return;
    Cell *scorrimento = c;
    while (scorrimento->next != nullptr)
    {
        Cell *inizio = scorrimento;
        Cell *min = scorrimento;
        Cell *s2 = scorrimento->next;
        while (s2 != nullptr)
        {
            if (dist(inizio->info, min->info) < (dist(inizio->info, s2->info)))
                min = s2;
            s2 = s2->next;
        }
        Gprmc temp = scorrimento->info;
        scorrimento->info = min->info;
        min->info = temp;
        scorrimento = scorrimento->next;
    }
}

void baricentro(ContactList c, float &latB, float &lonB)
{
    ContactList scorrimento = c;
    float sum_lat = 0;
    float sum_lon = 0;
    int celle = 0;
    while (scorrimento != nullptr)
    {
        sum_lat += scorrimento->info.lat;
        sum_lon += scorrimento->info.lon;
        celle++;
        scorrimento = scorrimento->next;
    }
    latB = sum_lat / celle;
    lonB = sum_lon / celle;
}
