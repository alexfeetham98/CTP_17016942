#pragma once
#include "Vector3.h"
#include "Perlin.h"

class Texture
{
public:
    virtual colour value(double u, double v, const point3& p) const = 0;
};

class solid_colour : public Texture {
public:
    solid_colour() {}
    solid_colour(colour c) : colour_value(c) {}

    solid_colour(double red, double green, double blue)
        : solid_colour(colour(red, green, blue)) {}

    virtual colour value(double u, double v, const Vector3& p) const override {
        return colour_value;
    }

private:
    colour colour_value;
};

class checker_texture : public Texture
{
public:
    checker_texture() {}

    checker_texture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
        : even(_even), odd(_odd) {}

    checker_texture(colour c1, colour c2)
        : even(make_shared<solid_colour>(c1)), odd(make_shared<solid_colour>(c2)) {}

    virtual colour value(double u, double v, const point3& p) const override {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    shared_ptr<Texture> odd;
    shared_ptr<Texture> even;
};

class Noise_Texture : public Texture
{
public:
    Noise_Texture() {}
    Noise_Texture(double sc) : scale(sc) {}

    virtual colour value(double u, double v, const point3& p) const override
    {
        //return colour(1, 1, 1) * noise.noise(p);
        //return colour(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
        return colour(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
    }

public:
    Perlin noise;
    double scale;
};