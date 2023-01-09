#include <iostream>
#include "scheme.h"

int main() {
    Interpreter inter;
    inter.Run("(define x 1)");
    inter.Run("(define (f x) (set! x (+ x 1)))"); // x = new int(*x + 1)
    inter.Run("(f x)");
    std::cout << inter.Run("(+ x)");
    return 0;
}
