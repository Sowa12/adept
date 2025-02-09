#include <string>

class Context;

class AState
{
protected:
    Context *context;
    bool _exit;
public:
    AState() = default;
    virtual std::wstring getInfo() noexcept(true) = 0;
    virtual std::wstring do_work(const std::wstring &) noexcept(true) = 0;
    void set_context(Context *context);
    bool needExit();
    virtual ~AState() {

    };
};

class StateMainInfo : public AState
{
public:
    std::wstring getInfo() noexcept(true) override;
    std::wstring do_work(const std::wstring &) noexcept(true) override;
};

class ReadState : public AState
{
public:
    std::wstring getInfo() noexcept(true) override;
    std::wstring do_work(const std::wstring &) noexcept(true) override;
};

class GroupState : public AState
{
public:
    std::wstring getInfo() noexcept(true) override;
    std::wstring do_work(const std::wstring &) noexcept(true) override;
};

class GroupStateByType : public AState
{
public:
    std::wstring getInfo() noexcept(true) override;
    std::wstring do_work(const std::wstring &) noexcept(true) override;
};

class AddState : public AState
{
public:
    std::wstring getInfo() noexcept(true) override;
    std::wstring do_work(const std::wstring &) noexcept(true) override;
};

class SaveState : public AState
{
public:
    std::wstring getInfo() noexcept(true) override;
    std::wstring do_work(const std::wstring &) noexcept(true) override;
};