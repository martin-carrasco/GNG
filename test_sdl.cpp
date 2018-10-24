//
// Created by martin on 10/23/18.
//
#include "SFML/Graphics.hpp"
#include <iostream>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 980

sf::CircleShape draw_node(sf::Vector2i pos){
    sf::CircleShape node(1);
    node.setFillColor(sf::Color::Green);
    node.setPosition(pos.x, pos.y);
}
sf::RectangleShape draw_edge(sf::Vector2f start, sf::Vector2f end){
    sf::Vertex edge[] = {
            sf::Vertex(start, sf::Color::Red),
            sf::Vertex(end, sf::Color::Red)
    };
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_HEIGHT, SCREEN_WIDTH), "Growing Neural Gas");
    sf::RectangleShape linea;

    std::vector<std::pair<sf::Vertex, sf::Vertex>> to_draw;
    std::vector<sf::CircleShape> nodes;
    std::vector<sf::RectangleShape> edges;
    std::vector<sf::CircleShape> to_make;
    sf::VertexArray figure(sf::LinesStrip, 10000);

    bool is_drawing = false;

    bool is_running = true;
    bool is_pressed = false;
    sf::Vector2i pos1(-1, -1);
    sf::Vector2i pos2(-1, -1);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                is_pressed = true;



               // pos1 = sf::Mouse::getPosition(window);
               // is_drawing = true;
            }
            if(event.type == sf::Event::MouseMoved){
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    sf::Vector2f pos_f = sf::Vector2f(pos.x, pos.y);
                    figure.append(sf::Vertex(pos_f));
                   // sf::CircleShape node(1);
                   // node.setFillColor(sf::Color::White);
                   // node.setPosition(pos.x, pos.y);

                   // to_make.push_back(node);
                }

            }
            if(event.type == sf::Event::MouseButtonReleased){

                is_pressed = false;
                /*if(is_drawing) {
                    pos2 = sf::Mouse::getPosition(window);
                    is_drawing = false;
                    to_draw.push_back(
                            std::make_pair<sf::Vertex, sf::Vertex>(
                                    sf::Vertex(sf::Vector2f(pos1.x, pos1.y), sf::Color::White),
                                    sf::Vertex(sf::Vector2f(pos2.x, pos2.y), sf::Color::White)
                                    )
                            );

                    //TODO make for min
                    for(float x = pos1.x;x <pos2.x;x++){
                        for(float y = pos1.y;y < pos2.y;y++){
                            std::cout << "X: " << x << "Y: " << y << std::endl;
                        }
                    }
                }*/
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                is_running = !is_running;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                pos1.x = -1;pos1.y = -1;
                pos2.x = -1;pos2.y = -1;
                figure.clear();
                window.clear();
            }
        }

        window.draw(figure);
        for(sf::CircleShape s : to_make){
            window.draw(s);
        }
        for(std::pair<sf::Vertex, sf::Vertex> v : to_draw){
            sf::Vertex l[] = {
                    v.first,
                    v.second
            };
            window.draw(l, 2, sf::Lines);
        }
/*        if(pos1.x != -1 && pos1.y != -1 && pos2.x != -1 && pos2.y != -1){
            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(pos1.x, pos1.y), sf::Color::White),
                    sf::Vertex(sf::Vector2f(pos2.x, pos2.y), sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
            pos1.x = -1;pos2.y = -1;
            pos2.x = -1;pos1.y = -1;

        }*/
        for(sf::CircleShape n : nodes){
            window.draw(n);
        }
        for(sf::RectangleShape l : edges){
            window.draw(l);
        }
        window.display();

        //nodes.push_back(draw_node(x, y)); Añadir un node a la cola de dibujo
        //edges.push_back(draw_edge(x, y)); Añadir un edge a la cola de dibujo

        if(is_running){

        }
    }

    return 0;
}

