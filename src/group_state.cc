#include <context.h>
#include <algorithm>
#include <math.h>
#include <climits>
#include <time.h>

class SortByRange
{
public:
    static gcontainer sort(const container &src) noexcept(true)
    {
        container ctr = const_cast<container &>(src);

        gcontainer gctr;

        container c100;
        container c1000;
        container c10000;
        container other;

        auto group = [](container &src, const uint32_t &min, const uint32_t &max)
        {
            container dst;
            std::copy_if(src.begin(),
                         src.end(),
                         std::back_inserter(dst),
                         [&min, &max](Data &d)
                         {
                             auto tmp = sqrt(pow(d._x, 2) * pow(d._y, 2));
                             return tmp >= min && tmp < max;
                         });

            std::remove_if(src.begin(),
                           src.end(),
                           [&min, &max](Data &d)
                           {
                               auto tmp = sqrt(pow(d._x, 2) * pow(d._y, 2));
                               return tmp >= min && tmp < max;
                           });

            std::sort(dst.begin(), dst.end(), [](Data &d1, Data &d2)
                      {
                auto dist1 = sqrt(pow(d1._x, 2) * pow(d1._y, 2));
                auto dist2 = sqrt(pow(d2._x, 2) * pow(d2._y, 2));
                return dist1 < dist2; });

            return dst;
        };

        gctr.insert(std::make_pair(L"c100", group(ctr, 0, 100)));
        gctr.insert(std::make_pair(L"c1000", group(ctr, 100, 1000)));
        gctr.insert(std::make_pair(L"c100000", group(ctr, 1000, 10000)));
        gctr.insert(std::make_pair(L"other", group(ctr, 10000, UINT32_MAX)));

        for (auto it = gctr.begin(); it != gctr.end();)
        {
            if (it->second.empty())
                it = gctr.erase(it);
            else
                ++it;
        }

        return gctr;
    };
};

class SortByName
{
public:
    static gcontainer sort(container &src) noexcept(true)
    {
        container ctr = const_cast<container &>(src);
        gcontainer gctr;
        std::vector<wchar_t> rusChar;

        for (uint16_t c = L'А'; c <= L'Я'; ++c)
        {
            rusChar.push_back(c);
        }

        auto group = [](container &src, const wchar_t &startC)
        {
            container dst;
            std::copy_if(src.begin(),
                         src.end(),
                         std::back_inserter(dst),
                         [&startC](Data &d)
                         {
                             if (d._name.empty())
                                 return false;
                             return ((*d._name.begin()) == startC);
                         });
            src.erase(
                std::remove_if(src.begin(),
                               src.end(),
                               [&startC](Data &d)
                               {
                                   if (d._name.empty())
                                       return false;
                                   return ((*d._name.begin()) == startC);
                               }),
                src.end());

            if (!dst.empty())
            {
                std::sort(dst.begin(), dst.end(), [](Data &d1, Data &d2)
                          { return d1._name > d2._name; });
            }

            return dst;
        };

        for (auto c : rusChar)
        {
            std::wstring name;
            name += c;
            gctr.insert(std::make_pair(name, group(ctr, c)));
        }

        gctr.insert(std::make_pair(L"#", ctr));

        for (auto it = gctr.begin(); it != gctr.end();)
        {
            if (it->second.empty())
                it = gctr.erase(it);
            else
                ++it;
        }

        return gctr;
    };
};

class SortByType
{
public:
    static gcontainer sort(container &ctr, const size_t &count) noexcept(true)
    {
        gcontainer gctr;
        gctr.insert(std::make_pair(L"Разное", container()));
        std::map<std::wstring, container> tmp;

        for (auto &d : ctr)
        {
            auto it = tmp.find(d._type);

            if (it == tmp.end())
            {
                tmp.insert(std::make_pair(d._type, container()));
                it = tmp.find(d._type);
            }

            it->second.push_back(d);
        }

        for (auto &item : tmp)
        {
            if (item.second.size() >= count)
            {
                gctr.insert(std::make_pair(item.first, item.second));
            }
            else
            {
                auto &raznoe = gctr.at(L"Разное");
                for (auto &d : item.second)
                {
                    raznoe.push_back(d);
                }
            }
        }

        for (auto it = gctr.begin(); it != gctr.end();)
        {
            if (it->second.empty())
                it = gctr.erase(it);
            else
                ++it;
        }

        return gctr;
    };
};

