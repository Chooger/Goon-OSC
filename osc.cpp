#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <lo/lo.h>
#include <csignal>


std::atomic<bool> running(true);
std::string message = "goon.bio for all your needs";
std::string stick(60, '\n');

// Function listening for ctrl+c to shutdown extra threads
void shutdown(int signum) {
    std::cout << "\nExiting...\n";
    running = false;
}

// Function to send message to VRChat via OSC
void send_to_vrchat(const std::string& msg) {
    lo_address t = lo_address_new("127.0.0.1", "9000");
    lo_message m = lo_message_new();
    lo_message_add_string(m, msg.c_str());
    lo_message_add_true(m);
    lo_send_message(t, "/chatbox/input", m);
    lo_message_free(m);

    std::cout << "[Sent]: " << msg << "\n";
}

// Background thread that sends message every 10s
void timer_thread() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        if (!running) break;
        send_to_vrchat(message);
    }
}

int main() {
    std::signal(SIGINT, shutdown);
    std::thread sender(timer_thread);

   std::cout << "Welcome to the best OSC tool you'll ever use!\n"
             << "Thank you for anyone who uses my stuff, feel free to check out my stuff:\n"
             << "goon.bio/chooger\ngoon.bio\ngithub.com/Chooger\n"
             << "\n\n\nTo get started, chooose an option.\n1). Goon.bio propoganda\n"
             << "2) Custom message (will ask what you want it to be after selection of choice 2)\n"
             << "3) Stick? Gangmonkeys use it, you'll know what I mean\n"
             << "4) 8ball: ask it a question, choose number 4, and guidance shall come from above\n"
             << "\n**KEEP IN MIND** Press Ctrl + C to exit and restart the program and change your option\n"
             << "\n more will come later, feel free to drop suggestions in the discord!\n";

    int choice = 0;

    while (running) {
        std::string input;
        std::getline(std::cin, input);

        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid input. Please enter a number 1-5.\n";
            continue;
        }

        switch (choice) {
            case 1:
                message = "goon.bio for all your needs";
                break;
            case 2:
                std::cout << "Enter custom message: ";
                std::getline(std::cin, message);
                break;
            case 3:
                message = stick;
                break;
            case 4:
                message = "Just fucking do it!";
                break;
            case 5:
                running = false;
                break;
            default:
                std::cout << "Please select a valid entry (1-5).\n";
                continue;
        }

        if (running) {
            send_to_vrchat(message);
        }
    }

    sender.join();
    return 0;
}
