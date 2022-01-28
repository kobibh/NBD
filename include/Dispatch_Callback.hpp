/*****************************************************
* File: 	   dispatch_callback.hpp               	 *
* Date: 	   07/10/2021							 *
* Description: Dispatch-Callback API                 * 
******************************************************/

#ifndef ILRD_HRD18_DISPATCH_CALLBACK
#define ILRD_HRD18_DISPATCH_CALLBACK

#include <memory>   //std::shared_ptr
#include <vector>   //std::vector
#include <algorithm>

namespace HRD18
{

template <typename Msg>
class ICallback;

template <typename Msg>
class Dispatcher
{
public:
    explicit Dispatcher() = default;
    ~Dispatcher();
    
    //uncopyable
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;


    void NotifyAll(const Msg& msg) const;

private:
    friend class ICallback<Msg>;
    
    //may throw std::bad_alloc
    void Register(ICallback<Msg>* callback);
    void Unregister(ICallback<Msg>* callback);
    
    std::vector<ICallback<Msg>*> m_callbacks;
};

template <typename Msg>
Dispatcher<Msg>::~Dispatcher()
{
    for (ICallback<Msg>* cb : m_callbacks)
    {
        cb->OutOfService();
    }    
}

template <typename Msg>
void Dispatcher<Msg>::NotifyAll(const Msg& msg) const
{
    for (ICallback<Msg>* cb : m_callbacks)
    {
        cb->Notify(msg);
    }    
}

template <typename Msg>
void Dispatcher<Msg>::Register(ICallback<Msg>* callback)
{
    m_callbacks.push_back(callback);
}

template <typename Msg>
void Dispatcher<Msg>::Unregister(ICallback<Msg>* callback)
{
    // erase-remove idiom 
    m_callbacks.erase(std::remove(m_callbacks.begin(), m_callbacks.end(), callback));
}


template <typename Msg>
class ICallback
{
public:
    explicit ICallback(Dispatcher<Msg>* dispatcher);
    virtual ~ICallback();

    ICallback(const ICallback&) = delete;
    ICallback& operator=(const ICallback&) = delete;

private:
    friend class Dispatcher<Msg>;

    virtual void Notify(const Msg& msg) = 0;
    virtual void OutOfService() = 0;

    Dispatcher<Msg>* m_dispatcher;

};


template <typename Msg>
ICallback<Msg>::ICallback(Dispatcher<Msg>* dispatcher) : m_dispatcher(dispatcher)
{
    m_dispatcher->Register(this);
}

template <typename Msg>
ICallback<Msg>::~ICallback()
{
    m_dispatcher->Unregister(this);
}

template<typename Msg, typename Obs>
class Callback : public ICallback<Msg>
{
public:
    using DoAction = void(Obs::*)(const Msg& msg);
    using DoStop = void(Obs::*)();

    explicit Callback(Dispatcher<Msg>* dispatcher, Obs& obs, DoAction do_action, DoStop do_stop = nullptr);
    ~Callback() override = default;
    
    Callback(const Callback&) = delete;
    Callback& operator=(const Callback&) = delete;

private:
    void Notify(const Msg& msg) override;
    void OutOfService() override;

    Obs& m_observer;
    DoAction m_do_action;
    DoStop m_do_stop;
};

template <typename Msg, typename Obs>
Callback<Msg, Obs>::Callback(Dispatcher<Msg>* dispatcher,
                            Obs& obs,
                            DoAction do_action,
                            DoStop do_stop) : ICallback<Msg>(dispatcher),
                                                        m_observer(obs),
                                                        m_do_action(do_action),
                                                        m_do_stop(do_stop)
{}

template <typename Msg, typename Obs>
void Callback<Msg, Obs>::Notify(const Msg& msg)
{
    (m_observer.*m_do_action)(msg);
}

template <typename Msg, typename Obs>
void Callback<Msg, Obs>::OutOfService()  
{
    (m_observer.*m_do_stop)();
}

}
#endif //ILRD_HRD18_DISPATCH_CALLBACK