#ifndef STATE_h
#define STATE_h

#include <string>
#include <vector>
#include <map>
namespace GameEngine
{
    namespace Agents
    {
        template <typename State>
        struct Event
        {
            const int id;
            const std::string name;
            const State *state;
            Event(int _id, State *_state) : id(_id), state(*_state){};
        };

        template <typename Holder>
        class State
        {
        public:
            using Self = typename State<Holder>;
            using Child = typename State<Self>;
            using NextChildlen = typename std::unorderdmap<int, Child *>;
            using Transition = typename std::unordermap<Child *, NextChildlen>;

        private:
            Holder *m_holder;
            std::vector<Child *> m_childlen;
            Child *m_currentChild;
            Transition m_transition;

        protected:
            virtual void onEnter(){} = 0;
            virtual void onUpdate(){} = 0;
            virtual void onExit(){} = 0;
            void ChangeChild(int event)
            {
                Child *next = m_transition[m_currentChild][event];
                if (next != nullptr)
                {
                    m_currentChild->Exit();
                    m_currentChild = next;
                    m_currentChild->Enter();
                };
            };
            void AddTransition(Child *from, Child *to, int event){

            };

        public:
            /**Holder 専用メソッド */
            void Enter()
            {
                if (m_currentChild != nullptr)
                {
                    m_currentChild->Enter();
                }
                onEnter();
            };
            virtual void Update()
            {
                if (m_currentChild != nullptr)
                {
                    m_currentChild->Update();
                };
                onUpdate();
            };
            void Exit()
            {
                if (m_currentChild != nullptr)
                {
                    m_currentChild->Exit();
                };
                onExit();
            };
            State(Holder *holder);
            ~State();
        };

    } // namespace Agents

} // namespace GameEngine

#endif