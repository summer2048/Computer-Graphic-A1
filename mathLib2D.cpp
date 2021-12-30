#include <math.h>
#include <iostream>
#include <experimental/random>
#include <string>
#include "mathLib2D.h"

// Implementation of Point2D
Point2D::Point2D(){
    mX = 0.0;
    mY = 0.0;
}

Point2D::Point2D(float inX, float inY){
    mX = inX;
    mY = inY;
}

float Point2D::distanceTo(Point2D other){
    float DiffX = abs(mX - other.mX);
    float DiffY = abs(mY - other.mY);
    return sqrt(pow(DiffX, 2.0) + pow(DiffY, 2.0));
}

float Point2D::fastDistanceTo(Point2D other){
    float DiffX = abs(mX - other.mX);
    float DiffY = abs(mY - other.mY);
    return pow(DiffX, 2.0) + pow(DiffY, 2.0);
}


// Implementation of Vec2D
Vec2D::Vec2D(){
    mX = 0.0;
    mY = 0.0;
}

Vec2D::Vec2D(float inX, float inY){
    mX = inX;
    mY = inY;
}

float Vec2D::length(){
    return sqrt(pow(mX, 2.0) + pow(mY, 2.0));
}

Vec2D Vec2D::normalize(){
    return Vec2D(mX/length(), mY/length());
}

Vec2D Vec2D::addVec(Vec2D vec){
    return Vec2D(this->mX+vec.mX, this->mY+vec.mY);
}
Vec2D Vec2D::multiply(float scalar){
    return Vec2D(mX * scalar, mY * scalar);
}

Point2D Vec2D::movePoint(Point2D source){
    return Point2D(source.mX+mX, source.mY + mY);
}

Vec2D Vec2D::createVector(Point2D p1, Point2D p2){
    return Vec2D(p2.mX-p1.mX, p2.mY-p1.mY);
}


Position::Position(){
}

Position::Position(float x, float y){
    Point2D point(x, y);
    this->point.mX = x;
    this->point.mY = y;
    //std::cout << "position with 2 para is called with x, y:" << to_string(x) << ", " << to_string(y)  << std::endl;
    //std::cout << to_string(this->point.mX) << ", " << to_string(x)  << std::endl;
    
}


Color::Color(){
    int a = rand() % 6;
    colorId = a;
}

Color::Color(int id){
    colorId = id;
}

// Color depends on left life duration of the particle.
void Color::scumble(int value){
    float mainColor;
    float otherColor;
    if (value >= 900){
        mainColor = 1;
        otherColor = (value - 900.0f)/1200.0f;
    } else if (value > 0){
        mainColor = value/900.0f;
        otherColor = 0;
    } else {
        mainColor = 0;
        otherColor = 0;
    }
    switch (colorId)
    {
    case RED:
        r = mainColor;
        g = otherColor;
        b = otherColor;
        break;
    case GREEN:
        r = otherColor;
        g = mainColor;
        b = otherColor;
        break;
    case BLUE:
        r = otherColor;
        g = otherColor;
        b = mainColor;
        break;
    case YELLOW:
        r = mainColor;
        g = mainColor;
        b = otherColor;
        break;
    case CYAN:
        r = otherColor;
        g = mainColor;
        b = mainColor;
        break;
    case PURPLE:
        r = mainColor;
        g = otherColor;
        b = mainColor;
        break;
    default:
        break;
    }
    //std::cout  << "r = " << to_string(r) << ", " << to_string(g) <<to_string(b) << std::endl;
}

Direction::Direction(){
    float i = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float j = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    vec = Vec2D(i-0.5f, j-0.5f).normalize();
}

Particle::Particle(){
    //std::cout << "particle without para is called!" << std::endl;
    Position position(0.0f, 0.0f);
    color = Color();
    direction = Direction();
    speed = 1.0f;
    life_duration_left = 1800; // Number of FPS it exists.
}

Particle::Particle(float mX, float mY){
    Position position(mX, mY);
    color = Color();
    direction = Direction();
    speed = 1.0f;
    life_duration_left = 1800; // Number of FPS it exists.
    this->size = 1+rand()%5;
    this->position.point.mX = mX;
    this->position.point.mY = mY;
}

// Particle::Particle(float mX, float mY, int colorId){
//     Particle(mX, mY);
//     this->color = Color(colorId);
// }

