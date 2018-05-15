
#include <sstream>
#include "SplashState.hpp"
#include "FichaTrampa.hpp"
#include "DEFINITIONS.hpp"
#include "Trampa.hpp"
#include "Attack.hpp"
#include "HUD.hpp"
#include "Hero.hpp"
#include "Healer.hpp"
#include "Ice.hpp"
#include "Nuke.hpp"
#include "Adrenalin.hpp"
#include "Fighter.hpp"
#include "GameOverState.hpp"
#include "PlaneReturnScene.hpp"
#include <string>
#include <iostream>

namespace Zenon {

    SplashState::SplashState(GameDataRef data, std::vector<FichaTrampa*> l_fichaTrampa) : m_data(data) {
        m_trampasSel = l_fichaTrampa;
        m_disponible = m_data->data.GetMoney();
    }

    void SplashState::Init() {
        std::cout << "Tienes " << m_trampasSel.size() << " trampas" << std::endl;

        const char * titulo = "MapicaNuevo.tmx";
        map = new Maps(m_data, titulo);

        m_mouseCoordinates.setFont(this->m_data->assets.GetFont("FUENTE_DINERO"));
        m_mouseCoordinates.setString(std::to_string(sf::Mouse::getPosition(this->m_data->window).x) + ", " + std::to_string(sf::Mouse::getPosition(this->m_data->window).y));
        m_mouseCoordinates.setOrigin(m_mouseCoordinates.getGlobalBounds().width / 2, m_mouseCoordinates.getGlobalBounds().height / 2);
        m_mouseCoordinates.setPosition(sf::Mouse::getPosition(this->m_data->window).x, sf::Mouse::getPosition(this->m_data->window).y - 10);

        m_loc_placers = map->GetPlacerLocation();
        //Creamos los placer en base a nuestro mapa de polla
        for (int x = 0; x < m_loc_placers.size(); x++) {
            Placer* plaser = new Placer(m_data, m_loc_placers.at(x));
            m_placer.push_back(plaser);
        }

        m_trampa = -1;
        m_posicion.x = m_data->window.getSize().x / 2;
        m_posicion.y = m_data->window.getSize().y / 2;

        Maps &mapref = *map;
        for (int i = 0; i < m_data->data.NumberOfHeroes(); i++) {
            if (m_data->data.IsHeroeAlive(i)) {
                const std::vector<Hero*>& l_allies = m_heroes;
                const std::vector<Enemy*>& l_enemies = m_enemies;
                Hero * c_hero;
                switch (i) {
                    case 0:
                        c_hero = new Fighter(m_data, mapref, i, l_enemies);
                        m_heroes.push_back(c_hero);
                        break;
                    case 1:
                        c_hero = new Healer(m_data, mapref, i, l_allies, l_enemies);
                        m_heroes.push_back(c_hero);
                        break;
                }

            }

        }

        m_hud = new HUD(m_data, m_trampasSel);
        m_noCompruebes = false;

        m_mouseConstruct.setTexture(m_data->assets.GetTexture("CURSOR_BUILD"));
        m_mouseConstruct.setOrigin(m_mouseConstruct.getGlobalBounds().width / 2, m_mouseConstruct.getGlobalBounds().height / 2);
        m_mouseConstruct.scale(0.7, 0.7);

        LoadPaths();
        LoadAssets();


    }

