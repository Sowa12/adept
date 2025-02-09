#include <context.h>
#include <fstream>
#include <vector>
#include <cctype>
#include <codecvt>
#include <locale>

// class AReader {
//     protected:
//     bool flag_open;
//     public:
//     virtual void open(const std::string&) = 0;
//     bool is_open() {return flag_open;};
//     virtual std::vector<std::string> read() = 0;
//     virtual ~AReader() = default;
// };

// class FileReader : public AReader {
//     std::ifstream ifs;
//     public:

//     void open(const std::string& path) override {
//         ifs.open(path);
//         flag_open = ifs.is_open();
//     }

//     std::vector<std::string> read()  {
//         return
//     }
// };

std::vector<std::wstring> split(const std::wstring &input) noexcept(true)
{
    std::vector<std::wstring> result;
    if (input.empty())
        return result;

    std::wstringstream sstream(input);
    std::wstring line;

    while (std::getline(sstream, line, L' '))
    {
        result.push_back(line);
    }
    return result;
}

std::wstring ReadState::getInfo() noexcept(true)
{
    return L"Введите путь до файла\n";
}

std::wstring ReadState::do_work(const std::wstring &path) noexcept(true)
{
    std::ifstream ifs(std::string(path.begin(), path.end()));
    std::wstring result;

    if (ifs.is_open())
    {
        std::string line;
        container ctr;
        while (std::getline(ifs, line))
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;

            auto vec = split(convert.from_bytes(line));

            try
            {
                Data data;
                data << vec;
                ctr.push_back(data);
            }
            catch (const std::exception &exc)
            {
            }
        }
        context->setContainer(ctr);
        result = L"Успешно создано: " + std::to_wstring(ctr.size()) + L"\n";
    }
    else
    {
        result = L"Невозможно открыть файл: " + path + L"\n";
    }
    context->transitionTo(std::make_unique<StateMainInfo>());
    return result;
}