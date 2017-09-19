#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>


enum class Figura {
    kolko, krzyzyk, brak
};

class Gra {
    public:
        void petla();
        void init_okno();
        void init_pozycje();
        void init_plansza();
        void init_tekstury();
        void draw_plansza();
        void handle_clock();
        void process_keyboard(sf::Event ev);
        void process_leftclick(sf::Event ev);
        bool kratka_click_detect(sf::Event ev, int n);
        void tura_next();
        void sprawdz_wygrana();
    protected:
        sf::RenderWindow okno;
        sf::IntRect pozycje[9];
        sf::Texture kolko;
        sf::Texture krzyzyk;
        sf::Texture bok;
        sf::Texture remis;
        sf::Texture wygrana;
        sf::Clock clock;
        std::vector<Figura> plansza;
        Figura tura;
        int win_a;
        int win_b;
        bool koniec;
        Figura zwyciezca;
        static const int width = 1200;
        static const int height = 900;
        static const int bok_tura_y = 400;  // y tury
        static const int wygr_x = 250; // relatywne x/y wygranej figury
        static const int wygr_y = 50;
        static const int popup_x = 150; // x/y popupu po grze
        static const int popup_y = 250;
        static constexpr const char* fKolko = "kolko.png";
        static constexpr const char* fKrzyzyk = "krzyzyk.png";
        static constexpr const char* fBok = "bok.png";
        static constexpr const char* fRemis = "remis.png";
        static constexpr const char* fWygrana = "wygrana.png";
};

void Gra::init_okno() {
    okno.create(sf::VideoMode(width, height), "Kolko i krzyzyk", sf::Style::Titlebar | sf::Style::Close);
}

void Gra::init_pozycje() {
    int s = sizeof(pozycje)/sizeof(pozycje[0]);  // iloœæ elementów w tablicy
    for (int n = 0; n < s; ++n) {
        pozycje[n].width = 300;
        pozycje[n].height = 300;
        pozycje[n].left = (n % 3) * 300;  // przydziela odpowiednio x na 0, 300 i 600
        int _y = n / 3;  // rzutuje z float na inta, dzielenie calkowite
        pozycje[n].top = _y * 300;
    }
    zwyciezca = Figura::brak;
    koniec = false;
    win_a = 0;
    win_a = 0;
}

void Gra::init_plansza() {
    plansza.clear();
    for (int p = 0; p < 9; ++p) {
        plansza.push_back(Figura::brak);
    }
    tura = Figura::kolko;
}

void Gra::init_tekstury() {  // mozna to zrobic ladniej ale mi sie nie chce
    kolko.loadFromFile(fKolko);
    krzyzyk.loadFromFile(fKrzyzyk);
    bok.loadFromFile(fBok);
    remis.loadFromFile(fRemis);
    wygrana.loadFromFile(fWygrana);
    kolko.setSmooth(true);
    krzyzyk.setSmooth(true);
    bok.setSmooth(true);
    remis.setSmooth(true);
    wygrana.setSmooth(true);
}
void Gra::process_keyboard(sf::Event ev) {
    if (ev.key.code == sf::Keyboard::Escape) {
        okno.close();
    }
}

void Gra::process_leftclick(sf::Event ev) {
    if (koniec) {
        return;
    }
    for(int p = 0; p < 9; ++p){
        if(kratka_click_detect(ev, p)){
            if (plansza[p] == Figura::brak) {
                plansza[p] = tura;
                tura_next();
                sprawdz_wygrana();
            }
        }
    }
}

void Gra::tura_next() {
    tura = (tura == Figura::kolko) ? Figura::krzyzyk : Figura::kolko; // ternary operator
}

bool Gra::kratka_click_detect(sf::Event ev, int  n){
    return ev.mouseButton.x >= pozycje[n].left
        && ev.mouseButton.x <= (pozycje[n].left + pozycje[n].width)
        && ev.mouseButton.y >= pozycje[n].top
        && ev.mouseButton.y <= (pozycje[n].top + pozycje[n].height);
}

