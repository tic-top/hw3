#include <iostream>
#include <deque>
#include <chrono>
#include <cmath>
#include "f.h"

using namespace std::chrono;
using namespace std;


int NUM_IT = 100;
double MAX = 0;

void ini_deque(deque<pair<pair<double, double>, pair<double, double>> >& pairs, double a, double b, int worker_num) {
    double interval = (b - a) / worker_num;
    double prev = a;
    double cur = a + interval;
    for (int i = 0; i < worker_num; i++) {
        pairs.push_back({{prev, f(prev)}, {cur, f(cur)}});
        prev = cur;
        cur += interval;
    }
}

int main() {

    int worker_num = 0;
    double min_interval = epsilon / s;
    deque<pair<pair<double, double>, pair<double, double>> >  pairs;

    #pragma omp parallel
    {
        #pragma omp critical(dataupdate)
        {
            worker_num++;
        }
    }
    auto start = high_resolution_clock::now();
    ini_deque(pairs, a, b, worker_num);
    MAX = max(f(a), f(b));
    while (!pairs.empty()) {
        int num_responsibility = ceil((double)pairs.size() / (double)worker_num);

        #pragma omp parallel
        {
            deque<pair<pair<double, double>, pair<double, double>>> pairs_tmp;

            #pragma omp for schedule(dynamic) 
            for (int i = 0; i < num_responsibility; i++) {
                if (!pairs.empty()) {
                    #pragma omp critical
                    {
                        if (!pairs.empty()) {
                            pairs_tmp.push_back(pairs.front());
                            pairs.pop_front();
                        }
                    }
                }
            }

            for (int iter = 0; iter < NUM_IT && !pairs_tmp.empty(); iter++) {
                auto cur_pair = pairs_tmp.back();
                pairs_tmp.pop_back();
                double tmp_max = max(cur_pair.first.second, cur_pair.second.second);

                #pragma omp critical
                {
                    MAX = max(MAX, tmp_max);
                }

                if ((cur_pair.second.first - cur_pair.first.first) / 2 >= min_interval) {
                    double mid_point = (cur_pair.first.first + cur_pair.second.first) / 2;
                    double g_mid_point = f(mid_point);

                    double possible_max_left = (cur_pair.first.second + g_mid_point + s * (mid_point - cur_pair.first.first)) / 2;
                    if (possible_max_left >= MAX + epsilon) {
                        pairs_tmp.push_back({cur_pair.first, {mid_point, g_mid_point}});
                    }

                    double possible_max_right = (g_mid_point + cur_pair.second.second + s * (cur_pair.second.first - mid_point)) / 2;
                    if (possible_max_right >= MAX + epsilon) {
                        pairs_tmp.push_back({{mid_point, g_mid_point}, cur_pair.second});
                    }
                }
            }

            #pragma omp critical
            {
                pairs.insert(pairs.end(), pairs_tmp.begin(), pairs_tmp.end());
            }
        }
    }

    cout << "MAX:" << MAX << endl;
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start);
    cout << "Time: " << duration.count() << endl;
}