    void SplashState::HandleInput() {
        sf::Event event;

        while (this->m_data->window.pollEvent(event)) {
            if (sf::Event::Closed == event.type || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                this->m_data->data.SaveChanges();
                this->m_data->window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (m_hud->CheckClick()) {
                    m_trampa = m_hud->GetClick();
                }

                if (m_trampa> -1 && m_trampasSel.at(m_trampa)->GetTipo() == 6) {

                    std::cout << "feo mierda" << std::endl;
                    const std::vector<Enemy*> &enes = m_enemies;



                    Trampa* tramp = new Ice(m_data, m_posicion, m_trampasSel.at(m_trampa)->GetTexturaPosicion(), enes);
                    m_trampas.push_back(tramp);
                }

                if (m_trampa> -1 && m_trampasSel.at(m_trampa)->GetTipo() == 7) {

                    sf::Vector2f m_posicion;
                    const std::vector<Enemy*> &enes = m_enemies;


                    if (m_trampasSel.at(m_trampa)->IsClicked()) {
                        std::cout << "feo mierda2" << std::endl;

                        m_posicion = (sf::Vector2f)m_data->input.GetMousePosition(m_data->window);

                        Trampa* tramp1 = new Nuke(m_data, m_posicion, m_trampasSel.at(m_trampa)->GetTexturaPosicion(), enes);
                        m_trampas.push_back(tramp1);
                        //crear aqui la mininuke con las posiciones pilladas arriba
                    }
                }


                for (int i = 0; i < m_placer.size(); i++) {
                    if (m_placer.at(i)->Clicked(m_trampa)) {
                        if (m_trampa> -1) {
                            if (m_trampasSel.at(m_trampa)->GetTipo() == 1) {
                                if (m_trampasSel.at(m_trampa)->Afordable(m_disponible)) {
                                    const std::vector<Enemy*> &enes = m_enemies;
                                    const std::vector<Bala*> &bull = m_bullets;
                                    Trampa* tramp = new Attack(m_data, m_placer.at(i)->GetPosicion(), m_trampasSel.at(m_trampa)->GetTexturaPosicion(), enes, m_bullets);
                                    tramp->SetAttributes(m_trampas.size(), m_trampasSel[m_trampa]->GetCadencia(), m_trampasSel[m_trampa]->GetAparicion(), m_trampasSel[m_trampa]->GetPotencia(), m_trampasSel[m_trampa]->GetRango(), m_trampasSel[m_trampa]->GetRefresco(), 0, m_trampasSel[m_trampa]->GetPorcentaje(), m_trampasSel[m_trampa]->GetPrecio());
                                    m_trampas.push_back(tramp);
                                    m_disponible -= m_trampasSel.at(m_trampa)->GetPrecio();
                                } else {
                                    m_placer.at(i)->SetOcupadoFalse();
                                }
                            } else if (m_trampasSel.at(m_trampa)->GetTipo() == 3) {
                                if (m_trampasSel.at(m_trampa)->Afordable(m_disponible)) {
                                    const std::vector<Enemy*> &enes = m_enemies;
                                    const std::vector<Bala*> &bull = m_bullets;
                                    Trampa* tramp = new Flamethrower(m_data, m_placer.at(i)->GetPosicion(), m_trampasSel.at(m_trampa)->GetTexturaPosicion(), enes, m_bullets);
                                    tramp->SetAttributes(m_trampas.size(), m_trampasSel[m_trampa]->GetCadencia(), m_trampasSel[m_trampa]->GetAparicion(), m_trampasSel[m_trampa]->GetPotencia(), m_trampasSel[m_trampa]->GetRango(), m_trampasSel[m_trampa]->GetRefresco(), 0, m_trampasSel[m_trampa]->GetPorcentaje(), m_trampasSel[m_trampa]->GetPrecio());
                                    m_trampas.push_back(tramp);
                                    m_disponible -= m_trampasSel.at(m_trampa)->GetPrecio();
                                } else {
                                    m_placer.at(i)->SetOcupadoFalse();
                                }
                            } else if (m_trampasSel.at(m_trampa)->GetTipo() == 4) {
                                if (m_trampasSel.at(m_trampa)->Afordable(m_disponible)) {
                                    const std::vector<Hero*> &heros = m_heroes;
                                    Trampa* tramp = new Support(m_data, m_placer.at(i)->GetPosicion(), m_trampasSel.at(m_trampa)->GetTexturaPosicion(), heros, RANGO_SANACION);
                                    m_trampas.push_back(tramp);
                                    m_disponible -= m_trampasSel.at(m_trampa)->GetPrecio();
                                } else {
                                    m_placer.at(i)->SetOcupadoFalse();
                                }
                            } else if (m_trampasSel.at(m_trampa)->GetTipo() == 5) {
                                if (m_trampasSel.at(m_trampa)->Afordable(m_disponible)) {
                                    const std::vector<Hero*> &heros = m_heroes;
                                    Trampa* tramp = new Adrenalin(m_data, m_placer.at(i)->GetPosicion(), m_trampasSel.at(m_trampa)->GetTexturaPosicion(), heros, RANGO_ADRENALINA);
                                    m_trampas.push_back(tramp);
                                    m_disponible -= m_trampasSel.at(m_trampa)->GetPrecio();
                                } else {
                                    m_placer.at(i)->SetOcupadoFalse();
                                }
                            }
                        }
                    }
                }

                for (int i = 0; i < m_heroes.size(); i++) {
                    if (m_heroes[i]->IsClicked()) {
                        m_heroes[i]->Select();
                    } else if (m_heroes[i]->IsSelected())
                        m_heroes[i]->Deselect();
                }
            }

        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            for (int i = 0; i < m_heroes.size(); i++) {
                if (m_heroes[i]->IsSelected()) {
                    m_heroes[i]->OrderMovement((sf::Vector2f)m_data->input.GetMousePosition(m_data->window));
                }
            }

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            for (int i = 0; i < m_heroes.size(); i++) {
                if (m_heroes[i]->IsSelected()) {
                    m_data->data.UpdateData(m_heroes[i]->GetId(), false);
                    delete m_heroes[i];
                    m_heroes.erase(m_heroes.begin() + i);
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
            m_wantsHydra = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            m_data->data.Reset();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            m_trampa = 0;
            m_hud->ProccessSelection(0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
            if (m_trampasSel.size() >= 2) {
                m_trampa = 1;
                m_hud->ProccessSelection(1);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
            if (m_trampasSel.size() >= 3) {
                m_trampa = 2;
                m_hud->ProccessSelection(2);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
            if (m_trampasSel.size() >= 4) {
                m_trampa = 3;
                m_hud->ProccessSelection(3);
            }
        }



        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
            m_wantsDoxy = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
            m_wantsBerseker = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
            m_wantsTrapper = true;
        }

        /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
            m_data->data.SetMoney(m_disponible);
            m_data->machine.AddState(StateRef(new PlaneReturnScene(m_data, true)));
        }*/
    }

    void SplashState::Update(float dt) {

        m_mouseCoordinates.setString(std::to_string(sf::Mouse::getPosition(this->m_data->window).x) + ", " + std::to_string(sf::Mouse::getPosition(this->m_data->window).y));
        m_mouseCoordinates.setOrigin(m_mouseCoordinates.getGlobalBounds().width / 2, m_mouseCoordinates.getGlobalBounds().height / 2);
        m_mouseCoordinates.setPosition(sf::Mouse::getPosition(this->m_data->window).x, sf::Mouse::getPosition(this->m_data->window).y - 20);

        /*if (m_spawnerClock.getElapsedTime().asSeconds() >= 5.0f) {
            SpawnTrapper(sf::Vector2f(300, 300));
            m_spawnerClock.restart();
        }*/

        if (m_wantsHydra) {
            SpawnHydra(m_routes[1].m_startPoint, m_routes[1]);
            m_wantsHydra = false;
        }

        if (m_wantsDoxy) {
            SpawnDoxy(sf::Vector2f(m_routes[0].m_startPoint.x, m_routes[0].m_startPoint.y), 0);
            m_wantsDoxy = false;
        }

        if (m_wantsBerseker) {
            SpawnBerseker(sf::Vector2f(100, 500), 0);
            m_wantsBerseker = false;
        }

        for (int i = 0; i < m_heroes.size(); i++) {
            m_heroes[i]->Update(dt);
            if (m_heroes[i]->GetState() == HERO_DEAD_STATE) {
                std::cout << "elimino al heore\n";
                m_data->data.UpdateData(m_heroes[i]->GetId(), false);
                delete m_heroes[i];
                m_heroes.erase(m_heroes.begin() + i);
            }
        }

        m_textoDinero.setString(std::to_string(m_disponible));
        int counter = 0;

        for (int i = 0; i < m_trampas.size(); i++) {
            if (m_trampas[i] != nullptr) {
                m_trampas.at(i)->Update(dt);
            }
        }

        for (int i = 0; i < m_trampas.size(); i++) {
            if (m_trampas[i] != nullptr) {
                if (m_trampas[i]->GetActualState() == TRAP_DYING_STATE && m_trampas[i]->getDyingClockTime() >= TRAP_DYING_TIME) {
                    FreePlacer(i);
                    delete m_trampas[i];
                    m_trampas[i] = nullptr;
                }
            }
        }

        for (int i = 0; i < m_enemies.size(); i++) {
            if (m_enemies.at(i)->GetActualState() != ENEMY_STATE_DEAD) {
                m_enemies.at(i)->Update(dt);
            }
        }

        for (int i = 0; i < m_bullets.size(); i++) {
            m_bullets[i]->Update(dt);
        }

        this->CheckColision();

        for (int i = 0; i < m_placer.size(); i++) {
            if (m_placer.at(i)->Hovered() && !m_placer.at(i)->GetOccuped()) {
                counter++;
            }
        }

        if (counter > 0)
            m_hideCursor = true;
        else
            m_hideCursor = false;

        m_mouseConstruct.setPosition((sf::Vector2f)m_data->input.GetMousePosition(m_data->window));

        //this->CheckFail();
        this->CheckDeadEnemies();

    }

    void SplashState::LoadAssets() {
        /*const std::vector<Enemy* >& l_enemies = m_enemies;

        //Objetivo 1
        m_obj = new Generator(m_data, sf::Vector2f(960, 60), l_enemies, m_routes[0].m_bRoutes[0]);
        m_objectives.push_back(m_obj);

        //Objective 2
        m_obj = new Generator(m_data, sf::Vector2f(1734, 786), l_enemies, m_routes[0].m_bRoutes[1]);
        m_objectives.push_back(m_obj);

        //Objective 3
        m_obj = new Core(m_data, sf::Vector2f(1580, 466), l_enemies, m_routes[0].m_bRoutes[2]);
        m_objectives.push_back(m_obj);*/


        m_textoDinero.setFont(m_data->assets.GetFont("FUENTE_DINERO"));
        m_textoDinero.setString(std::to_string(m_disponible));
        m_textoDinero.setCharacterSize(20);
        m_textoDinero.setOrigin(m_textoDinero.getGlobalBounds().width / 2, m_textoDinero.getGlobalBounds().height / 2);
        m_textoDinero.setPosition(900, 800);
    }

    void SplashState::LoadPaths() {


        Bezier bezier1;
        bezier1.m_startPoint = sf::Vector2f(208, 129);
        bezier1.m_endPoint = sf::Vector2f(455, 466);
        bezier1.m_controlPoint1 = sf::Vector2f(bezier1.m_startPoint.x, bezier1.m_startPoint.y + 200);
        bezier1.m_controlPoint2 = sf::Vector2f(bezier1.m_endPoint.x - 200, bezier1.m_endPoint.y);
        bezier1.m_segments = 20;
        bezier1.create();
        bezier1.AddCurve(bezier1.m_endPoint, sf::Vector2f(766, 657), sf::Vector2f(bezier1.m_endPoint.x + 200, bezier1.m_endPoint.y), sf::Vector2f(776, 500), 20);
        bezier1.AddCurve(bezier1.m_bezierBody[bezier1.m_bezierBody.size() - 1], sf::Vector2f(1218, 653),
                sf::Vector2f(bezier1.m_bezierBody[bezier1.m_bezierBody.size() - 1].x + 50, bezier1.m_bezierBody[bezier1.m_bezierBody.size() - 1].y + 150),
                sf::Vector2f(bezier1.m_bezierBody[bezier1.m_bezierBody.size() - 1].x + 300, bezier1.m_bezierBody[bezier1.m_bezierBody.size() - 1].y + 150), 20);
        bezier1.AddCurve(bezier1.m_bezierBody[bezier1.m_bezierBody.size() - 1], sf::Vector2f(1533, 459),
                sf::Vector2f(bezier1.m_bezierBody[bezier1.m_bezierBody.size() - 1].x + 100, bezier1.m_bezierBody[bezier1.m_bezierBody.size() - 1].y - 100),
                sf::Vector2f(1033, 459), 15);


        m_routes.push_back(bezier1);

        /*Bezier t_bezier;
        t_bezier.probability = 50;
        t_bezier.m_startPoint = sf::Vector2f(50, 50);
        t_bezier.m_endPoint = sf::Vector2f(725, 525);
        t_bezier.m_controlPoint1 = sf::Vector2f(500, 100);
        t_bezier.m_controlPoint2 = sf::Vector2f(500, 500);
        t_bezier.m_segments = 20;
        t_bezier.m_bPoints[100] = t_bezier.m_endPoint;
        t_bezier.create();
        t_bezier.m_bezierBody[t_bezier.m_bezierBody.size() - 2] = t_bezier.m_bezierBody[t_bezier.m_bezierBody.size() - 1];
        t_bezier.m_bPoints[10] = t_bezier.m_bezierBody[10];
        t_bezier.m_bPoints[20] = t_bezier.m_bezierBody[20];

        Bezier t_bezier2;
        t_bezier2.probability = 60;
        t_bezier2.m_startPoint = t_bezier.m_bPoints[10];
        t_bezier2.m_endPoint = sf::Vector2f(750, 100);
        t_bezier2.m_controlPoint1 = sf::Vector2f(500, 200);
        t_bezier2.m_controlPoint2 = sf::Vector2f(500, 100);
        t_bezier2.m_segments = 20;
        t_bezier2.create();
        t_bezier2.m_bezierBody[t_bezier2.m_bezierBody.size() - 2] = t_bezier2.m_bezierBody[t_bezier2.m_bezierBody.size() - 1];

        t_bezier.m_bRoutes.push_back(t_bezier2);

        Bezier t_bezier3;
        t_bezier3.probability = 30;
        t_bezier3.m_startPoint = t_bezier.m_bPoints[10];
        t_bezier3.m_endPoint = sf::Vector2f(450, 550);
        t_bezier3.m_controlPoint1 = sf::Vector2f(350, 400);
        t_bezier3.m_controlPoint2 = sf::Vector2f(300, 500);
        t_bezier3.m_segments = 20;
        t_bezier3.create();
        t_bezier3.m_bezierBody[t_bezier3.m_bezierBody.size() - 2] = t_bezier3.m_bezierBody[t_bezier3.m_bezierBody.size() - 1];

        t_bezier.m_bRoutes.push_back(t_bezier3);

        Bezier t_bezier4;
        t_bezier4.probability = 100;
        t_bezier4.m_startPoint = t_bezier.m_bPoints[20];
        t_bezier4.m_endPoint = sf::Vector2f(900, 200);
        t_bezier4.m_controlPoint1 = sf::Vector2f(1100, 400);
        t_bezier4.m_controlPoint2 = sf::Vector2f(1100, 500);
        t_bezier4.m_segments = 20;
        t_bezier4.create();
        t_bezier4.m_bezierBody[t_bezier4.m_bezierBody.size() - 2] = t_bezier4.m_bezierBody[t_bezier4.m_bezierBody.size() - 1];

        t_bezier.m_bRoutes.push_back(t_bezier4);


        m_routes.push_back(t_bezier);

        Bezier t_bezierH;
        t_bezierH.m_startPoint = sf::Vector2f(-100, 300);
        t_bezierH.m_endPoint = sf::Vector2f(2000, 300);
        t_bezierH.m_controlPoint1 = sf::Vector2f(t_bezierH.m_startPoint.x + 300, t_bezierH.m_startPoint.y - 500);
        t_bezierH.m_controlPoint2 = sf::Vector2f(t_bezierH.m_endPoint.x - 500, t_bezierH.m_endPoint.y + 800);
        t_bezierH.m_segments = 70;
        t_bezierH.create();
        t_bezierH.m_bezierBody[t_bezierH.m_bezierBody.size() - 2] = t_bezierH.m_bezierBody[t_bezierH.m_bezierBody.size() - 1];


        m_routes.push_back(t_bezierH);*/


        for (int i = 0; i < m_routes[0].m_bezierBody.size(); i++) {
            sf::CircleShape circle;
            circle.setRadius(5.0f);
            circle.setFillColor(sf::Color::Red);
            circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
            circle.setPosition(m_routes[0].m_bezierBody[i].x, m_routes[0].m_bezierBody[i].y);
            m_wps.push_back(circle);
            for (int j = 0; j < m_routes[0].m_bRoutes.size(); j++) {
                for (int k = 0; k < m_routes[0].m_bRoutes[j].m_bezierBody.size(); k++) {
                    sf::CircleShape circle;
                    circle.setRadius(5.0f);
                    circle.setFillColor(sf::Color::Red);
                    circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
                    circle.setPosition(m_routes[0].m_bRoutes[j].m_bezierBody[k].x, m_routes[0].m_bRoutes[j].m_bezierBody[k].y);
                    m_wps.push_back(circle);
                }
            }

        }

        for (int i = 0; i < m_routes[0].m_bezierBody.size(); i++) {
            sf::CircleShape circle;
            circle.setRadius(5.0f);
            circle.setFillColor(sf::Color::Magenta);
            circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
            circle.setPosition(m_routes[0].m_bezierBody[i].x, m_routes[0].m_bezierBody[i].y);
            m_wps.push_back(circle);
        }

        for (int i = 0; i < m_routes.size(); i++) {
            m_pathsVertex.push_back(ToVertex(m_routes[i].m_bezierBody));
            for (int j = 0; j < m_routes[i].m_bRoutes.size(); j++) {
                m_pathsVertex.push_back(ToVertex(m_routes[i].m_bRoutes[j].m_bezierBody));
            }
        }
    }

    void SplashState::FreePlacer(int l_trap) {
        bool stop = false;

        for (int i = 0; i < m_placer.size() && !stop; i++) {
            if (m_placer[i]->GetPosicion().x == m_trampas[l_trap]->GetPosition().x && m_placer[i]->GetPosicion().y == m_trampas[l_trap]->GetPosition().y) {
                m_placer[i]->SetOcupadoFalse();
                stop = true;
            }
        }
    }

    void SplashState::SpawnTrapper(sf::Vector2f l_position) {
        const std::vector<Trampa*>& l_traps = m_trampas;
        const std::vector<Objective*>& l_objectives = m_objectives;
        const std::vector<Enemy*>& l_enemies = m_enemies;
        Maps& l_map = *map;
        m_enemy = new Trapper(m_data, Enemy::trapper, l_position, l_map, l_enemies, l_traps, l_objectives);
        m_enemies.push_back(m_enemy);
    }

    void SplashState::SpawnDoxy(sf::Vector2f l_position, int l_path) {
        const std::vector<Enemy*>& l_neighbor = m_enemies;
        const std::vector<Bezier>& l_routes = m_routes;
        const std::vector<Objective*>& l_objectives = m_objectives;
        m_enemy = new Doxy(m_data, Enemy::doxy, l_position, l_neighbor, l_objectives, l_routes, l_path);
        m_enemies.push_back(m_enemy);
    }

    void SplashState::SpawnBerseker(sf::Vector2f l_position, int l_obj) {
        const std::vector<Enemy*>& l_neighbor = m_enemies;
        const std::vector<Hero*>& l_heroes = m_heroes;
        Maps &l_map = *map;
        m_enemy = new Berseker(m_data, Enemy::berseker, l_position, l_neighbor, l_map, m_objectives, l_heroes);
        m_enemies.push_back(m_enemy);
    }

    sf::VertexArray SplashState::ToVertex(std::vector<sf::Vector2f> l_points) {
        sf::VertexArray vertices(sf::LinesStrip, 0);

        for (std::vector<sf::Vector2f>::const_iterator a = l_points.begin(); a != l_points.end(); ++a)
            vertices.append(sf::Vertex(*a, sf::Color::White));

        return vertices;
    }

    void SplashState::Draw(float dt) {
        this->m_data->window.clear(sf::Color::Black);
        this->m_data->window.setView(this->m_data->window.getDefaultView());
        this->m_data->window.draw(m_textoDinero);
        map->Draw();
        this->m_data->window.draw(this->_background);


        for (int i = 0; i < m_placer.size(); i++) {
            m_placer.at(i)->Draw();
        }

        for (int i = 0; i < m_pathsVertex.size(); i++) {
            this->m_data->window.draw(m_pathsVertex[i]);
        }

        for (int i = 0; i < m_wps.size(); i++) {
            this->m_data->window.draw(m_wps[i]);
        }

        for (int i = 0; i < m_objectives.size(); i++) {
            m_objectives[i]->Draw();
        }

        for (int i = 0; i < m_trampas.size(); i++) {
            if (m_trampas[i] != nullptr) {
                m_trampas.at(i)->Draw();
            }
        }

        for (int i = 0; i < m_enemys.size(); i++) {
            m_enemys.at(i)->Draw();
        }



        for (int i = 0; i < m_bullets.size(); i++) {
            m_bullets[i]->Draw();
        }

        m_hud->Draw();
        for (int i = 0; i < m_heroes.size(); i++) {
            m_heroes[i]->Draw();
        }

        for (int i = 0; i < m_enemies.size(); i++) {
            m_enemies.at(i)->Draw();
        }

        if (m_hideCursor) {
            this->m_data->window.setMouseCursorVisible(false);
            this->m_data->window.draw(m_mouseConstruct);
        } else
            this->m_data->window.setMouseCursorVisible(true);

        this->m_data->window.draw(m_mouseCoordinates);
        this->m_data->window.display();
    }

    void SplashState::CheckColision() {
        for (int i = 0; i < m_enemies.size(); i++) {
            for (int j = 0; j < m_bullets.size(); j++) {
                if (m_bullets[j]->GetType() == 1) {
                    if (abs(m_bullets[j]->GetOrigin().x - m_bullets[j]->GetSprite().getPosition().x) > RANGO_FUEGO
                            || abs(m_bullets[j]->GetOrigin().y - m_bullets[j]->GetSprite().getPosition().y) > RANGO_FUEGO) {

                        delete m_bullets[j];
                        m_bullets.erase(m_bullets.begin() + j);
                    }
                }

                if (m_enemies[i]->GetSprite().getGlobalBounds().intersects(m_bullets[j]->GetSprite().getGlobalBounds())) {
                    int killerCandidate = m_bullets[j]->WhoShooted();
                    delete m_bullets[j];
                    m_bullets.erase(m_bullets.begin() + j);
                    m_enemies[i]->TakeDamage(m_bullets[j]->GetPower());
                    if (m_enemies[i]->GetActualState() == ENEMY_STATE_DEAD) {
                        for (int w = 0; w < m_trampas.size(); w++) {
                            if (m_trampas[w] != nullptr) {
                                m_trampas[w]->DeleteTarget(m_enemies[i]->GetId());
                            }
                        }
                        m_disponible = m_trampas[killerCandidate]->CalculateRec(m_disponible);
                        delete m_enemies[i];
                        m_enemies.erase(m_enemies.begin() + i);
                    }
                }

            }
        }
        for (int j = 0; j < m_bullets.size(); j++) {
            if (m_bullets[j]->GetType() == 1) {
                if (abs(m_bullets[j]->GetOrigin().x - m_bullets[j]->GetSprite().getPosition().x) > RANGO_FUEGO
                        || abs(m_bullets[j]->GetOrigin().y - m_bullets[j]->GetSprite().getPosition().y) > RANGO_FUEGO) {

                    delete m_bullets[j];
                    m_bullets.erase(m_bullets.begin() + j);
                }
            }
        }
    }

    void SplashState::CheckFail() {
        if (m_objectives[m_objectives.size() - 1]->GetActualState() == OBJECTIVE_DESTROYED_STATE) {
            this->m_data->data.SetConfidenceLevel(-3);
            this->m_data->machine.AddState(StateRef(new PlaneReturnScene(this->m_data, false)));
        }
    }

    void SplashState::SpawnHydra(sf::Vector2f l_position, Bezier& l_path) {

        const std::vector<Enemy*>& l_neighbor = m_enemies;
        Hydra * l_enemy = new Hydra(m_data, Enemy::hydra, l_position, l_neighbor, m_routes[1]);
        m_enemies.push_back(l_enemy);

        for (int i = 0; i < l_enemy->GetBodySize(); i++) {
            m_enemies.push_back(l_enemy->GetPart(i));
        }

    }

    void SplashState::CheckDeadEnemies() {
        for (int i = 0; i < m_enemies.size(); i++) {
            if (m_enemies[i]->GetActualState() == ENEMY_STATE_DEAD) {
                for (int w = 0; w < m_trampas.size(); w++) {
                    if (m_trampas[w] != nullptr) {
                        m_trampas[w]->DeleteTarget(m_enemies[i]->GetId());
                    }
                }
                delete m_enemies[i];
                m_enemies.erase(m_enemies.begin() + i);
            }
        }
    }
}

