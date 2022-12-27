namespace GameEngine
{
    namespace General
    {
        template <typename Holder>
        class Component
        {
        protected:
            Holder *m_holder;

        public:
            virtual void Start(){};
            virtual void Update(){};
            virtual void LastUpdate(){};
        };
    } // namespace General

} // namespace GameEngine
