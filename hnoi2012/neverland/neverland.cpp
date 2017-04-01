#include <bits/stdc++.h>

typedef long long LL;

#define FOR(i, a, b) for (int i = (a), i##_END_ = (b); i <= i##_END_; i++)
#define DNF(i, a, b) for (int i = (a), i##_END_ = (b); i >= i##_END_; i--)

template <typename Tp> void in(Tp &x) {
	char ch = getchar(); x = 0;
	while (ch < '0' || ch > '9') ch = getchar();
	while (ch >= '0' && ch <= '9') x = x * 10 + ch - '0', ch = getchar();
}

template <typename Tp> Tp Min(Tp x, Tp y) {return x < y ? x : y;}
template <typename Tp> Tp Max(Tp x, Tp y) {return x > y ? x : y;}
template <typename Tp> Tp chkmax(Tp &x, Tp y) {return x > y ? x : x=y;}
template <typename Tp> Tp chkmin(Tp &x, Tp y) {return x < y ? x : x=y;}

const int MAXN = 100010;

char command[10];
int n, m, rnk[MAXN], fa[MAXN];

struct Node {
	int data, sz, id;
	Node *ch[2], *fa;

	void update();
	void splay(Node*);
	void rotate();
	Node *find_key(int);
	void insert(Node*);
} *to[MAXN];

void Node::rotate()
{
	Node *pa = fa;
	fa = pa -> fa;
	if (fa != NULL) {
		bool t = fa -> ch[0] == pa ? 0 : 1;
		fa -> ch[t] = this;
	}
	pa -> fa = this;
	bool t = pa -> ch[0] == this ? 0 : 1;
	Node *chd = ch[t ^ 1];
	pa -> ch[t] = chd;
	if (chd != NULL) chd -> fa = pa;
	ch[t ^ 1] = pa;
	pa -> update(); update();
}

void Node::splay(Node *top)
{
	while (fa != top) {
		if (fa -> fa != top) {
			bool t = (fa -> ch[0] == this ? 0 : 1);
			if (fa -> fa -> ch[t] == fa) {
				fa -> rotate(); rotate();
			}
			else rotate(), rotate();
		}
		else rotate();
	}
}

Node* Node::find_key(int rnk)
{
	Node *hr = this;
	while (true) {
		int nowrnk;
		nowrnk = (hr -> ch[0] == NULL ? 1 : hr -> ch[0] -> sz + 1);
		if (nowrnk == rnk) return hr;
		if (nowrnk <  rnk) {
			rnk -= nowrnk;
			hr = hr -> ch[1];
		}
		else hr = hr -> ch[0];
	}
}

void Node::update()
{
	sz = 1;
	if (ch[0] != NULL) sz += ch[0] -> sz;
	if (ch[1] != NULL) sz += ch[1] -> sz;
}

void Node::insert(Node *x)
{
	bool t;
	Node *pos = this, *pa = NULL;
	while (pos != NULL) {
		pa = pos;
		if (x -> data > pos -> data) pos = pos -> ch[1], t = 1;
		else pos = pos -> ch[0], t = 0;
	}
	x -> fa = pa;
	if (pa != NULL) pa -> ch[t] = x;
	x -> ch[0] = x -> ch[1] = NULL;
	while (x != NULL) {
		x -> update();
		x = x -> fa;
	}
}

void Ins(Node* now, Node* to)
{
	if (now == NULL) return;
	Ins(now -> ch[0], to); Ins(now -> ch[1], to);
	to -> splay(NULL); to -> insert(now); now -> splay(NULL);
}

int find(int x)
{
	int tmp = x, pre;
	while (tmp != fa[tmp]) tmp = fa[tmp];
	while (x != tmp) pre = fa[x], fa[x] = tmp, x = pre;
	return tmp;
}

bool merge(int x, int y)
{
	int fx = find(x), fy = find(y);
	return fx == fy ? false : fa[fx] = fy, true;
}

int main()
{
	freopen("neverland.in", "r", stdin);
	freopen("neverland.out", "w", stdout);
	
	in(n); in(m);
	FOR(i, 1, n) fa[i] = i;
	FOR(i, 1, n) in(rnk[i]);
	FOR(i, 1, n) {
		to[i] = new Node;
		to[i] -> data = rnk[i];
		to[i] -> sz = 1; to[i] -> id = i;
		to[i] -> fa = to[i] -> ch[0] = to[i] -> ch[1] = NULL;
	}
	FOR(i, 1, m) {
		int x, y; in(x); in(y);
		if (merge(x, y)) {
			if (to[x] == NULL) printf("%d\n", x);
			to[x] -> splay(NULL);
			to[y] -> splay(NULL);
			if (to[x] -> sz < to[y] -> sz) {
				Ins(to[x], to[y]);
			}
			else Ins(to[y], to[x]);
		}
	}

	int q; in(q);

	while (q--) {
		scanf("%s", command);
		if (command[0] == 'Q') {
			int x, k;
			in(x); in(k);
			to[x] -> splay(NULL);
			if (to[x] -> sz < k) printf("-1\n");
			else {
				printf("%d\n", to[x] -> find_key(k) -> id);
			}
		}
		else {
			int x, y;
			in(x); in(y);
			if (merge(x, y)) {
				to[x] -> splay(NULL);
				to[y] -> splay(NULL);
				if (to[x] -> sz < to[y] -> sz) {
					Ins(to[x], to[y]);
				}
				else Ins(to[y], to[x]);
			}
		}
	}
	
	return 0;
}
