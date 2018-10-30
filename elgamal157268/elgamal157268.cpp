#include<bits/stdc++.h>
#include<gmp.h>
using namespace std;

int main() {
    mpz_t p, alpha, d, beta, k, r, t, pdecr, M, DM, minusd;
    gmp_randstate_t randstate;
    mpz_inits(p, alpha, d, beta, k, r, t, pdecr, M, DM, minusd, NULL);
    cout<<"Enter a large prime number p: ";
    cin>>p;
    cout<<"Enter a primitive element alpha: ";
    cin>>alpha;
    gmp_randinit_mt(randstate);
    mpz_sub_ui(pdecr, p, 1);
    mpz_urandomm(d, randstate, pdecr);
	mpz_powm(beta, alpha, d, p);
    cout<<"Enter the plaintext (numerical): ";
    cin>>M;

    gmp_randinit_mt(randstate);
    mpz_urandomm(k, randstate, pdecr);

    mpz_powm(r, alpha, k, p);
    mpz_powm(t, beta, k, p);
    mpz_mul(t, t, M);
    mpz_mod(t, t, p);

    gmp_printf("\nEnciphering..\nCiphertext: (%Zd, %Zd)\n", r, t);

    mpz_neg(minusd, d);
    mpz_powm(DM, r, minusd, p);
    mpz_mul(DM, DM, t);
    mpz_mod(DM, DM, p);

    gmp_printf("\nDeciphering...\nDeciphered text: %Zd\n", DM);
}
