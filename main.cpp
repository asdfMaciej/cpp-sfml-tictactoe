#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>


enum class Figura {
    kolko, krzyzyk, brak
};
enum class Pokoj {
    pMenu, pGra, pAbout
};

class Gra {
    public:
        void petla();
        void init_gra();
        void init_pozycje();
        void init_plansza();
        void init_tekstury();
        void draw_plansza();
        void draw_about();
        void draw_menu();
        void handle_clock();
        void process_keyboard(sf::Event ev);
        void process_leftclick(sf::Event ev);
        void process_menu(sf::Event ev);
        void process_plansza(sf::Event ev);
        void process_about(sf::Event ev);
        bool kratka_click_detect(sf::Event ev, int n);
        void tura_next();
        void sprawdz_wygrana();
        sf::Text gen_tekst(std::string txt, int px, sf::Color kolor);
    protected:
        sf::RenderWindow okno;
        sf::IntRect pozycje[9];
        sf::Texture kolko;
        sf::Texture krzyzyk;
        sf::Texture bok;
        sf::Texture remis;
        sf::Texture wygrana;
        sf::Texture papier1;
        sf::Texture papier2;
        sf::Font czcionka;
        sf::Text graj;
        sf::Text o_grze;
        sf::Text t_about;
        sf::Text wroc;
        sf::Clock clock;
        Pokoj pokoj;
        std::vector<Figura> plansza;
        Figura tura;
        int win_a;
        int win_b;
        bool koniec;
        Figura zwyciezca;
        static const int FPS = 60;
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
        static constexpr const char* fPapier1 = "papier1.png";
        static constexpr const char* fPapier2 = "papier2.png";
};

void Gra::init_gra() {
    okno.create(sf::VideoMode(width, height), "Kolko i krzyzyk", sf::Style::Titlebar | sf::Style::Close);
    okno.setFramerateLimit(FPS);
    pokoj = Pokoj::pMenu;
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

void Gra::init_tekstury() {  // mozna to zrobic ladniej
    kolko.loadFromFile(fKolko);
    krzyzyk.loadFromFile(fKrzyzyk);
    bok.loadFromFile(fBok);
    remis.loadFromFile(fRemis);
    wygrana.loadFromFile(fWygrana);
    papier1.loadFromFile(fPapier1);
    papier2.loadFromFile(fPapier2);
    kolko.setSmooth(true);
    krzyzyk.setSmooth(true);
    bok.setSmooth(true);
    remis.setSmooth(true);
    wygrana.setSmooth(true);
    papier1.setSmooth(true);
    papier2.setSmooth(true);
    czcionka.loadFromFile("arial.ttf");

    graj = gen_tekst("Graj!", 150, sf::Color::Red);
    graj.setStyle(sf::Text::Bold);

    o_grze = gen_tekst("O grze", 150, sf::Color::Red);
    o_grze.setStyle(sf::Text::Bold);

    t_about = gen_tekst("Gra stworzona przez Macieja Kaszkowiaka\n na przelomie kilku godzin.\nWykonano w C++ oraz SFML", 50, sf::Color::Blue);
    wroc = gen_tekst("Wroc", 100, sf::Color::Red);
    wroc.setStyle(sf::Text::Bold);

}
void Gra::process_keyboard(sf::Event ev) {
    if (ev.key.code == sf::Keyboard::Escape) {
        if (pokoj == Pokoj::pAbout || pokoj == Pokoj::pGra) {
            pokoj = Pokoj::pMenu;
        } else {
            okno.close();
        }
    }
}

void Gra::process_leftclick(sf::Event ev) {
    if (pokoj == Pokoj::pMenu)
        process_menu(ev);
    else if (pokoj == Pokoj::pGra)
        process_plansza(ev);
    else if (pokoj == Pokoj::pAbout)
        process_about(ev);
}

void Gra::process_plansza(sf::Event ev) {
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

void Gra::process_menu(sf::Event ev) {
    float x = ev.mouseButton.x;
    float y = ev.mouseButton.y;
    if (graj.getGlobalBounds().contains(x, y))
        pokoj = Pokoj::pGra;
    if (o_grze.getGlobalBounds().contains(x, y))
        pokoj = Pokoj::pAbout;
}

void Gra::process_about(sf::Event ev) {
    float x = ev.mouseButton.x;
    float y = ev.mouseButton.y;
    if (wroc.getGlobalBounds().contains(x, y))
        pokoj = Pokoj::pMenu;
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
sf::Text Gra::gen_tekst(std::string txt, int px, sf::Color kolor) {
    sf::Text generated;
    generated.setString(txt);
    generated.setColor(kolor);
    generated.setCharacterSize(px);
    generated.setFont(czcionka);
    return generated;
}

void Gra::draw_about() {
    float x, y;
    sf::Sprite tlo;
    tlo.setTexture(papier2);
    okno.draw(tlo);

    wroc.setColor(sf::Color::Red);
    wroc.setPosition(50, 750);
    t_about.setPosition(50, 100);
    sf::Vector2i pos = sf::Mouse::getPosition(okno);
    x = pos.x;
    y = pos.y;

    if (wroc.getGlobalBounds().contains(x, y))
        wroc.setColor(sf::Color::Blue);
    okno.draw(wroc);
    okno.draw(t_about);

}
void Gra::draw_menu() {
    float x, y;
    sf::Sprite tlo;
    tlo.setTexture(papier2);
    okno.draw(tlo);

    graj.setPosition(400, 200);
    graj.setColor(sf::Color::Red);
    o_grze.setPosition(400, 500);
    o_grze.setColor(sf::Color::Red);
    sf::Vector2i pos = sf::Mouse::getPosition(okno);
    x = pos.x;
    y = pos.y;
    if (graj.getGlobalBounds().contains(x, y))
        graj.setColor(sf::Color::Blue);
    if (o_grze.getGlobalBounds().contains(x, y))
        o_grze.setColor(sf::Color::Blue);
    okno.draw(graj);
    okno.draw(o_grze);


}
void Gra::draw_plansza() {
    sf::Sprite tlo;
    tlo.setTexture(papier1);
    okno.draw(tlo);

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
            if (pokoj == Pokoj::pGra)  // nie mozna uzywac switch z enumami
                draw_plansza();
            else if (pokoj == Pokoj::pMenu)
                draw_menu();
            else if (pokoj == Pokoj::pAbout)
                draw_about();
            okno.display();
        }
}
int main()
{
    Gra gierka;
    gierka.init_gra();
    gierka.petla();
}
