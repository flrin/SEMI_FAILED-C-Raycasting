#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Windows.h>

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067


using namespace std;
using namespace sf;

bool PointBetweenCoords(float x, Vector2f co) {
    if (co.x > co.y)
    {
        float a = co.x;
        co.x = co.y;
        co.y = a;
    }
    if (x >= co.x && x <= co.y)
        return 1;
    return 0;
}

Vector2f GetVollision(VertexArray ray, VertexArray line) {
    if (ray[0].position.x == ray[1].position.x) {
        float aux;
        float y1= ray[0].position.y;
        float y2 = ray[1].position.y;

        float ag = ((line[0].position.y - line[1].position.y) / (line[0].position.x - line[1].position.x));
        float bg = line[0].position.y - ag * line[0].position.x;
        
        if (y1 > y2) {
            aux = y1;
            y1 = y2;
            y2 = aux;
        }
        float gx = ag * ray[0].position.x + bg;
        for(int i=y1;i<=y2;i++)
            if (PointBetweenCoords(i, Vector2f(line[0].position.x, line[1].position.x)) && gx == i) {
                return Vector2f(ray[0].position.x,gx);
            }
    }
    else if (line[0].position.x == line[1].position.x) {
        float aux;
        float y1 = line[0].position.y;
        float y2 = line[1].position.y;

        float af = ((ray[0].position.y - ray[1].position.y) / (ray[0].position.x - ray[1].position.x));
        float bf = ray[0].position.y - af * ray[0].position.x;

        if (y1 > y2) {
            aux = y1;
            y1 = y2;
            y2 = aux;
        }
        float gx = af * ray[0].position.x + bf;
        for (int i = y1; i <= y2; i++)
            if (PointBetweenCoords(i, Vector2f(line[0].position.x, line[1].position.x)) && gx == i) {
                return Vector2f(ray[0].position.x, gx);
            }
    }
    else {
        float af = ((ray[0].position.y - ray[1].position.y) / (ray[0].position.x - ray[1].position.x));
        float bf = ray[0].position.y - af * ray[0].position.x;

        float ag = ((line[0].position.y - line[1].position.y) / (line[0].position.x - line[1].position.x));
        float bg = line[0].position.y - ag * line[0].position.x;

        if (af == ag)
            return Vector2f(0, 0);
        float x = (bg - bf) / (af - ag);
        if (PointBetweenCoords(x, Vector2f(ray[0].position.x, ray[1].position.x)) && PointBetweenCoords(x, Vector2f(line[0].position.x, line[1].position.x))) {
            float fx = af * x + bf;
            if (PointBetweenCoords(fx, Vector2f(ray[0].position.y, ray[1].position.y)) && PointBetweenCoords(fx, Vector2f(line[0].position.y, line[1].position.y)))
                return Vector2f(x, fx);
        }
    }
    return Vector2f(0, 0);
}

void SetLines(long double lineAngle, VertexArray& lines, Vector2f poz,VertexArray linie) {
    int j = 1;
    double firstSin, firstCos;
    for (int i = 0; i < 360 ; i += lineAngle) {
        firstSin = sin(i * PI / 180);
        firstCos = cos(i * PI / 180);
        
        lines[j].position = Vector2f(firstCos*1000+poz.x,firstSin*1000+poz.y);
        
        lines[j - 1].position = poz;
        VertexArray l(Lines, 2);
        l[0].position = lines[j-1].position;
        l[1].position = lines[j].position;
        if (GetVollision(l,linie) != Vector2f(0, 0)) {
            lines[j].position = GetVollision(l,linie);
        }

        j += 2;
    }
}

int main()
{
    RenderWindow window(sf::VideoMode(500, 500), "Raycasting Project");
    int lineNumber = 4;
    VertexArray lines(Lines, 1000);
    long double lineAngles = 360 / lineNumber;
    Vector2f poz;
    FloatRect patrat(200,200,100,100);
    RectangleShape p2(Vector2f(100,100));
    p2.setPosition(200,200);
    VertexArray linie(Lines, 2);
    linie[0].position = Vector2f(400,200);
    linie[1].position = Vector2f(300, 400);
    linie[0].color = Color::Red;
    linie[1].color = Color::Red;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        poz = Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

        if (Keyboard::isKeyPressed(Keyboard::A)) {
            lineNumber--;
            lineAngles = 360 / lineNumber;
            sleep(seconds(0.1));
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            lineNumber++;
            lineAngles = 360 / lineNumber;
            sleep(seconds(0.1));
        }
        
        SetLines(lineAngles, lines, poz,linie);

        window.clear();

        window.draw(lines);
        //window.draw(p2);
        window.draw(linie);

        window.display();
    }

    return 0;
}