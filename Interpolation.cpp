#include "Interpolation.h"
#include <cmath>

using namespace interpolation;

void interp::set_next(interp* ptr) {
	next = ptr;
}

interp::~interp() {
    if (next != nullptr) {
        delete next;
    }
};

void interp::show() const {
    std::cout << "x =< " << max_x << ":";
    for (size_t it = 0; it < coef.size(); ++it) {
        if (coef[it] < 0)
            std::cout << "\t" << coef[it] << "(x - " << sub_x << ")^" << it;
        else
            std::cout << "\t+" << coef[it] << "(x - " << sub_x << ")^" << it;
    }
    std::cout << '\n';
    if (next != nullptr) next->show();
}

float interp::operator()(const float x) const {
    if (x > max_x) {
        if (next != nullptr) return next->operator()(x);
        else {
            std::cout << "Value x = " << x << " is out of range\n";
            return 0;
        }
    }
    float sum = 0;
    for (size_t it = 0; it < coef.size(); ++it)
        sum += coef[it] * pow((x-sub_x), it);
    return sum;
}


void interp::linear(const float y[], const int length, const float step) {
    coef.clear();
    if (next != nullptr) delete next;

    // setup coefficients for this(first) segment
    max_x = step;
    sub_x = step;
    coef.push_back(y[1]);
    coef.push_back((y[1] - y[0]) / step);

    // setup data for the rest of segments and link it aka list
    interp* previous = this;
    for (int it = 2; it < length; ++it) {
        std::vector<float> coefs(2);
        coefs[0] = y[it];
        coefs[1] = (y[it] - y[it - 1]) / step;

        interp* current = new interp(coefs, step * it);
        previous->set_next(current);
        previous = current;
    }
}

void interp::quadric(const float y[], const int length, const float step) {
    coef.clear();
    if (next != nullptr) delete next;

    // setup coefficients for this(first) segment
    max_x = step;
    sub_x = step;
    coef.push_back(y[1]);
    coef.push_back((y[2] - y[0]) / (2 * step));
    coef.push_back((y[2] - 2 * y[1] + y[0]) / (2 * step * step));

    // setup data for the rest of segments and link it aka list
    interp* previous = this;
    for (int it = 2; it < length - 1; ++it) {
        std::vector<float> coefs(3);
        coefs[0] = y [it];
        coefs[1] = (y[it + 1] - y[it - 1]) / (2 * step);
        coefs[2] = (y[it + 1] - 2 * y[it] + y[it - 1]) / (2 * step * step);
                
        interp* current = new interp(coefs, step * it);
        previous->set_next(current);
        previous = current;
    }

    // setup last segment data equal the previus one (border)
    interp* last = new interp(previous->coef, step*(length-2), step*(length-1));
    previous->set_next(last);
}

void interp::cubic(const float y[], const int length, const float step) {
    coef.clear();
    if (next != nullptr) delete next;

    // border conditions for matrix coefficients
    std::vector<float> P(length), Q(length), c(length);
    float dy;
    P[0] = 0.0;
    Q[0] = 0.0;
    c[0] = 0.0;
    c[length - 1] = 0.0;

    // calculate the rest of matrix coefficients
    for (int it = 1; it < length - 1; ++it) {
        dy = (y[it + 1] + y[it - 1] - 2 * y[it]) * 3 / step / step;
        P[it] = -1.0 / (4 + P[it - 1]);
        Q[it] = (dy - Q[it - 1]) / (4 + P[it - 1]);
    }
    for (int it = length - 2; it > 0; --it) 
        c[it] = P[it] * c[it + 1] + Q[it];
    
    // setup coefficients for this(first) segment
    max_x = step;
    sub_x = step;
    coef.push_back(y[1]);
    coef.push_back((y[1] - y[0]) / step + (2 * c[1] )/ 3 * step);
    coef.push_back(c[1]);
    coef.push_back(c[1] / 3 / step);

    // setup data for the rest of segments and link it aka list
    interp* previous = this;
    for (int it = 2; it < length; ++it) {
        std::vector<float> coefs(4);

        coefs[0] = y[it];
        coefs[1] = (y[it] - y[it - 1]) / step + (2 * c[it] + c[it - 1]) / 3 * step;
        coefs[2] = c[it];
        coefs[3] = (c[it] - c[it - 1]) / 3 / step;

        interp* current = new interp(coefs, step * it);
        previous->set_next(current);
        previous = current;
    }
}
