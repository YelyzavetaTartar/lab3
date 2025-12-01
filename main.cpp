#include <iostream>
#include <vector>
#include <thread>
#include <barrier>
#include <syncstream>
#include <chrono>
#include <functional>

void f(char group, int id) {
    std::osyncstream(std::cout) << "From the set " << group << " action completed  " << id << "." << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void worker(int thread_idx, std::barrier<>& sync_point) {

    if (thread_idx == 0) {
        f('a', 1); f('a', 2); f('a', 3); f('a', 4);
    }
    else if (thread_idx == 1) {
        f('a', 5); f('a', 6); f('a', 7); f('b', 1);
    }
    else if (thread_idx == 2) {
        f('b', 2); f('b', 3); f('b', 4); f('b', 5);
    }
    else if (thread_idx == 3) {
        f('b', 6); f('b', 7); f('c', 1); f('c', 2);
    }
    else if (thread_idx == 4) {
        f('c', 3); f('c', 4); f('c', 5); f('c', 6);
    }

    sync_point.arrive_and_wait();

    if (thread_idx == 0) {
        f('d', 1); f('d', 2); f('d', 3);
    }
    else if (thread_idx == 1) {
        f('d', 4); f('d', 5); f('d', 6);
    }
    else if (thread_idx == 2) {
        f('e', 1); f('e', 2); f('e', 3);
    }
    else if (thread_idx == 3) {
        f('e', 4); f('e', 5); f('e', 6); f('e', 7);
    }
    else if (thread_idx == 4) {
        f('f', 1); f('f', 2); f('f', 3); f('f', 4);
    }

    sync_point.arrive_and_wait();

    if (thread_idx == 0) {
        f('g', 1); f('g', 2); f('g', 3);
    }
    else if (thread_idx == 1) {
        f('g', 4); f('h', 1); f('h', 2);
    }
    else if (thread_idx == 2) {
        f('h', 3); f('h', 4); f('h', 5);
    }
    else if (thread_idx == 3) {
        f('h', 6); f('h', 7); f('i', 1);
    }
    else if (thread_idx == 4) {
        f('i', 2); f('i', 3); f('i', 4);
    }

    sync_point.arrive_and_wait();

    if (thread_idx == 0) {
        f('j', 1); f('j', 2); f('j', 3);
    }
    else if (thread_idx == 1) {
        f('j', 4); f('j', 5); f('j', 6);
    }
    else if (thread_idx == 2) {
        f('j', 7); f('j', 8); f('j', 9);
    }
    else if (thread_idx == 3) {
        f('k', 1); f('k', 2); f('k', 3);
    }
    else if (thread_idx == 4) {
        f('k', 4); f('k', 5);
    }

}

int main() {
    std::cout << "Calculation started." << std::endl;

    const int nt = 5;
    std::barrier sync_point(nt);

    std::vector<std::jthread> threads;
    threads.reserve(nt);

    for (int i = 0; i < nt; ++i) {
        threads.emplace_back(worker, i, std::ref(sync_point));
    }

    threads.clear();

    std::cout << "Calculation complete." << std::endl;

    return 0;
}
