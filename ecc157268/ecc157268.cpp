#include<bits/stdc++.h>
#include<gmp.h>
using namespace std;

struct point {
    mpz_t first;
    mpz_t second;
};

bool equalPoints(point a, point b) {
    if(a.first == b.first && a.second == b.second)
        return true;
    return false;
}

point addPoints(point p, point q, mpz_t a, mpz_t prime) {
    point r;
    mpz_t lambda, temp;
    mpz_inits(lambda, temp, r.first, r.second, NULL);
    if(equalPoints(p, q)) {
        mpz_mul(lambda, p.first, p.first);
        mpz_mul_ui(lambda, p.first, 3);
        mpz_add(lambda, lambda, a);
        mpz_mul_ui(temp, p.second, 2);
        mpz_invert(temp, temp, prime);
        mpz_mul(lambda, lambda, temp);
        mpz_mod(lambda, lambda, prime);
    }
    else {
        mpz_sub(temp, q.first, p.first);
        mpz_invert(temp, temp, prime);
        mpz_sub(lambda, q.second, p.second);
        mpz_mul(lambda, lambda, temp);
        mpz_mod(lambda, lambda, prime);
    }

    mpz_mul(r.first, lambda, lambda);
    mpz_sub(r.first, r.first, p.first);
    mpz_sub(r.first, r.first, q.first);
    mpz_mod(r.first, r.first, prime);

    mpz_sub(r.second, p.first, r.first);
    mpz_mul(r.second, r.second, lambda);
    mpz_sub(r.second, r.second, p.second);
    mpz_mod(r.second, r.second, prime);

    return r;
}

void findN(mpz_t n, point g, point minusg, mpz_t a, mpz_t prime) {
    point result;
    mpz_inits(result.first, result.second, NULL);
    mpz_set_ui(n, 1);
    mpz_set(result.first, g.first);
    mpz_set(result.second, g.second);
    while(true) {
        result = addPoints(result, g, a, prime);
        mpz_add_ui(n, n, 1);
        if(equalPoints(result, minusg))
            break;
    }
}

point multiplyPoint(mpz_t n, point p, mpz_t a, mpz_t prime) {
    point result;
    mpz_inits(result.first, result.second, NULL);
    mpz_set(result.first, p.first);
    mpz_set(result.second, p.second);
    mpz_sub_ui(n, n, 2);
    while(mpz_sgn(n)) {
        result = addPoints(result, p, a, prime);
        mpz_sub_ui(n, n, 1);
    }
    return result;
}

int main() {
    mpz_t a, b, p, n, na, nb;
    point g, minusg, pa, pb, ka, kb;
    mpz_inits(a, b, p, n, na, nb, g.first, g.second, minusg.first, minusg.second, pa.first, pa.second, pb.first, pb.second, ka.first, ka.second, kb.first, kb.second, NULL);
    cout<<"Enter a prime number (or a number of the form 2^m) p: ";
    cin>>p;
    cout<<"Enter curve parameter a: ";
    cin>>a;
    cout<<"Enter curve parameter b: ";
    cin>>b;
    cout<<"Enter a point on the curve, G (x, y): ";
    cin>>g.first>>g.second;

    mpz_set(minusg.first, g.first);
    mpz_neg(minusg.second, minusg.second);
    mpz_mod(minusg.second, minusg.second, p);

    findN(n, g, minusg, a, p);

    gmp_randstate_t randstate;
    gmp_randinit_mt(randstate);
    gmp_randseed_ui(randstate, time(NULL));
    mpz_urandomm(na, randstate, n);
    pa = multiplyPoint(na, g, a, p);

    gmp_randseed_ui(randstate, time(NULL));
    mpz_urandomm(nb, randstate, n);
    pb = multiplyPoint(nb, g, a, p);

    ka = multiplyPoint(na, pb, a, p);
    kb = multiplyPoint(nb, pa, a, p);

    gmp_printf("n = %Zd\nna = %Zd\nnb = %Zd\nPa = (%Zd, %Zd)\nPb = (%Zd, %Zd)\nKa = (%Zd, %Zd)\nKb = (%Zd, %Zd)\n", n, na, nb, pa.first, pa.second, pb.first, pb.second, ka.first, ka.second, kb.first, kb.second);
}
