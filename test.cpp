#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

double a = 1.0;
double b = 100.0;
double epsilon = 1e-6;

// 原问题中的函数 f(x)
double f(double x) {
    double ans = 0;
    for (int i = 100; i >= 1; --i) {
        double temp = 0;
        for (int j = i; j >= 1; --j) temp += pow(x + 0.5 * j, -3.3);
        ans += sin(x + temp) / pow(1.3, i);
    }
    return ans;
}

// 随机搜索方法
double random_search(double a, double b, int iterations) {
    double best_x = a;
    double best_f = f(a);

    for (int i = 0; i < iterations; ++i) {
        double rand_x = a + static_cast<double>(rand()) / RAND_MAX * (b - a);
        double rand_f = f(rand_x);

        if (rand_f > best_f) {
            best_x = rand_x;
            best_f = rand_f;
        }
    }

    return best_x;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // 初始化随机数种子
    int iterations = 10000; // 随机搜索的迭代次数
    double max_x = random_search(a, b, iterations);

    std::cout << "After " << iterations << " iterations, the maximum value occurs at x = " << max_x << std::endl;
    std::cout << "The maximum value is f(x) = " << f(max_x) << std::endl;
    return 0;
}
