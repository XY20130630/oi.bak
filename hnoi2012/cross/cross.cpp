#include <bits/stdc++.h>

typedef long long LL;

#define FOR(i, a, b) for (LL i = (a), i##_END_ = (b); i <= i##_END_; i++)
#define DNF(i, a, b) for (LL i = (a), i##_END_ = (b); i >= i##_END_; i--)

template <typename Tp> void in(Tp &x) {
	char ch = getchar(); x = 0;
	while (ch < '0' || ch > '9') ch = getchar();
	while (ch >= '0' && ch <= '9') x = x * 10 + ch - '0', ch = getchar();
}

template <typename Tp> Tp Min(Tp x, Tp y) {return x < y ? x : y;}
template <typename Tp> Tp Max(Tp x, Tp y) {return x > y ? x : y;}
template <typename Tp> Tp chkmax(Tp &x, Tp y) {return x > y ? x : x=y;}
template <typename Tp> Tp chkmin(Tp &x, Tp y) {return x < y ? x : x=y;}

using std::vector;

const LL MAXN = 10010, MOD = 1000000009;

vector<LL>line[MAXN];
vector<LL>lr[MAXN], tp[MAXN], co[MAXN];

LL r, c, n;

struct Node {
	Node *ch[2], *fa;
	LL data1, data2, sum1, sum2, sum3, sz;

	void clear();
	void update();
	void rotate();
	void insert(LL, LL);
	LL get_rnk(LL);
	void splay(Node*);
	Node *find_key(LL);
} *treap;

void Node::rotate()
{
	Node *pa = fa;
	fa = pa -> fa; pa -> fa = this;
	if (fa != NULL) {
		bool t = (fa -> ch[0] == pa ? 0 : 1);
		fa -> ch[t] = this;
	}
	bool t = (pa -> ch[0] == this ? 0 : 1);
	Node *chd = ch[t ^ 1];
	ch[t ^ 1] = pa; pa -> ch[t] = chd;
	if (chd != NULL) chd -> fa = pa;
	pa -> update(); update();
}

void Node::splay(Node *top)
{
	while (fa != top) {
		if (fa -> fa != top) {
			bool t = (fa -> fa -> ch[0] == fa ? 0 : 1);
			if (fa -> ch[t] == this) fa -> rotate(), rotate();
			else rotate(), rotate();
		}
		else rotate();
	}
	if (top == NULL) treap = this;
}

LL Node::get_rnk(LL now)
{
	Node *x = this;

	LL ret = 0;
	
	while (true) {
		if (x -> data1 == now) {
			return (x -> ch[0] == NULL ? 1 : x -> ch[0] -> sz + 1) + ret;
		}
		if (x -> data1 > now) {
			if (x -> ch[0] == NULL) return ret;
			x = x -> ch[0];
		}
		else {
			if (x -> ch[1] == NULL) return x -> sz + ret;
			ret += (x -> ch[0] == NULL ? 1 : x -> ch[0] -> sz + 1);
			x = x -> ch[1];
		}
	}
}

Node* Node::find_key(LL rnk)
{
	Node *x = this;
	while (true) {
		if ((x -> ch[0] == NULL ? 1 : x -> ch[0] -> sz + 1) == rnk) return x;
		else if ((x -> ch[0] == NULL ? 1 : x -> ch[0] -> sz + 1) < rnk) {
			rnk -= (x -> ch[0] == NULL ? 1 : x -> ch[0] -> sz + 1);
			x = x -> ch[1];
		}
		else x = x -> ch[0];
	}
}

void Node::insert(LL now, LL now2)
{
	if (treap == NULL) {
		Node *hr = new Node;
		hr -> ch[0] = hr -> ch[1] = hr -> fa = NULL;
		hr -> data1 = now; hr -> data2 = now2; treap = hr; treap -> update();
		return;
	}
	LL rnk = treap -> get_rnk(now);
	if (rnk == 0) {
		Node *hr = new Node;
		hr -> ch[0] = hr -> fa = NULL;
		hr -> ch[1] = treap; hr -> data1 = now; hr -> data2 = now2;
		treap -> fa = hr; hr -> update();
		treap = hr;
	}
	else if (rnk == treap -> sz) {
		Node *hr = new Node;
		hr -> ch[1] = hr -> fa = NULL;
		hr -> ch[0] = treap; hr -> data1 = now; hr -> data2 = now2;
		treap -> fa = hr; hr -> update();
		treap = hr; 
	}
	else {
		Node *hr = new Node;
		treap -> find_key(rnk) -> splay(NULL);
		treap -> find_key(rnk + 1) -> splay(treap);
		hr -> data1 = now;
		hr -> data2 = now2;
		hr -> ch[0] = NULL;
		hr -> ch[1] = treap -> ch[1];
		hr -> fa = treap; treap -> ch[1] = hr;
		hr -> ch[1] -> fa = hr; hr -> update(); treap -> update();
	}
}

