//
//  ray_builtins.c
//  rayc
//
//  Created by Jeremy Jacobson on 11/15/14.
//  Copyright (c) 2014 Jeremy Jacobson. All rights reserved.
//

#include <math.h>
#include <stdio.h>
#include <iostream>

int __RAY_RUNTIME_POW_INT(int base, int power) {
    if (power == 0) {
        return 1;
    }
    return base * __RAY_RUNTIME_POW_INT(base, power);
}

double __RAY_RUNTIME_POW_FLOAT(double base, double power) {
    return pow(base, power);
}

void print(char *s) {
    std::cout << s << std::endl;
}