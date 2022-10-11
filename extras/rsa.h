#include "bignum.h";

class rsa_client {

private:
	bigint p, q;
	bigint n;
	bigint euler_n;
	bigint e;
	bigint d;

	bool can_decrypt;

	vector<int> e_bin;
	vector<int> d_bin;


private:
	bigint gcd(bigint a, bigint b, bigint& x, bigint& y) {
		if (a == bigint(0)) {
			x = bigint(0); y = bigint(1);
			return b;
		}
		bigint x1, y1;
		pair<bigint, bigint> t = b / a;
		bigint d = gcd(t.second, a, x1, y1);
		x = y1 - t.first * x1;
		y = x1;
		return d;
	}

public:

	rsa_client() {
		include_primes();

		p = 17;//bigint::generate_Marsen();
		q = 23;//bigint::generate_Marsen();

		n = p * q;

		euler_n = (p - bigint(1)) * (q - bigint(1));

		//mb add random fermat?
		int Fermat_primes[3] = { 17, 257, 65537 };
		srand(time(nullptr));
		e = bigint(Fermat_primes[rand() % 3]);
		e_bin = e.get_binary();
		reverse(e_bin.begin(), e_bin.end());

		//d = euler_n - (euler_n / e).first;
		//d_bin = d.get_binary();
		bigint y;
		gcd(e, euler_n, d, y);
		d_bin = d.get_binary();
		reverse(d_bin.begin(), d_bin.end());

		can_decrypt = true;
	};
	rsa_client(pair<bigint, bigint> e_n) {
		e = e_n.first;
		n = e_n.second;
		can_decrypt = false;
	};

	pair<bigint, bigint> get_public_key() {
		return pair<bigint, bigint>(e, n);
	}

	string encrypt(string message) {
		int message_iterator = 0;
		int m_len = message.length();
		int n_size = n.get_size();
		vector<int> num;
		int base = 128;

		while (message_iterator < m_len) {
			long long tmp_digit = 0;
			int digit = 0;
			for (int i = 0; message_iterator < m_len; i++, message_iterator++) {
				tmp_digit += ((char)message[message_iterator])*(long long)pow(base, i);
				if (tmp_digit <= INT_MAX)
					digit = tmp_digit;
				else {
					message_iterator--;
					break;
				}
			}
			num.push_back(digit);
		}

		bigint m(num, 1, 0);
		bigint m_i = m;

		for (vector<int>::const_iterator iter = e_bin.begin(); iter != e_bin.end(); iter++) {
			m_i = ((((m_i * m_i) * (m ^ *iter))) / n).second;
		}
		return m_i.get_string(0);
	}

	string decrypt(string message) {
		if (!can_decrypt)
			throw "RSA_EXCEPT: this client can't decrypt!";

		bigint m(message, 0);
		bigint m_i = m;

		for (vector<int>::const_iterator iter = d_bin.begin(); iter != d_bin.end(); iter++) {
			m_i = ((((m_i * m_i) * (m ^ *iter))) / n).second;
		}
		//m_i.print_me();
		vector<int> num = m_i.get_number();

		string res = "";
		int num_len = num.size();
		int base = 128;

		for (int i = 0; i < num_len; i++) {
			for (int j = 0; num[i] != 0; j++) {
				int pw = (int)pow(base, j);
				res += (char)(num[i] % pw);
				num[i] /= pw;
			}
		}

		return res;
	}
};