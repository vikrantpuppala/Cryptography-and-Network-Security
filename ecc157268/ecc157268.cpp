#include<bits/stdc++.h>
#include<gmp.h>
using namespace std;

void addPoints(mpz_t px, mpz_t py, mpz_t qx, mpz_t qy, mpz_t a, mpz_t prime) {
    mpz_t rx, ry, lambda, temp;
    mpz_inits(lambda, temp, rx, ry, NULL);
    if(!(mpz_cmp(px, qx) || mpz_cmp(py, qy))) {
        mpz_mul(lambda, px, px);
        mpz_mul_ui(lambda, lambda, 3);
        mpz_add(lambda, lambda, a);
        // gmp_printf("num = %Zd\n", lambda);
        mpz_mul_ui(temp, py, 2);
        // gmp_printf("den = %Zd\n", temp);
        mpz_invert(temp, temp, prime);
        // gmp_printf("deninv = %Zd\n", temp);
        mpz_mul(lambda, lambda, temp);
        // gmp_printf("mul = %Zd\n", lambda);
        mpz_mod(lambda, lambda, prime);
        // gmp_printf("mulmod = %Zd\n", lambda);
    }
    else {
        mpz_sub(temp, qx, px);
        mpz_invert(temp, temp, prime);
        mpz_sub(lambda, qy, py);
        mpz_mul(lambda, lambda, temp);
        mpz_mod(lambda, lambda, prime);
    }

    // gmp_printf("lambda = %Zd\n", lambda);

    mpz_mul(rx, lambda, lambda);
    mpz_sub(rx, rx, px);
    mpz_sub(rx, rx, qx);
    mpz_mod(rx, rx, prime);

    mpz_sub(ry, px, rx);
    mpz_mul(ry, ry, lambda);
    mpz_sub(ry, ry, py);
    mpz_mod(ry, ry, prime);

    mpz_set(px, rx);
    mpz_set(py, ry);
}

void findN(mpz_t n, mpz_t gx, mpz_t gy, mpz_t minusgx, mpz_t minusgy, mpz_t a, mpz_t prime) {
    mpz_t resultx, resulty;
    mpz_inits(resultx, resulty, NULL);
    mpz_set_ui(n, 1);
    mpz_set(resultx, gx);
    mpz_set(resulty, gy);
        // gmp_printf("n = %Zd\tresult = %Zd, %Zd\n", n, resultx, resulty);
    while(true) {
        addPoints(resultx, resulty, gx, gy, a, prime);
        mpz_add_ui(n, n, 1);
        // gmp_printf("n = %Zd\tresult = %Zd, %Zd\n", n, resultx, resulty);
        if(!(mpz_cmp(resultx, minusgx) || mpz_cmp(resulty, minusgy)))
            break;
    }
}

void multiplyPoint(mpz_t resultx, mpz_t resulty, mpz_t n, mpz_t px, mpz_t py, mpz_t a, mpz_t prime) {
    mpz_inits(resultx, resulty, NULL);
    mpz_set(resultx, px);
    mpz_set(resulty, py);
    mpz_sub_ui(n, n, 2);
    while(mpz_sgn(n)>0) {
        gmp_printf("n = %Zd\tresult = %Zd, %Zd\n", n, resultx, resulty);
        addPoints(resultx, resulty, px, py, a, prime);
        mpz_sub_ui(n, n, 1);
    }
}

int main() {
    mpz_t a, b, p, n, na, nb, gx, gy, minusgx, minusgy, pax, pay, pbx, pby, kax, kay, kbx, kby;
    mpz_inits(a, b, p, n, na, nb, gx, gy, minusgx, minusgy, pax, pay, pbx, pby, kax, kay, kbx, kby, NULL);
    cout<<"Enter a prime number (or a number of the form 2^m) p: ";
    cin>>p;
    cout<<"Enter curve parameter a: ";
    cin>>a;
    cout<<"Enter curve parameter b: ";
    cin>>b;
    cout<<"Enter a point on the curve, G (x, y): ";
    cin>>gx>>gy;

    gmp_printf("gx = %Zd\tgy = %Zd\t%d\n", gx, gy, (unsigned int)1);
    mpz_set(minusgx, gx);
    mpz_neg(minusgy, gy);
    mpz_mod(minusgy, minusgy, p);

    gmp_printf("minusgx = %Zd\tminusgy = %Zd\n", minusgx, minusgy);
    findN(n, gx, gy, minusgx, minusgy, a, p);

    gmp_printf("n = %Zd\n", n);

    gmp_randstate_t randstate;
    gmp_randinit_mt(randstate);
    mpz_urandomm(na, randstate, n);
    gmp_printf("na = %Zd\t", na);
    multiplyPoint(pax, pay, na, gx, gy, a, p);

    gmp_randseed_ui(randstate, time(NULL));
    mpz_urandomm(nb, randstate, n);
    gmp_printf("nb = %Zd\n", nb);
    multiplyPoint(pbx, pby, nb, gx, gy, a, p);

    multiplyPoint(kax, kay, na, pbx, pby, a, p);
    multiplyPoint(kbx, kby, nb, pax, pay, a, p);

    gmp_printf("n = %Zd\nna = %Zd\nnb = %Zd\nPa = (%Zd, %Zd)\nPb = (%Zd, %Zd)\nKa = (%Zd, %Zd)\nKb = (%Zd, %Zd)\n", n, na, nb, pax, pay, pbx, pby, kax, kay, kbx, kby);
}
