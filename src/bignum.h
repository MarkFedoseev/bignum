#include <vector>
#include <cstring>
#include <climits>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <map>

using namespace std;

class bigint {

private:
	vector<int> number;
	char sign = 1;
	static vector<int> primes;
	int BASE = INT_MAX;

	//CONSTRUCTORS:
public:
	bigint() {
		number = { 0 };
		sign = 1;
	}
	bigint(const int& num, char sgn = 1) {
		number = { num };
		if (sgn >= 0)
			sign = 1;
		else
			sign = -1;
	}
	bigint(const vector<int>& vec, char sgn = 1, char rev = 1) {
		vector<int> v = vec;
		if (rev)
			reverse(v.begin(), v.end());
		number = v;
		if (sgn >= 0)
			sign = 1;
		else
			sign = -1;
	}
	bigint(const string& num, int rev = 1) {
		string s = num;
		string delimiter = " ";
		vector<int> nm;

		size_t pos = 0;
		string token;
		while ((pos = s.find(delimiter)) != string::npos) {
			token = s.substr(0, pos);
			nm.push_back(atoi(token.c_str()));
			s.erase(0, pos + delimiter.length());
		}

		nm.push_back(atoi(s.c_str()));

		if (rev)
			reverse(nm.begin(), nm.end());
		number = nm;
	}
	int get_sign() {
		return sign;
	}
	vector<int> get_number(int rev = 1) {
		if (rev) {
			vector<int> rev_num = number;
			reverse(rev_num.begin(), rev_num.end());
			return rev_num;
		}
		else
			return number;
	}
	string get_string(int rev = 1) {
		string res = "";
		vector<int> num;
		if (rev)
			num = get_number();
		else
			num = get_number(0);
		for (auto&& num : num)
			res += to_string(num) + " ";
		res.pop_back();
		return res;
	}
	int get_size() {
		return number.size();
	}
	void print_me() {
		vector<int> rev_vec = get_number();
		if (sign < 0)
			cout << "-";
		for (auto&& num : rev_vec)
			cout << num << " ";
		cout << endl;
	}
	vector<int> get_binary() {

		vector<long long> num;
		vector<int> res;

		for (auto iter = number.begin(); iter != number.end(); iter++)
			num.push_back((long long)(*iter));

		char flag = 1;
		while (num.size() > 0) {
			for (int i = num.size() - 1; i > 0; i--)
				if (num[i] % 2 == 0)
					num[i] = num[i] / 2;
				else {
					num[i] = num[i] / 2;
					num[i - 1] += (long long)INT_MAX;
				}
			res.push_back(num[0] % 2);
			num[0] /= 2;
			while (num.size() > 0 and *(num.end() - 1) == 0)
					num.pop_back();
		}
		return res;
	}

private:
	vector<int>* get_number_ptr() {
		return &number;
	}

	void delete_zeros() {
		int i = number.size() - 1;
		for (; number[i] == 0 && i > 0; i--);
		number.resize(i + 1);
	}
	void pop_digits(int first, int last) {
		number.erase(number.begin() + first, number.end() + last);
	}

