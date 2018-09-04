#include<stdio.h>
#include<gmp.h>
#include<time.h>

void gcdExtended(mpz_t gcd, mpz_t a, mpz_t b, mpz_t x, mpz_t y)
{
    if (mpz_sgn(a) == 0)
    {
        mpz_init_set_ui(x, 0);
        if(mpz_sgn(b)>=0)
	        mpz_init_set_ui(y, 1);
	    else
	    	mpz_init_set_si(y, -1);
        mpz_init_set(gcd, b);
        mpz_abs(gcd, gcd);
        return;
    }
 
    mpz_t x1, y1, mod;
    mpz_inits(x1, y1, mod, NULL);
    mpz_mod(mod, b, a);
	gmp_printf("\nGCD(%Zd, %Zd) (remainder = %Zd)", a, b, mod);
    gcdExtended(gcd, mod, a, x1, y1);
 
 	mpz_t temp;
 	mpz_init(temp);
 	mpz_fdiv_q(temp, b, a);
 	mpz_mul(temp, temp, x1);
 	mpz_sub(temp, y1, temp);
 	mpz_init_set(x, temp);
 	mpz_init_set(y, x1);
    return;
}

int main(int argc, char* argv[]) {
    mpz_t a,b,c,x,y,z;
    char input[1024];
    
    mpz_inits(a, b, c, x, y, z,NULL);
    
	printf("\nEnter the first number: ");
	scanf("%1023s", input);
	mpz_set_str(a, input, 10);
	
	printf("Enter the second number: ");
	scanf("%1023s", input);
	mpz_set_str(b, input, 10);

	clock_t start = clock();
    gcdExtended(c, a, b, x, y);
	clock_t end = clock();
	gmp_printf("\n\nGCD(%Zd, %Zd) = %Zd\n", a, b, c);
	mpz_gcd(z, a, b);
	gmp_printf("Output of GMP library gcd function = %Zd\n", z);
	gmp_printf("x=%Zd\ty=%Zd\n\n", x, y);
	
	double timetaken = ((double) (end - start)) / CLOCKS_PER_SEC;
	gmp_printf("The time taken is %lf\n", timetaken); 
	
    return 0;
}
