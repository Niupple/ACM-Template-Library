/* Copyright dyj (c) 2017
 * any codes cannot be used for business
 * Problem:值域数据结构值域树状数组求逆序对单点修改区间查询BIT
 * BUAA开学季!
 * 深入贯彻落实新时代中国特色社会主义思想，为决胜全面建成小康社会减少bug！
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
#define NAME "cv1688"
#define read(x) scanf("%d", &x)
#define lowbit(x) ((x)&(-(x)))
#ifdef DEBUG
#	define DP(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#	define DP(fmt, ...) 0
#endif

using namespace std;

/*变量名
 * N		count of numbers
 * av[]		array of value
 * tree[]	BIT itself
 * M		upper bound of BIT, maximum of av[i]
 */
long long N, av[100005], tree[100005], M;

void readin() {
	cin >> N;
	for(long long i = 1; i <= N; ++i) {
		cin >> av[i];
		M = max(av[i], M);
	}
}

//树状数组的单点修改，从pos一路加到上界M
void modify(long long pos) {
	while(pos <= M) {
		tree[pos]++;	//如果是增加其他的数就加上该点的数，注意树状数组的修改必须依赖已有的值
		pos += lowbit(pos);
	}
}

//树状数组的访问，只支持前缀和访问，故要求维护性质满足减法，从pos一路减到0
//另注意访问[l,r]时，使用query(r)-query(r-1)
long long query(long long r) {
	long long ret = 0;
	while(r > 0) {
		ret += tree[r];
		r -= lowbit(r);
	}
	return ret;
}

void process() {
	long long ans = 0;
	for(long long i = 1; i <= N; ++i) {
		modify(av[i]);
		ans += i-query(av[i]);
	}
	cout << ans << endl;
}

int main() {
	freopen(NAME".in", "r", stdin);
#ifndef DEBUG
	freopen(NAME".out", "w", stdout);
#endif

	readin();
	process();

	return 0;
}
 
 
