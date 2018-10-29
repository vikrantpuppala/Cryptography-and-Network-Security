#include<bits/stdc++.h>
#include<gmp.h>
using namespace std;

string toStringNum(int num) {
	string res;
	if(num == 0)
		return "0";
	while(num) {
		res.push_back((unsigned char)(num%10 + '0'));
		num/=10;
	}
	reverse(res.begin(), res.end());
	return res;
}

int toNum(string s) {
	int res=(s[0]-'0')*10 + (s[1]-'0');
	return res;
}

string toStringChar(string s) {
	string res="";
	if(s.size()%2)
		s="0"+s;
	while(s.size()) {
		string temp = s.substr(s.size()-2, 2);
		res.push_back((unsigned char)(toNum(temp)+'a'));
		s = s.substr(0, s.size()-2);
	}
	reverse(res.begin(), res.end());
	return res;
}

int main() {
	mpz_t p, q, n, d, e, M, C, phi, pdec, qdec, temp;
	mpz_inits(p, q, n, d, e, M, C, phi, pdec, qdec, temp, NULL);
	cout<<"Enter two prime numbers: ";
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
	gmp_printf("\nn: %Zd\nphi: %Zd\ne: %Zd\nd: %Zd\n", n, phi, e, d);
	string plaintext, numplaintext;
	cout<<"\nEnter the plaintext: ";
	cin>>plaintext;
	for(int i=0; i<plaintext.size(); i++) {
		plaintext[i] = tolower(plaintext[i]);
		string temp = toStringNum(plaintext[i]-'a');
		if(temp.size() == 1)
			temp = "0" + temp;
		numplaintext+=temp;
	}
	char* numptchar = (char*)numplaintext.c_str();
	mpz_set_str(M, numptchar, 10);
	mpz_powm(C, M, e, n);
	gmp_printf("\nEnciphering...\nCiphertext: %Zd\n", C);
	mpz_powm(M, C, d, n);
	char* pt = new char[mpz_sizeinbase (M, 10) + 2];
	mpz_get_str(pt, 10, M);
	string res(pt);
	cout<<"\nDeciphering...\nPlaintext: "<<toStringChar(res)<<endl;
}
