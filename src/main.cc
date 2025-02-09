#include <iostream>
#include <context.h>

void runTask() {
    Context context;
    std::wstring line;

    while (!context.needExit()) {
        std::wcout << context.getInfo();
        std::getline(std::wcin, line);
        std::wcout << context.do_work(line);
    }
} 

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");
    runTask();
    return 0;
}