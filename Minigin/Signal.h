#pragma once

#include <algorithm>
#include <deque>
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



        //-------------------------------------------------------------------
        // Base Slot class
        //-------------------------------------------------------------------
        template<typename... Args>
        class BaseSlot
        {
        public:
            // Default constructor used by the derived classes
            BaseSlot() = default;
            virtual ~BaseSlot() = default;

            // Move and copy constructors should not be used
            BaseSlot(BaseSlot&&) = delete;
            BaseSlot(const BaseSlot&) = delete;
            BaseSlot& operator=(BaseSlot&&) = delete;
            BaseSlot& operator=(const BaseSlot&) = delete;

            // Invoke the function (pure virtual function)
            virtual void Invoke(Args... args) = 0;
        };


        //-------------------------------------------------------------------
        // Static Function Slot class
        //-------------------------------------------------------------------
        template<typename... Args>
        class StaticFunctionSlot final : public BaseSlot<Args...>
        {
        public:
            // Default constructor should not be used
            StaticFunctionSlot() = delete;

            // Constructor with the function to be called
            StaticFunctionSlot(std::function<void(Args...)> function)
                : m_Function(function)
            {

            }

            // Invoke the static function
            void Invoke(Args... args) override
            {
                m_Function(std::forward<Args>(args)...);
            }
        private:
            // The function to be called
            std::function<void(Args...)>  m_Function;
        };


        //-------------------------------------------------------------------
        // Member Function Slot class
        //-------------------------------------------------------------------
        template<typename ClassType, typename... Args>
        class MemberFunctionSlot final : public BaseSlot<Args...>
        {
        public:
            // Constructor with the class and the member function to be called
            MemberFunctionSlot(ClassType* functionClass, void(ClassType::* function) (Args...))
                : m_FunctionClass(functionClass)
                , m_Function(function)
            {

            }

            // Default constructor should not be used
            MemberFunctionSlot() = delete;

            // Invoke the member function
            void Invoke(Args... args) override
            {
                if (m_FunctionClass)
                {
                    (m_FunctionClass->*m_Function)(std::forward<Args>(args)...);
                }
                else
                {
                    // Todo: disconnect the connection if the function class is null
                }
            }

        private:
            // The class that contains the member function
            ClassType* m_FunctionClass;

            // The member function to be called
            void(ClassType::* m_Function) (Args...);

        };



        //-------------------------------------------------------------------
        // Connection class
        //-------------------------------------------------------------------
        template<typename... Args>
        class Connection final
        {
        public:
            // Enum class to define the state of the connection (connected, disconnected, paused)
            enum class ConnectionState
            {
                connected,
                disconnected,
                paused
            };

            // Default constructor should not be used
            Connection() = delete;

            // Member function connection
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

            // Static Connection
            Connection(Signal<Args...>* signal, std::function<void(Args...)> function, std::string functionName = "")
                : m_pSignal(signal)
                , m_FunctionName(std::move(functionName))
                , m_Slot(std::make_unique<StaticFunctionSlot<Args...>>(function))
            {
                if (m_Slot.get())
                {
                    m_ConnectionState = ConnectionState::connected;
                }
            }

            // Destructor
            ~Connection() = default;

            // Move and copy constructors should not be used
            Connection(Connection&&) = delete;
            Connection(const Connection&) = delete;
            Connection& operator=(Connection&&) = delete;
            Connection& operator=(const Connection&) = delete;

            // Disconnect the connection (can't be used anymore)
            void Disconnect()
            {
                m_ConnectionState = ConnectionState::disconnected;
                m_pSignal->RemoveListener(this);
            }

            // Pause the connection
            void Pause()
            {
                m_ConnectionState = ConnectionState::paused;
            }

            // Resume the connection
            void Resume()
            {
                if (m_ConnectionState == ConnectionState::paused)
                    m_ConnectionState = ConnectionState::connected;
            }

            // Invoke the function
            void Invoke(Args... args)
            {
                if (m_ConnectionState == ConnectionState::connected)
                {
                    m_Slot->Invoke(std::forward<Args>(args)...);
                }
            }

            // Returns the state of the connection (connected, disconnected, paused)
            ConnectionState GetState() const
            {
                return m_ConnectionState;
            }

            // Returns the name of the function (only useful if you name the function), can be used for debugging
            std::string GetFunctionName() const
            {
                return m_FunctionName;
            }

            // Returns true if the connection is a member function, can be used for debugging
            bool IsMemberFunction() const
            {
                return m_IsMemberFunction;
            }

        private:
            // True if the connection is a member function
            bool m_IsMemberFunction = false;

            // Name of the function, can be used for debugging
            std::string m_FunctionName;

            // State of the connection
            ConnectionState m_ConnectionState = ConnectionState::disconnected;

            // Slot that contains the function to be called
            std::unique_ptr<BaseSlot<Args...>> m_Slot;

            // Pointer to the signal
            Signal<Args...>* m_pSignal = nullptr;
        };


        //-------------------------------------------------------------------
        // Signal class
        //-------------------------------------------------------------------
        template<typename... Args>
        class Signal final
        {
            typedef std::function<void(Args...)> RawFunctionType;

        public:
            // Create a signal in its own thread
            Signal()
                : m_IsThreaded(false)
            {
            }

            // Create a signal in a thread that is passed as a reference (useful if you want to join the thread)
            Signal(std::jthread& SignalThread)
                : m_IsThreaded(true)
            {
                std::jthread t(&Signal::Run, this);
                SignalThread = std::move(t);
            }

            // Destructor
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

            // Disconnects all the listeners
            void DisconnectAll()
            {
                std::lock_guard<std::mutex> lock(m_ConnectionMutex);
                for (int i{}; i < m_ListenerFunctions.size(); ++i)
                {
                    m_ListenerFunctions[i]->Disconnect();
                }
                m_ListenerFunctions.clear();
            }

            // Connects a static function to the signal
            Connection<Args...>* Connect(RawFunctionType listener)
            {
                std::lock_guard<std::mutex> lock(m_ConnectionMutex);
                m_ListenerFunctions.push_back(std::make_unique<Connection<Args...>>(this, listener));
                return m_ListenerFunctions.back().get();
            }

            // Connects a member function to the signal
            template<typename ClassType>
            Connection<Args...>* Connect(ClassType* functionClass, void(ClassType::* memberFunc) (Args...))
            {
                std::lock_guard<std::mutex> lock(m_ConnectionMutex);
                m_ListenerFunctions.push_back(std::make_unique<Connection<Args...>>(this, functionClass, memberFunc));
                return m_ListenerFunctions.back().get();
            }

            // Removes a listener from the list
            void RemoveListener(Connection<Args...>* listener)
            {
                std::lock_guard<std::mutex> lock(m_ConnectionMutex);
                std::erase_if(m_ListenerFunctions, [&](auto&& element)
                    {
                        return element.get() == listener;
                    });
            }

            // Pushes the arguments to the que and notifies the signal thread to emit the arguments
            void Emit(Args... args)
            {
                std::unique_lock<std::mutex> lock(m_QueMutex);
                m_Que.push_back(std::make_tuple(args...));
                lock.unlock();
                m_Condition.notify_one();
            }

            // End the signal thread
            void End()
            {
                m_ShouldEnd = true;
                m_Condition.notify_one();
            }

            // Function for updating the signal in the main thread
            // This function is used when the signal is not threaded
            void Update()
            {
                if (m_IsThreaded)
                {
                    return;
                }

                // Get the arguments from the que
                std::unique_lock<std::mutex> lock(m_QueMutex);

                std::tuple<Args...> argsTuple = m_Que.front();
                m_Que.pop_front();

                lock.unlock();

                // Invoke the listeners
                std::lock_guard<std::mutex> lock2(m_ConnectionMutex);
                std::ranges::for_each
                (
                    m_ListenerFunctions, [&argsTuple](auto& function)
                    {
                        std::apply([&](auto&&... args) { function->Invoke(std::forward<Args>(args)...); }, argsTuple);
                    }
                );
            }


        private:

            // Thread function that processes the arguments in the que
            void Run()
            {
                while (m_IsActive)
                {
                    // Create a unique lock with the mutex
                    std::unique_lock<std::mutex> lock(m_QueMutex);

                    // Wait until the queue is not empty
                    m_Condition.wait(lock, [this] { return !m_Que.empty(); });

                    std::tuple<Args...> argsTuple = m_Que.front();
                    m_Que.pop_front();

                    // Unlock the mutex
                    lock.unlock();

                    //lock the connection mutex so we can safely invoke the listeners even if they are removed
                    std::lock_guard<std::mutex> lock2(m_ConnectionMutex);
                    std::ranges::for_each
                    (
                        m_ListenerFunctions, [&argsTuple](auto& function)
                        {
                            std::apply([&](auto&&... args) { function->Invoke(std::forward<Args>(args)...); }, argsTuple);
                        }
                    );

                    if (m_ShouldEnd)
                    {
                        m_IsActive = false;
                        break;
                    }
                }
            }

            // Protects the listeners list
            std::mutex m_ConnectionMutex;

            // List of listeners
            std::vector<std::unique_ptr<Connection<Args...>>> m_ListenerFunctions{};

            // Protects the que of arguments
            std::mutex m_QueMutex;

            // Que of arguments that need to be processed
            std::deque<std::tuple<Args...>> m_Que{};

            // Condition variable to notify the signal thread that there are new arguments
            std::condition_variable m_Condition;

            // Bool to check if the signal thread should keep running
            std::atomic_bool m_ShouldEnd = false;

            // Bool to see if the signal tread is running
            std::atomic_bool m_IsActive = true;

            // Bool to see if the signal is threaded
            const bool m_IsThreaded;
        };
    }
}

// nice