	//OPERATORS_METHODS:
private:
	static bigint sum(bigint bi1, bigint bi2) {

		vector<int>* num1p = bi1.get_number_ptr();
		vector<int>* num2p = bi2.get_number_ptr();

		vector<int> greater_num = num1p->size() >= num2p->size() ? bi1.get_number(0) : bi2.get_number(0);
		int min_len = min(num1p->size(), num2p->size());
		int max_len = max(num1p->size(), num2p->size());
		vector<int> num;
		num.resize(max_len);

		int overflow = 0;

		for (int i = 0; i < min_len; i++) {
			if (INT_MAX - (*(num1p))[i] - overflow >= (*(num2p))[i]) {
				num[i] = (*num1p)[i] + (*num2p)[i] + overflow;
				overflow = 0;
			}
			else {
				num[i] = (*num1p)[i] - INT_MAX + (*num2p)[i] + overflow;
				overflow = 1;
			}
		}
		for (int i = min_len; i < max_len; i++) {
			num[i] = greater_num[i] + overflow;
			if (num[i] == INT_MAX) {
				num[i] = 0;
				overflow = 1;
			}
			else
				overflow = 0;
		}
		if (overflow)
			num.push_back(1);

		bigint bi(num, bi1.sign, 0);
		return bi;
	}
	static bigint diff(bigint bi1, bigint bi2) {

		bi2.sign = -bi2.sign;

		int sgn;
		vector<int>* num1p;
		vector<int>* num2p;

		if (bi1 >= bi2) {
			sgn = 1;
			num1p = bi1.get_number_ptr();
			num2p = bi2.get_number_ptr();
		}
		else {
			sgn = -1;
			num1p = bi2.get_number_ptr();
			num2p = bi1.get_number_ptr();
		}

		vector<int>* greater_num = num1p->size() >= num2p->size() ? num1p : num2p;
		int min_len = min(num1p->size(), num2p->size());
		int max_len = max(num1p->size(), num2p->size());
		vector<int> num;
		num.resize(max_len);

		int underflow = 0;

		for (int i = 0; i < min_len; i++) {
			num[i] = ((*num1p)[i] - (*num2p)[i]) - underflow;
			if (num[i] < 0) {
				num[i] += INT_MAX;
				underflow = 1;
			}
			else
				underflow = 0;
		}
		for (int i = min_len; i < max_len; i++) {
			num[i] = (*num1p)[i] - underflow;
			if (num[i] < 0) {
				num[i] = INT_MAX - num[i];
				underflow = 1;
			}
			else
				underflow = 0;
		}

		bigint bi(num, sgn, 0);
		bi.delete_zeros();
		return bi;
	}
	static tuple<bool, bool, bool, bool, bool, bool> comparison_boolean(bigint bi1, bigint bi2) {

		vector<int>* num1p = bi1.get_number_ptr();
		vector<int>* num2p = bi2.get_number_ptr();

		pair<bool, bool> vls(0, 0);


		if (num1p->size() == num2p->size() && bi1.sign == bi2.sign) {
			int len = num1p->size();
			for (int i = len - 1; i >= 0; i--)
				if ((*num1p)[i] > (*num2p)[i]) {
					vls.first = 1;
					break;
				}
				else if ((*num1p)[i] < (*num2p)[i]) {
					vls.second = 1;
					break;
				}

		}

		return make_tuple(
			bi1.sign > bi2.sign,
			bi1.sign < bi2.sign,
			num1p->size() > num2p->size(),
			num1p->size() < num2p->size(),
			vls.first,
			vls.second
		);
	}
	//------------------------
	/*
	void eq_size(bigint *bi1, bigint *bi2) {
		if (bi1->get_size() >= bi2->get_size()) {
			int len = bi1->get_size() - bi2.get_size();
			for (int i = 0; i < len; i++)
				bi2->get_number_ptr()->push_back()
		}
		else
			add_zeros(bi1, bi2.get_size());


		int num_of_zeros = len - bi.get_size();
		for (int i = 0; i < num_of_zeros; i++)
			bi->get_num_ptr()->push_back(0);
	}
	*/
	//-------------------------
	static constexpr int exps[31] = {
		1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536,
		131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864,
		134217728, 268435456, 536870912, 1073741824
	};
	static constexpr long long exps_long[63] = {
		1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072,
		262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728,
		268435456, 536870912, 1073741824, 2147483648, 4294967296, 8589934592, 17179869184, 34359738368,
		68719476736, 137438953472, 274877906944, 549755813888, 1099511627776, 2199023255552,
		4398046511104, 8796093022208, 17592186044416, 35184372088832, 70368744177664, 140737488355328,
		281474976710656, 562949953421312, 1125899906842624, 2251799813685248, 4503599627370496,
		9007199254740992, 18014398509481984, 36028797018963968, 72057594037927936, 144115188075855872,
		288230376151711744, 576460752303423488, 1152921504606846976, 2305843009213693952, 4611686018427387904
	};
	/*
	static vector<vector<int>> exp_sum_split(bigint bi) {

	}
	static vector<int> exp_sum_split(int num) {
		for (int e = 30; e <= 0; e++)
			if ()
	}
	*/
	static bigint exps_multiplexer(vector<int> ea, vector<int> eb) {
		int grt = ea.size();
		int lwr = eb.size();
		bigint res;


		for (int b = 0; b < lwr; b++) {
			vector<int> num;
			int overflow = 0;
			for (int i = 0; i < b; i++)
				num.push_back(0);
			for (int a = 0; a < grt; a++) {
				if (ea[a] + eb[b] > 30) {
					int over = overflow;
					overflow = exps[ea[a] + eb[b] - 31];
					num.push_back(over + overflow);
				}
				else {
					if (ea[a] >= 0 && eb[b] >= 0) {
						if (INT_MAX - exps[ea[a] + eb[b]] > overflow) {
							num.push_back(exps[ea[a] + eb[b]] + overflow);
							overflow = 0;
						}
						else {
							num.push_back(exps[ea[a] + eb[b]] - INT_MAX + overflow);
							overflow = 1;
						}
					}
					else {
						num.push_back(overflow);
						overflow = 0;
					}
				}
			}
			if (overflow)
				num.push_back(overflow);
			res = res + bigint(num, 1, 0);
		}
		return res;
	}
	//works xx times slower than the regular one lol
	static bigint fast_multiplication(bigint A, bigint B) {

		vector<int>* a = A.get_number_ptr();
		vector<int>* b = B.get_number_ptr();
		int a_len = a->size();
		int b_len = b->size();


		vector<vector<int>> ea, eb;

		for (int e = 30; e >= 0; e--) {
			vector<int> ea_e;
			for (int i = 0; i < a_len; i++)
				if ((*a)[i] >= bigint::exps[e]) {
					ea_e.push_back(e);
					(*a)[i] -= bigint::exps[e];
				}
				else
					ea_e.push_back(-1);
			ea.push_back(ea_e);
		}

		for (int e = 30; e >= 0; e--) {
			vector<int> eb_e;
			for (int i = 0; i < b_len; i++)
				if ((*b)[i] >= bigint::exps[e]) {
					eb_e.push_back(e);
					(*b)[i] -= bigint::exps[e];
				}
				else
					eb_e.push_back(-1);
			eb.push_back(eb_e);
		}
		bigint res;

		vector<int> mone_a;
		vector<int> mone_b;
		for (int i = 0; i < a_len; i++)
			mone_a.push_back(-1);
		for (int i = 0; i < b_len; i++)
			mone_b.push_back(-1);

		for (auto&& e_a : ea)
			for (auto&& e_b : eb)
				if (e_a != mone_a && e_b != mone_b) {
					//cout << e_a[0] << " | " << e_b[0] << endl;
					if (a_len >= b_len)
						res = res + exps_multiplexer(e_a, e_b);
					else
						res = res + exps_multiplexer(e_b, e_a);
				}
		//cout << endl;
		res.delete_zeros();
		return res;
	}
	static bigint multiplication(bigint A, bigint B) {
		
		vector<int>* great = A.get_size() >= B.get_size() ?
			A.get_number_ptr() : B.get_number_ptr();
		vector<int>* low = A.get_size() >= B.get_size() ?
			B.get_number_ptr() : A.get_number_ptr();

		int gr_len = (*great).size();
		int lw_len = (*low).size();

		bigint res;

		for (int l = 0; l <lw_len; l++) {
			vector<int> num;
			long long overflow = 0;
			for (int i = 0; i < l; i++)
				num.push_back(0);
			for (int g = 0; g < gr_len; g++) {
				long long mres = (long long)(*low)[l] * (long long)(*great)[g] + overflow;
				overflow = mres / (long long)INT_MAX;
				num.push_back((int)(mres % INT_MAX));
			}
			if ((int)overflow)
				num.push_back((int)overflow);
			res = res + bigint(num, 1, 0);
		}
		return res;
	}
	static vector<int> exp_decomposition_int(int num) {
		vector<int> res;
		for (int i = 30; i >= 0; i--)
			if (num >= exps[i]) {
				res.push_back(i);
				num -= exps[i];
			}
		return res;
	}
	static bigint fast_pow(bigint bi, int exp) {

		if (bi == bigint(0) && exp == 0)
			throw "BIGINT_EXCEPT: 0^0 except on fast_pow(bigint, int)";

		if (exp == 0)
			return bigint(1);

		bigint res(1);

		vector<int> decomp = exp_decomposition_int(exp);
		bigint calced_exps[31];
		calced_exps[0] = bi;
		int max_exp = decomp[0];
		for (int i = 1; i <= max_exp; i++)
			calced_exps[i] = calced_exps[i - 1] * calced_exps[i - 1];
		for (auto&& e : decomp)
			res = res * calced_exps[e];
		return res;
	}
	static vector<int> exp_decomposition_int(long long num) {
		vector<int> res;
		for (int i = 62; i >= 0; i--)
			if (num >= exps_long[i]) {
				res.push_back(i);
				num -= exps_long[i];
			}
		return res;
	}
	static bigint fast_pow(bigint bi, long long exp) {

		if (bi == bigint(0) && exp == 0)
			throw "BIGINT_EXCEPT: 0^0 except on fast_pow(bigint, long long)";

		if (exp == 0)
			return bigint(1);

		bigint res(1);

		vector<int> decomp = exp_decomposition_int(exp);
		bigint calced_exps[63];
		calced_exps[0] = bi;
		int max_exp = decomp[0];
		for (int i = 1; i <= max_exp; i++)
			calced_exps[i] = calced_exps[i - 1] * calced_exps[i - 1];
		for (auto&& e : decomp)
			res = res * calced_exps[e];
		return res;
	}
	static vector<int> exp_decomposition_bigint(bigint exp) {
		//find MAX_POW:
		return vector<int>(0);
		
	}
	static bigint fast_pow(bigint bi, bigint exp) {
		if (bi == bigint(0) && exp == bigint(0))
			throw "BIGINT_EXCEPT: 0^0 except on fast_pow(bigint, bigint)";
		if (exp == bigint(0))
			return bigint(1);
		int n = 0;
		vector<bigint> _exps = { 1 };
		for (bigint e(1); e <= exp; n++, _exps.push_back(e), e = e * bigint(2));
		n--;
		int _exps_size = _exps.size();
		vector<bigint> calced_exps(_exps_size);
		calced_exps[0] = bigint(1);
		calced_exps[1] = bi;

		for (int i = 2; i < _exps_size; i++)
			calced_exps[i] = calced_exps[i - 1] * calced_exps[i - 1];
		/*
		for (vector<bigint>::const_iterator iter = calced_exps.begin() + 1; iter != calced_exps.end(); iter++) {
			replace(iter, iter + 1, bigint(0), *(iter - 1) * *(iter - 1));
		}
		*/

		bigint res(1);
		int counter = n;
		for (int i = _exps_size - 1; i >= 0; i--)
			if (exp >= _exps[i]) {
				res = res * calced_exps[i];
				exp = exp - _exps[i];
			}
		return res;
		/*
		for (vector<bigint> ::const_iterator iter = _exps.end() - 1; exp != bigint(); iter--, counter--)
			if (exp >= *iter) {
				used_exps.push_back(counter);
				exp = exp - *iter;
			}
			*/
	}
	static int div_binsearch(bigint *bi2, int _q, bigint *r, int shift) {
		bigint check_bi = *bi2 * bigint(_q);
		
		if (check_bi > *r)
			return div_binsearch(bi2, _q - shift, r, (int)ceil((double)shift / 2));
		else if (check_bi <= *r - *bi2)
			return div_binsearch(bi2, _q + shift, r, (int)ceil((double)shift / 2));
		else
			return _q;
	}
	static pair<bigint, bigint> division(bigint bi1, bigint bi2) {
		if (bi1 < bi2)
			return pair <bigint, bigint>(0, bi1);

		vector<int> numr = bi1.get_number();
		vector<int> domr = bi2.get_number();

		int numr_size = bi1.get_size();
		int domr_size = bi2.get_size();

		vector<int> q;
		vector<int> r;

		vector<int>::const_iterator iter = numr.begin();
		char flag = 1;
		while (iter != numr.end()) {
			for (; bigint(r) < bi2; iter++) {
				if (iter == numr.end()) {
					flag = 0;
					break;
				}
				r.push_back(*iter);
			}

			long long a = 0;
			int _q = 0;
			if (r[0] < domr[0]) {
				a = (long long)INT_MAX * (long long)r[0] + (long long)r[1];
				_q = (int)(a / (long long)domr[0]);
			}
			else
				_q = r[0] / domr[0];
			bigint _r = bigint(r);
			_q = div_binsearch(&bi2, _q, &_r, (int)ceil((double)_q / 2));
			q.push_back(_q);
			r = (bigint(r) - (bigint(_q) * bi2)).get_number();
		}
		return pair<bigint, bigint>(bigint(q), bigint(r));
	}

