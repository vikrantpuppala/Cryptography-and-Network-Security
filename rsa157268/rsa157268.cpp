#include<bits/stdc++.h>
#include<gmp.h>
using namespace std;

string toStringNum(int num) {
	string res;
	while(num) {
		res += to_string((unsigned char)(num%10 + '0'));
		num/=10;
	}
	reverse(res.begin(), res.end());
	return res;
}

int main() {
	mpz_t p, q, n, d, e, M, C, phi, pdec, qdec, temp;
	mpz_inits(p, q, n, d, e, M, C, phi, pdec, qdec, temp, NULL);
	cout<<"Enter two prime numbers: "<<endl;
	cin>>p>>q;
	mpz_mul(n, p, q);
	mpz_sub_ui(pdec, p, 1);
	mpz_sub_ui(qdec, q, 1);
	mpz_mul(phi, pdec, qdec);
	for(int i=2; mpz_cmp_ui(phi, i)>0; i++) {
		mpz_gcd_ui(temp, phi, i);
		if(mpz_cmp_ui(temp, 1) == 0) {
			mpz_set_ui(e, i);
			break;
		}
	}
	mpz_invert(d, e, phi);
	string plaintext, numplaintext;
	cout<<"Enter the plaintext: "<<endl;
	cin>>plaintext;
	for(int i=0; i<plaintext.size(); i++) {
		plaintext[i] = tolower(plaintext[i]);
		string temp = toStringNum(plaintext[i]-'a');
		if(temp.size() == 1)
			temp = "0" + temp;
		numplaintext+=temp;
	}
	char numptchar[10000] = numplaintext.c_str();
	mpz_set_str(M, numptchar, 10);
	mpz_powm(C, M, e, n);
	gmp_printf("%Zd\n", C); 
}
