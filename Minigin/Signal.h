//credits to Julian Rijken, Matias Devred, Leander Flossie, Patrick Nellessen, Viktor Mendonck and Leen Ritserveldt
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <mutex>


namespace dae
{
    namespace signal
    {
        template<typename... Args>
        class Signal;

        template<typename... Args>
        class Connection;

        template<typename... Args>
        class BaseSlot
        {
        public:
            BaseSlot() = default;
            virtual ~BaseSlot() = default;

            BaseSlot(BaseSlot&&) = delete;
            BaseSlot(const BaseSlot&) = delete;
            BaseSlot& operator=(BaseSlot&&) = delete;
            BaseSlot& operator=(const BaseSlot&) = delete;

            virtual void Invoke(Args... args) = 0;
        };

        template<typename... Args>
        class StaticFunctonSlot final : public BaseSlot<Args...>
        {
        public:
            StaticFunctonSlot() = default;
            StaticFunctonSlot(std::function<void(Args...)> function)
                : m_Function(function)
            {

            }

            ~StaticFunctonSlot() override
            {

            }

            void Invoke(Args... args) override
            {
                m_Function(std::forward<Args>(args)...);
            }
        private:

            std::function<void(Args...)>  m_Function;
        };

        template<typename ClassType, typename... Args>
        class MemberFunctionSlot final : public BaseSlot<Args...>
        {
        public:
            MemberFunctionSlot(ClassType* functionClass, void(ClassType::* function) (Args...))
                : m_FunctionClass(functionClass)
                , m_Function(function)
            {

            }

            void Invoke(Args... args) override
            {
                (m_FunctionClass->*m_Function)(std::forward<Args>(args)...);
            }

        private:

            ClassType* m_FunctionClass;

            void(ClassType::* m_Function) (Args...);

        };



        template<typename... Args>
        class Connection final
        {
        public:
            enum class ConnectionState
            {
                connected,
                disconnected,
                paused
            };


            Connection() = delete;
            //member function connection
            template<typename ClassType>
            Connection(Signal<Args...>* signal, ClassType* functionClass, void(ClassType::* function) (Args...), std::string functionName = "")
                : m_pSignal(signal)
                , m_FunctionName(std::move(functionName))
                , m_IsMemberFunction(true)
                , m_Slot(std::make_unique<MemberFunctionSlot<ClassType, Args...>>(functionClass, function))
            {
                if (m_Slot.get())
                {
                    m_ConnectionState = ConnectionState::connected;
                }
            }
            //static Connection
            Connection(Signal<Args...>* signal, std::function<void(Args...)> function, std::string functionName = "")
                : m_pSignal(signal)
                , m_IsMemberFunction(false)
                , m_FunctionName(std::move(functionName))
                , m_Slot(std::make_unique<StaticFunctonSlot<Args...>>(function))
            {
                if (m_Slot.get())
                {
                    m_ConnectionState = ConnectionState::connected;
                }
            }

            ~Connection() = default;

            Connection(Connection&&) = delete;
            Connection(const Connection&) = delete;
            Connection& operator=(Connection&&) = delete;
            Connection& operator=(const Connection&) = delete;

            void Disconnect()
            {
                m_ConnectionState = ConnectionState::disconnected;
                m_pSignal->RemoveListener(this);
            }
            void Pause()
            {
                m_ConnectionState = ConnectionState::paused;
            }

            void Resume()
            {
                if (m_ConnectionState == ConnectionState::paused)
                    m_ConnectionState = ConnectionState::connected;
            }

            void Invoke(Args... args)
            {
                if (m_ConnectionState == ConnectionState::connected)
                {
                    m_Slot->Invoke(std::forward<Args>(args)...);
                }
            }

            ConnectionState GetState() const
            {
                return m_ConnectionState;
            }

            std::string GetFunctionName() const
            {
                return m_FunctionName;
            }

            bool IsMemberFunction() const
            {
                return m_IsMemberFunction;
            }

        private:

            bool m_IsMemberFunction = false;
            std::string m_FunctionName;

            ConnectionState m_ConnectionState = ConnectionState::disconnected;

            std::unique_ptr<BaseSlot<Args...>> m_Slot;

            Signal<Args...>* m_pSignal = nullptr;

        };

        template<typename... Args>
        class Signal final
        {
            typedef std::function<void(Args...)> RawFunctionType;

            typedef std::pair<std::optional<void*>, RawFunctionType> FunctionType;

        public:
            Signal() = default;

            ~Signal()
            {
                for (int i{}; i < m_ListenerFunctions.size(); ++i)
                {
                    m_ListenerFunctions[i]->Disconnect();
                }
                m_ListenerFunctions.clear();
            }

            Signal(Signal&&) = delete;
            Signal(const Signal&) = delete;
            Signal& operator=(Signal&&) = delete;
            Signal& operator=(const Signal&) = delete;

            Connection<Args...>* Connect(RawFunctionType listener)
            {
                m_ListenerFunctions.push_back(std::make_unique<Connection<Args...>>(this, listener));
                return m_ListenerFunctions.back().get();
            }

            template<typename ClassType>
            Connection<Args...>* Connect(ClassType* functionClass, void(ClassType::* memberFunc) (Args...))
            {
                m_ListenerFunctions.push_back(std::make_unique<Connection<Args...>>(this, functionClass, memberFunc));
                return m_ListenerFunctions.back().get();
            }

            void RemoveListener(Connection<Args...>* listener)
            {
                std::erase_if(m_ListenerFunctions, [&](auto&& element)
                    {
                        return element.get() == listener;
                    });
            }

            void Emit(Args... args)
            {
                for (int i{}; i < m_ListenerFunctions.size(); ++i)
                {
                    m_ListenerFunctions[i]->Invoke(args...);
                }
            }

        private:

            template<typename T, typename... U>
            size_t GetAddress(std::function<T(U...)> func) const
            {
                typedef T(fnType)(U...);
                fnType** fnPointer = func.template target<fnType*>();

                if (fnPointer == nullptr)
                    return 0;

                return reinterpret_cast<size_t>(*fnPointer);
            }

            std::vector<std::unique_ptr<Connection<Args...>>> m_ListenerFunctions{};
        };
    }
}