class SortByTime
{
public:
    static gcontainer sort(container &ctr) noexcept(true)
    {
        gcontainer gctr;
        time_t now;
        time(&now);
        struct tm dTimeNow;
        localtime_r(&now, &dTimeNow);
        dTimeNow.tm_hour = 0;
        dTimeNow.tm_min = 0;
        dTimeNow.tm_sec = 0;

        gctr.insert(std::make_pair(L"Сегодня", container()));
        gctr.insert(std::make_pair(L"Завтра", container()));
        gctr.insert(std::make_pair(L"На этой неделе", container()));
        gctr.insert(std::make_pair(L"В этом месяце", container()));
        gctr.insert(std::make_pair(L"В этом году", container()));
        gctr.insert(std::make_pair(L"Ранее", container()));

        for (auto &d : ctr)
        {
            time_t t = d._createTime;

            struct tm dTime;
            localtime_r(&t, &dTime);
            dTime.tm_hour = 0;
            dTime.tm_min = 0;
            dTime.tm_sec = 0;
            time_t tommorow = now + 86400;

            time_t st_week = now - (dTime.tm_wday * 86400);
            time_t end_week = now + ((6 - dTime.tm_wday) * 86400);

            if (dTime.tm_year == dTimeNow.tm_year)
            {
                if (dTime.tm_mon == dTimeNow.tm_mon)
                {
                    if (dTime.tm_mday == dTimeNow.tm_mday)
                    {
                        gctr.at(L"Сегодня").push_back(d);
                    }
                    else if (localtime(&tommorow)->tm_year == dTime.tm_year)
                    {
                        gctr.at(L"Завтра").push_back(d);
                    }
                    else if (st_week <= t && t < end_week)
                    {
                        gctr.at(L"На этой неделе").push_back(d);
                    }
                    else
                    {
                        gctr.at(L"В этом месяце").push_back(d);
                    }
                }
                else
                {
                    gctr.at(L"В этом году").push_back(d);
                }
            }
            else
            {
                gctr.at(L"Ранее").push_back(d);
            }
        }

        for (auto it = gctr.begin(); it != gctr.end();)
        {
            if (it->second.empty())
                it = gctr.erase(it);
            else {
                std::sort(it->second.begin(), it->second.end(), [](const Data& left, const Data& right) {
                    return left._createTime > right._createTime; 
                });
                ++it;
            }
        }

        return gctr;
    };
};

std::wstring GroupState::getInfo() noexcept(true)
{
    std::wstringstream sstream;
    sstream << L"1. Группировать по расстоянию\n"
            << L"2. Группировка по имени\n"
            << L"3. Группировка по тиму\n"
            << L"4. Группировка по времени создания\n"
            << L"5. Показать результат\n"
            << L"6. Назад\n";

    return sstream.str();
}

std::wstring GroupStateByType::getInfo() noexcept(true)
{
    return L"1. Введите кол-во объектов для группировки\n";
}

std::wstring GroupStateByType::do_work(const std::wstring &input) noexcept(true)
{
    std::wstring result;
    std::unique_ptr<AState> nextState(new GroupState);
    auto ctr = context->getContainer();
    gcontainer gstr;

    try
    {
        uint32_t choose = std::stoi(input);
        gstr = SortByType::sort(ctr, choose);
        context->setGContainer(gstr);
    }
    catch (...)
    {
        context->transitionTo(std::move(nextState));
        return L"Выбрана не существующий пункт!\n";
    }

    context->transitionTo(std::move(nextState));

    return result;
}

std::wstring GroupState::do_work(const std::wstring &input) noexcept(true)
{
    std::wstring result;
    std::unique_ptr<AState> nextState;
    auto ctr = context->getContainer();
    gcontainer gstr;

    try
    {
        uint8_t choose = std::stoi(input);
        switch (choose)
        {
        case 1:
        {
            gstr = SortByRange::sort(ctr);
            context->setGContainer(gstr);
        }
        break;
        case 2:
        {
            gstr = SortByName::sort(ctr);
            context->setGContainer(gstr);
        }
        break;
        case 3:
        {
            nextState.reset(new GroupStateByType);
        }
        break;
        case 4:
        {
            gstr = SortByTime::sort(ctr);
            context->setGContainer(gstr);
        }
        break;
        case 5:
        {
            std::wstringstream sstream;

            for (auto &item : context->getGContainer())
            {
                sstream << item.first << std::endl;
                for (auto &d : item.second)
                {
                    sstream << "\t";
                    d >> sstream;
                    sstream << "\n";
                }
            }

            result = sstream.str();
        }
        break;
        case 6:
        {
            nextState.reset(new StateMainInfo);
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
