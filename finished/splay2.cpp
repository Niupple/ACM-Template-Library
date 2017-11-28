/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem: splay spaly ����ƽ����ά��������Ϣ�������ʶ���������չ������ת��Ϣά��������뵥��ɾ���ϲ��ظ����ǰ�����Һ�̲����������ҵ�k���kС������ѯ��ͨƽ����bzoj 3224
 * BUAA��ѧ��!
 * ����᳹��ʵ��ʱ���й���ɫ�������˼�룬Ϊ��ʤȫ�潨��С��������bug��
 * Thoroughly apply the Thinking of Socialism with Chinese Characteristics in the New Era, fight for accomplishing a broad wealthy society to reduce bugs!
 * */
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>
#include <climits>
#include <cstring>
#include <cassert>
#define NAME "bzoj3224"
#define read(x) scanf("%d", &x)
#ifdef DEBUG
#	define DP(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#	define DP(fmt, ...) 0
#endif

using namespace std;

//����������
struct tree {
	int v, cnt, size;	//cntָ��ǰ�����ظ�������sizeָ��ǰ������cnt�ĺ�
	tree *father, *son[2];
} T[100005];	//�ڴ��

int N, M, cnt;
tree *root, *nil;	//root�����̶���nil�ǹ̶��Ŀսڵ㣬��cnt��size����0

tree *new_tree() {
	return T+cnt++;
}

//��ת֮�󣬶�������Ϣ����ά��
void maintain(tree *x) {
	x->size = x->son[0]->size + x->son[1]->size + x->cnt;	//ע�����ﲻ�Ǽ�һ�����Ǽ��ϵ�ǰ����cntֵ
}

//��ת��������x���y�ĸ���
void rotate(tree *x, tree *y) {
	int c;
	x->father = y->father;
	if(y->father != nil) {
		tree *z = y->father;
		z->son[z->son[1] == y] = x;
	}
	c = y->son[1] == x;
	y->son[c] = x->son[c^1];
	x->son[c^1]->father = y;
	y->father = x;
	x->son[c^1] = y;
	if(root == y) {		//ע��ά��root��ָ��
		root = x;
	}
	//ע����ά�����ڵĶ��ӣ�y������Ϣ����ά�����ף�x������Ϣ���Ե�����ά����Ϣ��
	maintain(y);
	maintain(x);
}

//splay��������x��ת����
void splay(tree *x) {
	while(x != nil && x != root) {	//root�Ǳ߽磬nil������
		tree *y = x->father;
		if(y == root) {			//����˫���Ľ�㣬����
			rotate(x, y);
		} else {				//˫����splay���ĸ��Ӷȱ�֤
			tree *z = y->father;
			int c, d;
			c = y->son[1] == x;
			d = z->son[1] == y;
			if(c == d) {
				rotate(y, z);
				rotate(x, y);
			} else {
				rotate(x, y);
				rotate(x, z);
			}
		}
	}
}

//�������
tree *insert(int x) {
	tree *now = root, *fat = nil, *ret = nil;
	while(now != nil) {
		fat = now;
		if(now->v >= x) {
			ret = now;
		}
		now = now->son[now->v < x];
	}
	//fat�ҵ��ľ��Ǵ�����λ�õĸ��ף�now�Ѿ����������nil��
	if(ret->v == x) {
		ret->cnt++;
	} else {
		ret = new_tree();
		ret->father = fat;
		ret->son[0] = ret->son[1] = nil;	//��ʼ������ret��ָ��ָ����ȷ�ķ���
		ret->v = x;
		ret->size = 1;
		ret->cnt = 1;
		if(fat == nil) {
			root = ret;
		} else {
			fat->son[x > fat->v] = ret;
		}
	}
	splay(ret);		//ע�⽫retת����
	return ret;
}

