#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

namespace interpolation {

    class interp {
    private:
        std::vector<float> coef;
        float sub_x;
        float max_x;
        interp* next;

    protected:
        interp(const std::vector<float> new_coef, const float new_sub, const float new_max) :
            coef(new_coef), sub_x(new_sub), max_x(new_max), next(nullptr) {};
        interp(const std::vector<float> new_coef, const float new_sub) :
            coef(new_coef), sub_x(new_sub), max_x(new_sub), next(nullptr) {};
        void set_next(interp* ptr);

    public:
        interp() : coef(0), sub_x(0.0), max_x(0.0), next(nullptr) {};
        ~interp();
        void show() const;
        float operator()(const float x) const;

        void linear(const float y[], const int length, const float step = 1);
        void quadric(const float y[], const int length, const float step = 1);
        void cubic(const float y[], const int length, const float step = 1);
    };
}
