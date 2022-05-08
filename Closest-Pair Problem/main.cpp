#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
// 点
struct point {
	double x = 0.0;
	double y = 0.0;

	point() {}
	point(double x, double y)
		:x(x), y(y) {}
	point(const point& old)
		:x(old.x), y(old.y) {}

	point& operator=(const point& right) {
		x = right.x;
		y = right.y;
		return *this;
	}
};

// 点的比较：按x值
bool CmpPointByX(const point& right, const point& left) {
	return right.x < left.x;
}

// 点的比较：按y值
bool CmpPointByY(const point& right, const point& left) {
	return right.y < left.y;
}

// 两点的距离平方
double inline DistanceSquare(const point& p1, const point& p2) {
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// 最邻近点对距离平方
double ClosestPair(point* Px, point* Py, int begin, int end) {
	int size = end - begin + 1;									// 点集的模
	// (终点) ---> 0 <= size <= 3
	if (size <= 1) {
		return 200000000.0;
	}
	else if (size == 2) {
		return DistanceSquare(Px[begin], Px[end]);
	}
	else if (size == 3) {
		double	d1 = DistanceSquare(Px[begin], Px[begin + 1]),
			d2 = DistanceSquare(Px[begin + 1], Px[begin + 2]),
			d3 = DistanceSquare(Px[begin + 2], Px[begin]);
		if (d1 <= d2 && d1 <= d3)
			return d1;
		else if (d2 <= d3)
			return d2;
		else
			return d3;
	}
	// (递归) ---> size > 3
	// 点集划分
	int lSize = size / 2;										// 左侧点集的模
	double mid = Px[begin + lSize - 1].x;						// 垂直分割线

	// Px划分
	for (; lSize < size; lSize++)
		if (Px[begin + lSize].x > mid)
			break;
	if (lSize == size) {										// 特殊情况：一半以上的点在垂直分割线上
		if (Px[begin].x == mid) {								// 全在一条垂直线上
			double delta = 100000000.0;
			for (int i = 0; i < size - 1; i++) {
				double d = (Py[i].y - Py[i + 1].y) * (Py[i].y - Py[i + 1].y);
				if (d < delta)
					delta = d;
			}
			return delta;
		}
		else {													// 点不全在垂直分割线上
			for (lSize = 1; lSize < size; lSize++)
				if (Px[begin + lSize].x == mid) {
					mid = Px[begin + lSize - 1].x;
					break;
				}
		}
	}

	// Py划分
	point* Ly = new point[lSize],
		* Ry = new point[size - lSize];
	for (int i = 0, j = 0, k = 0; i < size; i++)
		if (Py[i].x <= mid)
			Ly[j++] = Py[i];
		else
			Ry[k++] = Py[i];

	// 获得两边最邻近点对距离平方最小值：delta
	double d1 = ClosestPair(Px, Ly, begin, begin + lSize - 1);	// 左侧最邻近点对距离
	delete[]Ly;
	double d2 = ClosestPair(Px, Ry, begin + lSize, end);		// 右侧最邻近点对距离
	delete[]Ry;
	double delta = min(d1, d2);									// 两边最邻近点对距离最小值
	
	// 获得中央区域（M）点（按y排序）
	vector<point> My;
	for (int i = 0; i < size; i++)
		if (abs(Py[i].x - mid) < delta)
			My.push_back(Py[i]);

	// 获得中央区域最邻近点对距离平方：mDelta
	double mDelta = 200000000.0;								// 中央区域最邻近点对距离
	for (int i = 0, mSize = My.size(); i < mSize; i++) {
		for (int j = 1; j < 8 && i + j < mSize; j++) {
			double d = DistanceSquare(My[i], My[i + j]);
			if (d < mDelta) {
				mDelta = d;
			}
		}
	}
	
	// 返回最邻近点对距离
	return mDelta < delta ? mDelta : delta;
}

int main() {
	// 获取点数
	int n = 0;													// 顶点数
	cin >> n;

	// 获取点集
	point* Px = new point[n];									// 按x排序点集
	point* Py = new point[n];									// 按y排序点集
	for (int i = 0; i < n; i++) {
		cin >> Px[i].x >> Px[i].y;
		Py[i] = Px[i];
	}

	// 预处理：排序
	sort(Px, Px + n, CmpPointByX);
	sort(Py, Py + n, CmpPointByY);

	// 计算并输出
	cout << setiosflags(ios::fixed) << setprecision(2) << sqrt(ClosestPair(Px, Py, 0, n - 1));

	// 收尾
	delete[] Px;
	delete[] Py;
	return 0;
}