#include<bits/stdc++.h>
using namespace std;

string stringxor(string one, string two) {
	string result;
	for(int i=max(one.size(), two.size())-1; i>=0; i--) {
		result.push_back(((one[i]+two[i]-'0'-'0')%2)+'0');
	}
	reverse(result.begin(), result.end());
	return result;
}

int main() {
	string irr_poly, polyone, polytwo;
	cout<<"Enter the irreducible polynomial in bit format (ex: x^2+1 as 101): ";
	cin>>irr_poly;
	irr_poly=irr_poly.substr(1);
	int degree = irr_poly.size();
	cout<<"Enter the first polynomial in bit format: ";
	cin>>polyone;
	cout<<"Enter the second polynomial in bit format: ";
	cin>>polytwo;
	vector<string> multiplications;
	multiplications.push_back(polyone);
	for(int i=1; i<degree; i++) {
		string temp = multiplications[i-1];
		temp.push_back('0');
		if(temp[0]=='1') 
			multiplications.push_back(stringxor(temp.substr(1), irr_poly));
		else 
			multiplications.push_back(temp.substr(1));
	}
	cout<<endl<<"Now we have the multiplications as follows:"<<endl;
	for(int i=0; i<multiplications.size(); i++)
		cout<<polyone<<" * x^"<<i<<" = "<<multiplications[i]<<endl;
	string result;
	for(int j=0; j<degree; j++)
		result.push_back('0');
	cout<<"result = xor ";
	for(int i=0; i<polytwo.size(); i++) {
		if(polytwo[i]=='1') {
			cout<<multiplications[degree-i-1]<<" ";
			result=stringxor(result, multiplications[degree-i-1]);
		}
	}
	cout<<endl<<endl;
	cout<<polyone<<" * "<<polytwo<<" = "<<result<<endl;
}
