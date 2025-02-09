#include <sstream>
#include <ctime>
#include <memory>
#include <list>
#include <vector>
#include <iostream>
#include <map>
#include <state.h>

std::vector<std::wstring> split(const std::wstring &input) noexcept(true);

struct Data
{
    std::wstring _name;
    double _x;
    double _y;
    std::wstring _type;
    double _createTime;

    Data() = default;
    Data(const Data &other)
    {
        this->operator=(other);
    }

    Data &operator=(const Data &other)
    {
        if (&other != this)
        {
            _name = other._name;
            _x = other._x;
            _y = other._y;
            _type = other._type;
            _createTime = other._createTime;
        }
        return *this;
    }

    Data &operator<<(const std::vector<std::wstring> &input)
    {
        try
        {
            _name = input.at(0);
            _x = std::stod(input.at(1));
            _y = std::stod(input.at(2));
            _type = input.at(3);
            _createTime = std::stod(input.at(4));

            return *this;
        }
        catch (...)
        {
            throw;
        }
    };

    Data &operator>>(std::wstringstream &input)
    {
        input << _name << " ";
        input << std::to_wstring(_x) << " ";
        input << std::to_wstring(_y) << " ";
        input << _type << " ";
        input << _createTime << " ";
        return *this;
    }
    ~Data() = default;
};

using container = std::vector<Data>;
using gcontainer = std::map<std::wstring, container>;

class Context
{
    std::unique_ptr<AState> _state;
    container _container;
    gcontainer _gcontainer;

public:
    Context();
    Context(const Context &) = delete;

    std::wstring getInfo() noexcept(true);
    std::wstring do_work(const std::wstring &) noexcept(true);
    void transitionTo(std::unique_ptr<AState> &&state) noexcept(true);

    void setContainer(const container &container) noexcept(true);
    container getContainer() const noexcept(true);
    void setGContainer(const gcontainer &container) noexcept(true);
    gcontainer getGContainer() const noexcept(true);
    void addInContainer(const Data &d);
    bool needExit();
    ~Context() = default;
};