void Node::clear()
{
	if (ch[0] != NULL) ch[0] -> clear();
	if (ch[1] != NULL) ch[1] -> clear();
	delete this;
}

void Node::update()
{
	sz = 1;
	sum1 = data1 * data2 % MOD;
	sum2 = (data1 * (data1 + 1) / 2) * data2 % MOD;
	sum3 = data2;

	if (ch[0] != NULL) {
		sz += ch[0] -> sz;
		sum1 = (sum1 + ch[0] -> sum1) % MOD;
		sum2 = (sum2 + ch[0] -> sum2) % MOD;
		sum3 = (sum3 + ch[0] -> sum3) % MOD;
	}
	if (ch[1] != NULL) {
		sz += ch[1] -> sz;
		sum1 = (sum1 + ch[1] -> sum1) % MOD;
		sum2 = (sum2 + ch[1] -> sum2) % MOD;
		sum3 = (sum3 + ch[1] -> sum3) % MOD;
	}
}

int main()
{
	freopen("cross.in", "r", stdin);
	freopen("cross.out", "w", stdout);
	
	in(r); in(c); in(n);

	FOR(i, 0, r + 1) {
		lr[i].resize(c + 2);
		tp[i].resize(c + 2);
		co[i].resize(c + 2);
		line[i].resize(c + 2);
	}

	FOR(i, 1, r) FOR(j, 1, c) line[i][j] = 1;
	
	FOR(i, 1, n) {
		LL x, y;
		in(x); in(y);
		line[x][y] = 0;
	}

	FOR(i, 1, r) {
		FOR(j, 1, c) {
			if (line[i][j] == 0) continue;
			if (j == 1 || line[i][j - 1] == 0) lr[i][j] = 0;
			else lr[i][j] = lr[i][j - 1] + 1;
		}
		DNF(j, c, 1) {
			if (line[i][j] == 0) continue;
			if (j == c || line[i][j + 1] == 0) lr[i][j] = 0;
			else chkmin(lr[i][j], lr[i][j + 1] + 1);
		}

		FOR(j, 1, c) if (line[i][j]) {
			if (i == 1 || !line[i - 1][j]) tp[i][j] = 0;
			else tp[i][j] = tp[i - 1][j] + 1;
		}
	}

	DNF(i, r, 1) {
		FOR(j, 1, c) if (line[i][j]) {
			if (i == r || !line[i + 1][j]) co[i][j] = 0;
			else co[i][j] = co[i + 1][j] + 1;
		}
	}

	treap = NULL;
	bool is_cleared = true;

	LL ans = 0;
	
	FOR(j, 1, c) {
		if (!is_cleared) {
			treap -> clear();
			treap = NULL;
			is_cleared = true;
		}

		FOR(i, 1, r) {
			if (i > 2) {
				if (lr[i - 2][j] != 0 && line[i - 1][j]) {
					treap -> insert(lr[i - 2][j], tp[i - 2][j]);
					is_cleared = false;
				}
			}
			
			if (!line[i][j]) {
				if (!is_cleared) {
					treap -> clear();
					treap = NULL;
					is_cleared = true;
				}
				continue;
			}

			LL now = lr[i][j];
			
			if (now && treap != NULL) {
				LL rnk = treap -> get_rnk(now);
				if (rnk != 0) {
					Node *hr;
					if (rnk != treap -> sz) {
						treap -> find_key(rnk + 1) -> splay(NULL);
						treap -> find_key(rnk) -> splay(treap);
						hr = treap -> ch[0];
					}
					else {
						treap -> find_key(rnk) -> splay(NULL);
						hr = treap;
					}
					LL fst = hr -> sum1 * now % MOD * co[i][j] % MOD;
					LL sec = hr -> sum2 * co[i][j] % MOD;
					ans = (ans + fst - sec) % MOD;
				}

				if (rnk != treap -> sz) {
					Node *hr;
					if (rnk != 0) {
						treap -> find_key(rnk) -> splay(NULL);
						treap -> find_key(rnk + 1) -> splay(treap);
						hr = treap -> ch[1];
					}
					else {
						treap -> find_key(rnk + 1) -> splay(NULL);
						hr = treap;
					}
					LL valu =
						(now * now - now * (now + 1) / 2) * co[i][j] % MOD;
					ans = (ans + hr -> sum3 * valu % MOD) % MOD;
				}
			}
		}
	}

	printf("%lld\n", ans);
		
	return 0;
}
