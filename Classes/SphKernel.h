#ifndef __JFSPH_KERNEL_H__
#define __JFSPH_KERNEL_H__

#include "math/Vec2.h"

class SphKernel {
public:
    virtual void setRadius(float h) = 0;
    virtual float value(float r) const = 0;
    virtual float firstDerivative(float r) const = 0;
    virtual float secondDerivative(float r) const = 0;

    cocos2d::Vec2 gradient(
        float distance,
        const cocos2d::Vec2& directionToCenter) const {
        return directionToCenter * -firstDerivative(distance);
    }

    float radius() { return _h; }

protected:
    float _h;
};

class SphPoly6Kernel2 final : public SphKernel {
public:
    static SphKernel* create() {
        return new SphPoly6Kernel2(1.0f);
    }

    SphPoly6Kernel2(float h) {
        setRadius(h);
    }

    void setRadius(float h) {
        _h = h;
        _h2 = h * h;
        _h3 = h * _h2;
        _h4 = h * _h3;
    }

    float value(float r) const override {
        if (r > _h)
            return 0.0;

        float t = 1 - r * r / _h2;
        return 4.0 / M_PI / _h2 * t * t * t;
    }

    float firstDerivative(float r) const override {
        if (r > _h)
            return 0.0;

        double t = 1.0 - r * r / _h2;
        return -24.0 * r / (M_PI * _h4) * t * t;
    }

    float secondDerivative(float r) const override {
        if (r > _h)
            return 0.0;

        float t = r * r / _h2;
        return 24.0 / (M_PI * _h4) * (1.0 - t) * (5.0 * t - 1.0);
    }

private:
    float _h2, _h3, _h4;
};

class SphSpikyKernel2 final : public SphKernel {
public:
    static SphKernel* create() {
        return new SphSpikyKernel2(1.0f);
    }

    SphSpikyKernel2(float h) {
        setRadius(h);
    }

    void setRadius(float h) {
        _h = h;
        _h2 = h * h;
        _h3 = h * _h2;
        _h4 = h * _h3;
    }

    float value(float r) const override {
        if (r > _h)
            return 0.0;

        float t = 1 - r / _h;
        return 10.0 / M_PI / _h2 * t * t * t;
    }

    float firstDerivative(float r) const override {
        if (r > _h)
            return 0.0;

        double t = 1.0 - r / _h;
        return -30.0 / (M_PI * _h3) * t * t;
    }

    float secondDerivative(float r) const override {
        if (r > _h)
            return 0.0;

        double t = 1.0 - r / _h;
        return 60.0 / (M_PI * _h4) * t;
    }

private:
    float _h2, _h3, _h4;
};

#endif