#pragma once

#include <string>
#include <vector>

class Curve
{
public:
    virtual float MaxX() const = 0;  // MaxX defines the max of X-Axis for this curve.
    virtual float MinX() const = 0;  // MinX defines the min of X-Axis for this curve.
    virtual std::string Color() const = 0;  // Color defines the color of the curve.
    virtual float Value(float p, float x) const = 0;  // Value(p, x) returns y at x for slider value p.
    virtual bool Point() const = 0;  // Point defines if the curve should be plotted as a line or point.
    virtual ~Curve() = default;
};

class Graph
{
public:
    virtual float MaxX() const = 0;  // MaxX defines the max of X-Axis for this graph.
    virtual float MinX() const = 0;  // MinX defines the min of X-Axis for this graph.
    virtual float MaxY() const = 0;  // MaxY defines the max of Y-Axis for this graph.
    virtual float MinY() const = 0;  // MinY defines the min of Y-Axis for this graph.
    virtual float MaxP() const = 0;  // MaxP defines the max of slider parameter.
    virtual float MinP() const = 0;  // MinP defines the min of slider parameter.
    virtual std::vector<const Curve*> Curves() const = 0;  // Curves defines the curves for this graph.
    virtual ~Graph() = default;
};