	public:
	static bigint generate_Marsen() {
		bigint M;
		srand(time(nullptr));
		int num = rand() % primes.size();
		M = (bigint(2) ^ num) - bigint(1);
		return M;
	}

	friend bigint operator + (bigint bi1, bigint bi2);
	friend bigint operator - (bigint bi1, bigint bi2);
	friend bigint operator - (const bigint& bi);
	friend bigint operator * (bigint bi1, bigint bi2);
	friend bigint operator ^ (bigint bi, int exp);
	friend bigint operator ^ (bigint bi, long long exp);
	friend bigint operator ^ (bigint bi, bigint exp);
	friend pair<bigint, bigint> operator / (bigint bi1, bigint bi2);
	friend bool operator > (bigint bi1, bigint bi2);
	friend bool operator < (bigint bi1, bigint bi2);
	friend bool operator == (bigint bi1, bigint bi2);
	friend bool operator != (bigint bi1, bigint bi2);
	friend bool operator >= (bigint bi1, bigint bi2);
	friend bool operator <= (bigint bi1, bigint bi2);

	friend void include_primes();
};



bigint operator + (bigint bi1, bigint bi2) {

	if (bi1.sign != bi2.sign) {
		if (bi1.sign == 1) {
			bi2.sign = 1;
			return bi1 - bi2;
		}
		else {
			bi1.sign = 1;
			return bi2 - bi1;
		}
	}
	else
		return bigint::sum(bi1, bi2);
}
bigint operator - (bigint bi1, bigint bi2) {

	bi2.sign = -bi2.sign;

	if (bi1.sign == bi2.sign)
		return bi1 + bi2;
	else {
		if (bi1.sign == 1)
			return bigint::diff(bi1, bi2);
		else
			return bigint::diff(bi2, bi1);
	}
}
bigint operator - (const bigint& bi) {
	bigint res = bi;
	res.sign = -1;
	return res;
}
bigint operator * (bigint bi1, bigint bi2) {
	char sgn = 1;
	if (bi1.sign == bi2.sign)
		return bigint::multiplication(bi1, bi2);
	else
		return bigint(0) - bigint::multiplication(bi1, bi2);
}
bigint operator ^ (bigint bi, int exp) {
	return bigint::fast_pow(bi, exp);
}
bigint operator ^ (bigint bi, long long exp) {
	return bigint::fast_pow(bi, exp);
}
bigint operator ^ (bigint bi, bigint exp) {
	return bigint::fast_pow(bi, exp);
}
pair<bigint, bigint> operator / (bigint bi1, bigint bi2) {
	if (bi1 == bigint(0))
		return pair<bigint, bigint>(bigint(0), bigint(0));
	else if (bi2 == bigint(0))
		throw "BIGINT_EXCEPT: division by zero";
	else
		return bigint::division(bi1, bi2);
}