//����ǽ�x������tree��ɾ��
void clear(tree *x) {
	tree *now = x, *s, *y = x->father;
	if(x->son[0] == nil || x->son[1] == nil) {	//���x�Ķ��Ӳ���
		s = x->son[x->son[0] == nil];
		y->son[y->son[1] == x] = s;
		s->father = y;
		if(x == root) {
			root = s;
		}
		splay(y);			//��Ϣ�ƻ�����y������s�����s��nil��splay(s)���޷�������Ϣ
	} else {	//���x���������Ӷ���
		now = x->son[1];
		while(now->son[0] != nil) {
			now = now->son[0];
		}
		now->son[0] = x->son[0];
		x->son[0]->father = now;
		x->son[1]->father = y;
		if(y != nil) {		//�����������rotate�е��жϣ�����y�Ƿ���nil
			y->son[y->son[1] == x] = x->son[1];
		}
		if(x == root) {		//ֻҪ�漰�����޸ģ���Ҫ����root����
			root = x->son[1];
		}
		splay(now);			//splay�������Ǹ��Ӷȵı�֤���������size��Ϣ�ĸ����Ǳ�Ҫ��
	}
}

//�ܵ�ɾ�����������������ɾ���ļ�ֵ
void remove(int x) {
	tree *now = root;
	while(now != nil) {
		if(now->v == x) {
			if(now->cnt > 0) {			//��������У�³���Եı���
				if(--now->cnt == 0) {	//��cnt==0ʱ�����뽫now�������Ƴ���������ҵĶ����Խ�ɥʧ������
					clear(now);
					return;
				}
			}
			break;	//���ﲻ��return����Ϊnow���������cnt�ı仯������Ӧ��ͨ��splay������Ϣά��
		} else {
			now = now->son[x > now->v];
		}
	}
	splay(now);
}

//��ѯx�������е�����
int rank_of(int x) {
	tree *now = root;
	int ret = 0;
	while(now != nil) {
		if(now->v == x) {	//�ҵ���
			ret += now->son[0]->size+1;
			splay(now);		//ע����ת
			break;
		} else if(now->v < x) {
			ret += now->cnt+now->son[0]->size;
		}
		now = now->son[now->v < x];
	}
	//�Ҳ����Ͳ�ת����
	return ret;
}

//��ѯ����Ϊx�Ľ��
tree *ranked(int x) {
	tree *now = root, *ret = nil;
	while(now != nil && x > 0) {
		if(now->son[0]->size < x) {
			ret = now;
			x -= now->son[0]->size + now->cnt;
			now = now->son[1];		//ע�����ﲻ�ܺ���һ��д��x��if����з����˸ı䣡����
		} else {
			now = now->son[0];
		}
	}
	splay(ret);
	return ret;
}

//��x��ǰ��
tree *pred(int x) {
	tree *now = root, *ret = nil;
	while(now != nil) {
		if(now->v < x && now->cnt > 0) {
			ret = now;
		}
		now = now->son[now->v < x];
	}
	splay(ret);
	return ret;
}

//��x�ĺ��
tree *succ(int x) {
	tree *now = root, *ret = nil;
	while(now != nil) {
		if(now->v > x && now->cnt > 0) {
			ret = now;
		}
		now = now->son[now->v <= x];
	}
	splay(ret);
	return ret;
}

//���������ڵ��Ե�splay����ĺ���
#ifdef DEBUG
void print(tree *t) {
	if(t == nil) {
		return;
	}
	print(t->son[0]);
	printf("	v:%d size:%d cnt:%d\n", t->v, t->size, t->cnt);
	print(t->son[1]);
}
#endif

//ע�⽫niu����ã����ҽ�ָ�붼ָ�����õĿռ�
void preprocess() {
	nil = new_tree();
	nil->son[0] = nil->son[1] = nil->father = nil;
	nil->size = nil->cnt = 0;
	root = nil;
}

void process() {
	int a, c;
	scanf("%d", &N);
	for(int i = 1; i <= N; ++i) {
		scanf("%d%d", &c, &a);
		if(c == 1) {
			insert(a);
		} else if(c == 2) {
			remove(a);
		} else if(c == 3) {
			printf("%d\n", rank_of(a));
		} else if(c == 4) {
			printf("%d\n", ranked(a)->v);
		} else if(c == 5) {
			printf("%d\n", pred(a)->v);
		} else if(c == 6) {
			printf("%d\n", succ(a)->v);
		}
	}
}

int main(int argc, char **argv) {
	freopen(NAME".in", "r", stdin);
#ifdef DEBUG
	freopen(NAME".out", "w", stdout);
#endif

	preprocess();
	process();

	return 0;
}
 
 
