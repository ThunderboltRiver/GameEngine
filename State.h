#ifndef STATE_h
#define STATE_h

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
namespace GameEngine
{
    namespace Agents
    {
        template <typename Holder>
        class State
        {
        private:
            typedef State<Holder> Child;
            using NextChildlen = typename std::unordered_map<int, Child *>;
            using Transition = typename std::unordered_map<Child *, NextChildlen>;

        private:
            std::vector<Child *> m_childlen;
            Transition m_transition;

        protected:
            Holder *m_holder;
            Child *m_currentChild;
            virtual void onFirstEnter(){};
            virtual void onEnter(){};
            virtual void onFirstUpdate(){};
            virtual void onUpdate(){};
            virtual void onFirstExit(){};
            virtual void onExit(){};
            void AddChild(Child *child)
            {
                /** バリデーション
                 * 親と子のステートが同じホルダーを持つかどうか
                 */
                if (child->m_holder != this->m_holder)
                {
                    throw std::invalid_argument("This given State in argument has not same holder");
                };
                if (std::find(m_childlen.begin(), m_childlen.end(), child) == m_childlen.end())
                {
                    m_childlen.emplace_back(child);
                };
            };
            void ChangeCurrent(int event)
            {
                /**
                 * 遷移テーブルにeventに対応する遷移が設定されているかどうか
                 */
                if (m_transition.count(m_currentChild) != 0 && m_transition[m_currentChild].count(event) != 0)
                {
                    Child *next = m_transition[m_currentChild][event];
                    if (next != nullptr)
                    {
                        m_currentChild->Exit();
                        m_currentChild = next;
                        m_currentChild->Enter();
                    };
                    return;
                };
                throw std::invalid_argument("This event key for the current child is not assigned yet");
            };

        public:
            void AddTransition(Child *from, Child *to, int event)
            {
                /**バリデーション
                 *二つのChildが同じホルダーを持っているか */
                if (from->m_holder != to->m_holder)
                {
                    throw std::invalid_argument("This pair of States has not same holder.");
                };

                /*バリデーション
                 * from と to がすでに遷移テーブルに設定されているかどうか */
                NextChildlen *_from_transtion = &(m_transition[from]);
                for (auto &&iter : *_from_transtion)
                {
                    if (iter.second == to)
                    {
                        throw std::invalid_argument("This pair of States is already assigned");
                    };
                };

                /**バリデーション
                 * from と event がすでに遷移テーブルに設定されているかどうか*/
                if (_from_transtion->count(event) != 0)
                {
                    throw std::invalid_argument("This event key is already assigned");
                };

                AddChild(from);
                AddChild(to);
                (*_from_transtion)[event] = to;
            };
            /**Holder 専用メソッド */
            void Enter()
            {
                onFirstEnter();
                if (m_currentChild != nullptr)
                {
                    m_currentChild->Enter();
                }
                onEnter();
            };
            void Update()
            {
                onFirstUpdate();
                if (m_currentChild != nullptr)
                {
                    m_currentChild->Update();
                };
                onUpdate();
            };
            void Exit()
            {
                onFirstExit();
                if (m_currentChild != nullptr)
                {
                    m_currentChild->Exit();
                };
                onExit();
            };
        };

    } // namespace Agents

} // namespace GameEngine

#endif