// AtomicClientQueue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<int> client(0);  
const int maxClients = 10;   

void clientTh() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        int expected = client.load(std::memory_order_relaxed);
        while (expected < maxClients &&
            !client.compare_exchange_weak(expected, expected + 1, std::memory_order_relaxed)) {
           
            expected = client.load(std::memory_order_relaxed);
        }

        std::cout << "Клиент пришел. Всего клиентов: " << client.load(std::memory_order_relaxed) << std::endl;
    }
}

void operatorTh() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        int expected = client.load(std::memory_order_relaxed);
        while (expected > 0 &&
            !client.compare_exchange_weak(expected, expected - 1, std::memory_order_relaxed)) {
            
            expected = client.load(std::memory_order_relaxed);
        }

        std::cout << "Операционист обслужил клиента. Всего клиентов: " << client.load(std::memory_order_relaxed) << std::endl;
    }
}

int main() {
    std::setlocale(LC_ALL, "RU");

    std::thread clientObj(clientTh);
    std::thread operatorObj(operatorTh);

    clientObj.join();
    operatorObj.join();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
