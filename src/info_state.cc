#include <context.h>

std::wstring StateMainInfo::getInfo() noexcept(true)
{
    std::wstringstream sstream;
    sstream << L"1. Прочитать список объектов из файла\n"
            << L"2. Добавить объект в список\n"
            << L"3. Группировать объекты\n"
            << L"4. Сохранить результаты в файл\n"
            << L"5. Завершить работу\n";

    return sstream.str();
}

std::wstring StateMainInfo::do_work(const std::wstring &input) noexcept(true)
{
    std::wstring result;
    std::unique_ptr<AState> nextState;
    
    try
    {
        uint8_t choose = std::stoi(input);
        switch (choose)
        {
        case 1:
        {
            nextState.reset(new ReadState);
        }
        break;
        case 2:
        {
            nextState.reset(new AddState);
        }
        break;
        case 3:
        {
            nextState.reset(new GroupState);
        }
        break;
        case 4:
        {
            nextState.reset(new SaveState);
        }
        break;
        case 5:
        {
            _exit = true;
        }
        break;
        default:
        {
            throw std::runtime_error("");
        }
        break;
        }
    }
    catch (...)
    {
        return L"Выбрана не существующий пункт!\n";
    }
    context->transitionTo(std::move(nextState));
    return result;
}
