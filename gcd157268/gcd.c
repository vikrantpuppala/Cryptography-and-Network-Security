#include<stdio.h>
#include<gmp.h>
#include<time.h>

void gcd(mpz_t z, mpz_t x, mpz_t y) {
	if(mpz_sgn(y) == 0) {
		mpz_init_set(z, x);
		return;
	}
	mpz_t k;
	mpz_init(k);
	mpz_mod(k, x, y);
	gmp_printf("\nGCD(%Zd, %Zd) (remainder = %Zd)", x, y, k);
	gcd(z, y, k);
}

int main(int argc, char* argv[]) {
    mpz_t a,b,c,z;
    char input[1024];
    
    mpz_inits(a, b, c, z, NULL);
    
    printf("\nEnter the first number: ");
    scanf("%1023s", input);
    mpz_set_str(a, input, 10);
    
    printf("Enter the second number: ");
    scanf("%1023s", input);
    mpz_set_str(b, input, 10);
    
	clock_t start = clock();
    gcd(c, a, b);
	clock_t end = clock();
	gmp_printf("\n\nGCD(%Zd, %Zd) = %Zd\n", a, b, c);
	mpz_gcd(z, a, b);
	gmp_printf("Output of GMP library gcd function = %Zd\n", z);
	
	double timetaken = ((double) (end - start)) / CLOCKS_PER_SEC;
	gmp_printf("The time taken is %lf\n", timetaken); 
	
    return 0;
}
