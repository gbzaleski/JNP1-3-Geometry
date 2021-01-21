//    Antoni Koszowski (418333) &
//    Grzegorz Zaleski (418494)

#include <cassert>
#include "geometry.h"

Geoobject::Geoobject(bint x, bint y) : px(x), py(y) {};

bint Geoobject::x() const
{
    return px;
}

bint Geoobject::y() const
{
    return py;
}

bool Geoobject::operator==(const Geoobject &gobj) const
{
    return px == gobj.px && py == gobj.py;
}

Position::Position(bint x, bint y) : Geoobject(x, y) {};

Position::Position(const Vector &vec) : Geoobject(vec.x(), vec.y()) {};

Position Position::reflection() const
{
    return Position(py, px);
}

const Position& Position::origin()
{
    static Position origin = Position(0, 0);
    return origin;
}

Position& Position::operator+=(const Vector &vec)
{
    this->px += vec.x();
    this->py += vec.y();
    return *this;
}

const Position operator+(const Position &pos, const Vector &vec)
{
    Position result = pos;
    result.px += vec.x();
    result.py += vec.y();
    return result;
}

const Position operator+(const Vector &vec, const Position &pos)
{
    Position result = pos;
    result.px += vec.x();
    result.py += vec.y();
    return result;
}

Vector::Vector(bint x, bint y) : Geoobject(x, y) {};

Vector::Vector(const Position &pos) : Geoobject(pos.x(), pos.y()){};

Vector Vector::reflection() const
{
    return Vector(py, px);
}

Vector& Vector::operator+=(const Vector &vec)
{
    this->px += vec.x();
    this->py += vec.y();
    return *this;
}

const Vector operator+(const Vector &vec1, const Vector &vec2)
{
    Vector result = vec1;
    result.px += vec2.x();
    result.py += vec2.y();
    return result;
}

Rectangle::Rectangle(bint width, bint height, Position pos)
        : rwidth(width), rheight(height), rpos(pos)
{
    assert(width > 0 && height > 0);
};

Rectangle::Rectangle(bint width, bint height)
        :rwidth(width), rheight(height), rpos(0, 0)
{
    assert(width > 0 && height > 0);
};

bint Rectangle::height() const
{
    return rheight;
}

bint Rectangle::width() const
{
    return rwidth;
}

bint Rectangle::area() const
{
    return rheight * rwidth;
}

Position Rectangle::pos() const
{
    return Position(rpos);
}

Rectangle Rectangle::reflection() const
{
    return Rectangle(rheight, rwidth, Position(rpos.reflection()));
}

bool Rectangle::operator==(const Rectangle &rec) const
{
    return rpos == rec.pos() && rwidth == rec.width() && rheight == rec.height();
}

Rectangle &Rectangle::operator+=(const Vector &vec)
{
    this->rpos += vec;
    return *this;
}

const Rectangle operator+(const Rectangle &rec, const Vector &vec)
{
    Rectangle result = rec;
    result += vec;
    return result;
}

const Rectangle operator+(const Vector &vec, const Rectangle &rec)
{
    return rec + vec;
}

Rectangles::Rectangles() : tab() {};

Rectangles::Rectangles(std::initializer_list<Rectangle> rects) : tab(rects) {};

size_t Rectangles::size() const
{
    return tab.size();
}

Rectangle& Rectangles::operator[](size_t id)
{
    assert(id >= 0 && id < tab.size());
    return tab[id];
}

const Rectangle& Rectangles::operator[](size_t id) const
{
    assert(id >= 0 && id < tab.size());
    return tab[id];
}

bool Rectangles::operator==(const Rectangles &recs) const
{
    if (this->size() == recs.size())
    {
        for (size_t i = 0; i < this->size(); ++i)
        {
            if (!(this->tab[i] == recs.tab[i]))
                return false;
        }

        return true;
    }

    return false;
}

Rectangles &Rectangles::operator+=(const Vector &vec)
{
    for (auto &rec: this->tab)
    {
        rec += vec;
    }
    return *this;
}

const Rectangles operator+(const Rectangles &recs, const Vector &vec)
{
    Rectangles result = recs;
    result += vec;
    return result;
}

const Rectangles operator+(const Vector &vec, const Rectangles &recs)
{
    return recs + vec;
}

const Rectangles operator+(Rectangles &&recs, const Vector &vec)
{
    Rectangles result = recs;
    result += vec;
    return result;
}

const Rectangles operator+(const Vector &vec, Rectangles &&recs)
{
    return std::move(recs) + vec;
}

static inline bool check_merge_hori(const Rectangle &rec1, const Rectangle &rec2)
{
    return rec1.width() == rec2.width()
        && rec1.pos().y() + rec1.height() == rec2.pos().y()
        && rec1.pos().x() == rec2.pos().x();
}

static inline bool check_merge_vert(const Rectangle &rec1, const Rectangle &rec2)
{
    return rec1.height() == rec2.height()
        && rec1.pos().x() + rec1.width() == rec2.pos().x()
        && rec1.pos().y() == rec2.pos().y();
}

Rectangle merge_horizontally(const Rectangle &rec1, const Rectangle &rec2)
{
    assert(check_merge_hori(rec1, rec2));
    return Rectangle(rec1.width(), rec1.height() + rec2.height(), rec1.pos());
}

Rectangle merge_vertically(const Rectangle &rec1, const Rectangle &rec2)
{
    assert(check_merge_vert(rec1, rec2));
    return Rectangle(rec1.width() + rec2.width(), rec2.height(), rec1.pos());
}

Rectangle merge_all(const Rectangles &recs)
{
    assert(recs.size());
    Rectangle result = recs[0];
    for (size_t i = 1; i < recs.size(); ++i)
    {
        if (check_merge_hori(result, recs[i]))
        {
            result = merge_horizontally(result, recs[i]);
        }
        else if (check_merge_vert(result, recs[i]))
        {
            result = merge_vertically(result, recs[i]);
        }
        else
        {
            assert(false);
        }
    }

    return result;
}