void Gra::sprawdz_wygrana() {
    int int_a = 0, int_b = 0;
    if (((plansza[0] == plansza[1]) && (plansza[1] == plansza[2]) && (plansza[1] != Figura::brak))) {int_a=0; int_b=2;}  // dziala w taki sposob, ze:
    if (((plansza[3] == plansza[4]) && (plansza[4] == plansza[5]) && (plansza[4] != Figura::brak))) {int_a=3; int_b=5;}  // int_a i int_b wyznaczaja pola
    if (((plansza[6] == plansza[7]) && (plansza[7] == plansza[8]) && (plansza[7] != Figura::brak))) {int_a=6; int_b=8;}  // na ktorych trzeba zaczac
    if (((plansza[0] == plansza[3]) && (plansza[3] == plansza[6]) && (plansza[3] != Figura::brak))) {int_a=0; int_b=6;}  // aby narysowac kreche
    if (((plansza[1] == plansza[4]) && (plansza[4] == plansza[7]) && (plansza[4] != Figura::brak))) {int_a=1; int_b=7;}
    if (((plansza[2] == plansza[5]) && (plansza[5] == plansza[8]) && (plansza[5] != Figura::brak))) {int_a=2; int_b=8;}
    if (((plansza[0] == plansza[4]) && (plansza[4] == plansza[8]) && (plansza[4] != Figura::brak))) {int_a=0; int_b=8;}
    if (((plansza[2] == plansza[4]) && (plansza[4] == plansza[6]) && (plansza[4] != Figura::brak))) {int_a=2; int_b=6;}
    if (!((int_a == 0) && (int_b == 0))) { // NAND
        koniec = true;
        zwyciezca = plansza[int_a];
        win_a = int_a;
        win_b = int_b;
    } else {
        bool remis = true;
        for (int p = 0; p < 9; ++p) {
            if (plansza[p] == Figura::brak) {
                remis = false;
                break;
            }
        }
        if (remis) {
            koniec = true;
            zwyciezca = Figura::brak;
        }
    }
    if (koniec) {
        clock.restart();
    }
}

void Gra::handle_clock() {
    if (koniec) {
        sf::Time t = clock.getElapsedTime();
        int elapsed = t.asSeconds();
        if (elapsed > 2) {
            koniec = false;
            zwyciezca = Figura::brak;
            win_a = 0;
            win_b = 0;
            init_plansza();
        }
    }
}
void Gra::draw_plansza() {
    sf::Sprite boczek;
    boczek.setTexture(bok);
    boczek.setPosition(900, 0);
    okno.draw(boczek);

    sf::Sprite turka;
    turka.setTexture((tura == Figura::kolko) ? kolko : krzyzyk);
    turka.setPosition(900, bok_tura_y);
    okno.draw(turka);

    for (int p = 0; p < 9; ++p) {
        if (plansza[p] != Figura::brak) {
            sf::Sprite shape;
            shape.setTexture((plansza[p] == Figura::kolko) ? kolko : krzyzyk);
            shape.setPosition(pozycje[p].left, pozycje[p].top);
            okno.draw(shape);
        }
    }

    if (koniec && zwyciezca != Figura::brak) {
        sf::ConvexShape convex;
        int x_1 = ((win_a % 3) * 300) + 140;  // nie da sie narysowac grubej linii
        int y_1 = win_a / 3;  // trzeba rysowac poprzez rysowanie rct
        y_1 = (y_1 * 300) + 140;
        int x_2 = x_1 + 20;
        int y_2 = y_1 + 20;

        int x_3 = ((win_b % 3) * 300) + 140;
        int y_3 = win_b / 3;
        y_3 = (y_3 * 300) + 140;
        int x_4 = x_3 + 20;
        int y_4 = y_3 + 20;
        convex.setPointCount(4);

        convex.setPoint(0, sf::Vector2f(x_1, y_1));
        convex.setPoint(1, sf::Vector2f(x_2, y_2));
        convex.setPoint(2, sf::Vector2f(x_4, y_4));  // odwrocone dla poprawnego renderowania
        convex.setPoint(3, sf::Vector2f(x_3, y_3));  // w przeciwnym razie ksztalt hourglass

        convex.setFillColor(sf::Color::Red);
        okno.draw(convex);

        sf::Sprite popup;
        popup.setTexture(wygrana);
        popup.setPosition(popup_x, popup_y);
        okno.draw(popup);
        sf::Sprite shape;
        shape.setTexture((zwyciezca == Figura::kolko) ? kolko : krzyzyk);
        shape.setPosition(popup_x+wygr_x, popup_y+wygr_y);
        okno.draw(shape);
    }

    if (koniec && zwyciezca == Figura::brak) {
        sf::Sprite popup;
        popup.setTexture(remis);
        popup.setPosition(popup_x, popup_y);
        okno.draw(popup);
    }
}


void Gra::petla() {
    init_pozycje();
    init_plansza();
    init_tekstury();
    while (okno.isOpen())
        {
            sf::Event event;
            while (okno.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    okno.close();
                if (event.type == sf::Event::KeyPressed) {
                    process_keyboard(event);
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        process_leftclick(event);
                    }
                }
            }

            okno.clear();
            handle_clock();
            draw_plansza();
            okno.display();
        }
}
int main()
{
    Gra gierka;
    gierka.init_okno();
    gierka.petla();
}
