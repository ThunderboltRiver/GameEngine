#include <string>
#include "State.h"
#include <iostream>

class Actor
{
private:
    std::string m_name;

public:
    std::string GetName() const { return m_name; };
    Actor(std::string name) : m_name(name){};
    ~Actor(){};
};

// using Component = typename GameEngine::General::Component<Actor>;

// class testComponent : public Component
// {
// private:
//     std::string m_name;

// public:
//     void Update() override
//     {
//         std::cout << m_name << std::endl;
//     };
//     testComponent(Actor *actor, std::string name) : m_name(name)
//     {
//         this->m_holder = actor;
//     };
//     ~testComponent(){};
// };

template <typename T>
using State = typename GameEngine::Agents::State<T>;

template <typename T>
class TestState : public State<T>
{
private:
    /* data */
    std::string m_name;
    void onFirstUpdate() override
    {
        std::cout << "called at first update:" << m_name << std::endl;
    };

    void onUpdate() override
    {
        std::cout << "called at update:" << m_name << std::endl;
    };

    void onExit() override
    {
        std::cout << "Exit:" << m_name << std::endl;
    };

public:
    TestState(T *t, State<T> *initial, std::string name) : m_name(name)
    {
        this->m_holder = t;
        this->m_currentChild = initial;
    };
    ~TestState(){};
};

class RootState : public State<Actor>
{
private:
    /* data */
    std::string m_name;
    int m_event;

public:
    void onFirstUpdate() override;
    RootState(Actor *acptr, State<Actor> *initial, std::string name, int event);
    ~RootState();
};

void RootState::onFirstUpdate()
{
    try
    {
        this->ChangeCurrent(m_event);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    };
};

RootState::RootState(Actor *acptr, State<Actor> *initial, std::string name, int event) : m_name(name), m_event(event)
{
    this->m_holder = acptr;
    m_currentChild = initial;
};

RootState::~RootState()
{
}

int main(void)
{
    /**Update 呼び出し順のテスト
     * root -> test -> test -> rootとなればよい
     */
    Actor *acptr = new Actor((std::string) "acptr-test");
    State<Actor> *test_state = new TestState<Actor>(acptr, nullptr, "test_state");
    State<Actor> *root_state = new RootState(acptr, test_state, "root", 1);
    State<Actor> *test_state2 = new TestState<Actor>(acptr, nullptr, acptr->GetName());
    root_state->Update();
    root_state->AddTransition(test_state, test_state2, 1);
    root_state->Update();
    root_state->Update();

    // /**AddTransition 正常テスト */
    // root_state->AddTransition(test_state, test_state2, 1);
    // std::cout << "OK" << std::endl;
    // /**AddTransition 例外テスト */
    // root_state->AddTransition(test_state, test_state2, 1);
    // Actor *acptr2 = new Actor((std::string) "test2");
    // State<Actor> *test_state3 = new TestState<Actor>(acptr2, nullptr, acptr2->GetName());
    // // root_state->AddTransition(test_state, test_state3, 1);
    // root_state->AddTransition(test_state, test_state2, 2);
    return 0;
};