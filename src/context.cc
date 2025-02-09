#include <context.h>
#include <fstream>
#include <locale>
#include <codecvt>

Context::Context()
{
    _state.reset(new StateMainInfo);
    _state->set_context(this);
}
std::wstring Context::getInfo() noexcept(true)
{
    return _state->getInfo();
}

std::wstring Context::do_work(const std::wstring &input) noexcept(true)
{
    return _state->do_work(input);
}

void Context::transitionTo(std::unique_ptr<AState> &&state) noexcept(true)
{
    if (state.get() != nullptr)
    {
        _state = std::move(state);
        _state->set_context(this);
    }
}

void Context::setContainer(const container &container) noexcept(true)
{
    _container = container;
}

container Context::getContainer() const noexcept(true)
{
    return _container;
}

void Context::setGContainer(const gcontainer &container) noexcept(true)
{
    _gcontainer = container;
}

gcontainer Context::getGContainer() const noexcept(true)
{
    return _gcontainer;
}

void Context::addInContainer(const Data &d)
{
    this->_container.push_back(d);
}

bool Context::needExit()
{
    return _state->needExit();
}

void AState::set_context(Context *context)
{
    this->context = context;
}

bool AState::needExit()
{
    return _exit;
}

std::wstring AddState::getInfo() noexcept(true)
{
    return L"1. Введите объект\n";
}

std::wstring AddState::do_work(const std::wstring &input) noexcept(true)
{
    std::wstring result;

    try
    {
        auto vec = split(input);
        Data d;
        d << vec;
        context->addInContainer(d);
        result = L"Succes create!\n";
    }
    catch (...)
    {
        result = L"Object incorrect!\n";
    }
    context->transitionTo(std::move(std::make_unique<StateMainInfo>()));
    return result;
}

std::wstring SaveState::getInfo() noexcept(true)
{
    return L"1. Введите имя файла для сохранения\n";
}

std::wstring SaveState::do_work(const std::wstring &path) noexcept(true)
{
    std::wstring result;
    std::unique_ptr<AState> nextState(new StateMainInfo);
    gcontainer gstr = context->getGContainer();

    std::ofstream ofs(std::string(path.begin(), path.end()));
    if (ofs.is_open())
    {
        std::wstringstream sstream;
        std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;

        for (auto &item : gstr)
        {
            sstream << item.first << std::endl;
            for (auto &d : item.second)
            {
                sstream << "\t";
                d >> sstream;
                sstream << "\n";
            }
        }

        ofs << convert.to_bytes(sstream.str());
        result = L"Сохранено в файл: " + path + L"\n";
    }
    else
    {
        result = L"Невозможно создать файл: " + path + L"\n";
    }

    context->transitionTo(std::move(nextState));
    return result;
}
