#ifndef ZAMLICZENIOWY_GEOMETRY_H
#define ZAMLICZENIOWY_GEOMETRY_H

#include <cstdlib>
#include <vector>
#include <initializer_list>

// Simplifying code.
using bint = int32_t;
using std::vector;

class Vector;

// Generic class representing a geometric object with (x, y) coordinates
// on the Cartesian Plane.
class Geoobject
{
protected:
    bint px, py;

    // So that it could be invoked either from class Position or class Vector.
    Geoobject (bint x, bint y);

public:
    bint x() const;

    bint y() const;

    bool operator==(const Geoobject &gobj) const;
};

// Class representing a point (position on plane), with (x, y) coordinates.
class Position: public Geoobject
{
public:
    Position(bint x, bint y);

    explicit Position(const Vector &vec);

    Position reflection() const;

    static const Position& origin();

    Position& operator+=(const Vector&);

    friend const Position operator+(const Position &pos, const Vector &vec);

    friend const Position operator+(const Vector &vec, const Position &pos);
};

// Class representing a vector defined by (x, y) coordinates.
class Vector: public Geoobject
{
public:
    Vector(bint x, bint y);

    explicit Vector(const Position &pos);

    Vector reflection() const;

    Vector& operator+=(const Vector& vec);

    friend const Vector operator+(const Vector &vec1, const Vector &vec2);
};

// Class representing a rectangle defined by (x,y) coordinates
// of left bottom corner as well as width and height parameters.
class Rectangle
{
    bint rwidth, rheight;
    Position rpos;

public:
    Rectangle(bint width, bint height, Position pos);

    Rectangle(bint width, bint height);

    bint width() const;

    bint height() const;

    bint area() const;

    Position pos() const;

    Rectangle reflection() const;

    bool operator==(const Rectangle &rec) const;

    Rectangle& operator+=(const Vector &vec);

    friend const Rectangle operator+(const Rectangle &rec, const Vector &vec);

    friend const Rectangle operator+(const Vector &vec, const Rectangle &rec);
};

// Class representing a collection of objects of class Rectangle.
class Rectangles
{
    vector<Rectangle> tab;

public:
    Rectangles();

    Rectangles(std::initializer_list<Rectangle> rects);

    size_t size() const;

    Rectangle& operator[](size_t id);

    const Rectangle& operator[](size_t id) const;

    bool operator==(const Rectangles &recs) const;

    Rectangles& operator+=(const Vector &vec);

    friend const Rectangles operator+(const Rectangles &recs, const Vector &vec);

    friend const Rectangles operator+(const Vector &vec, const Rectangles &recs);

    friend const Rectangles operator+(Rectangles &&recs, const Vector &vec);

    friend const Rectangles operator+(const Vector &vec, Rectangles &&recs);

};

Rectangle merge_horizontally(const Rectangle &rec1, const Rectangle &rec2);

Rectangle merge_vertically(const Rectangle &rec1, const Rectangle &rec2);

Rectangle merge_all(const Rectangles &recs);

#endif //ZAMLICZENIOWY_GEOMETRY_H