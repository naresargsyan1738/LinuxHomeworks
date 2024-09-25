#include "Complex.h"
#include <cmath>
#include <iostream>

Complex::Complex(double real, double imag) : real(real), imag(imag) {}

Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::operator*(double scalar) const {
    return Complex(real * scalar, imag * scalar);
}

double Complex::abs() const {
    return std::sqrt(real * real + imag * imag);
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.real << " + " << c.imag << "i";
    return os;
}