bool operator > (bigint bi1, bigint bi2) {
	tuple<bool, bool, bool, bool, bool, bool> x = bigint::comparison_boolean(bi1, bi2);
	return !get<1>(x) and !get<3>(x) and !get<5>(x) and (get<0>(x) or get<2>(x) or get<4>(x));
	//return get<0>(x) || (!get<1>(x) && get<2>(x)) || (!get<3>(x) && get<4>(x));
}
bool operator < (bigint bi1, bigint bi2) {
	tuple<bool, bool, bool, bool, bool, bool> x = bigint::comparison_boolean(bi1, bi2);
	return !get<0>(x) and !get<2>(x) and !get<4>(x) and (get<1>(x) or get<3>(x) or get<5>(x));
	//return get<1>(x) || (!get<0>(x) && get<3>(x)) || (!get<2>(x) && get<5>(x));
}
bool operator == (bigint bi1, bigint bi2) {
	tuple<bool, bool, bool, bool, bool, bool> x = bigint::comparison_boolean(bi1, bi2);
	return !get<0>(x) and !get<1>(x) and !get<2>(x) and !get<3>(x) and !get<4>(x) and !get<5>(x);
}
bool operator != (bigint bi1, bigint bi2) {
	tuple<bool, bool, bool, bool, bool, bool> x = bigint::comparison_boolean(bi1, bi2);
	return get<0>(x) or get<1>(x) or get<2>(x) or get<3>(x) or get<4>(x) or get<5>(x);
}
bool operator >= (bigint bi1, bigint bi2) {
	tuple<bool, bool, bool, bool, bool, bool> x = bigint::comparison_boolean(bi1, bi2);
	//cout << get<0>(x) << get<1>(x) << get<2>(x) << get<3>(x) << get<4>(x) << get<5>(x) << endl;
	return !get<1>(x) and !get<3>(x) and !get<5>(x);
}
bool operator <= (bigint bi1, bigint bi2) {
	tuple<bool, bool, bool, bool, bool, bool> x = bigint::comparison_boolean(bi1, bi2);
	return !get<0>(x) and !get<2>(x) and !get<4>(x);
}

constexpr int bigint::exps[];
constexpr long long bigint::exps_long[];
vector<int> bigint::primes;

void include_primes() {
	ifstream rd;
	string str = "";
	int counter = 0;

	rd.open("primes.txt");
	if (rd.is_open()) {
		while (getline(rd, str, ' ')) {
			bigint::primes.push_back(atoi(str.c_str()));
			counter++;
		}
	}
	else
		cout << "IOExcept" << endl;
}