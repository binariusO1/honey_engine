#pragma once

#include <functional>
#include <iostream>
#include <tuple>

namespace he
{
class IEvent {
public:
    virtual ~IEvent() = default;
};
template <typename T>
class Event : public IEvent {
public:
    explicit Event(const T& d) : data(d) {}
    ~Event() override = default;

    T data;
};

template <typename T>
static std::shared_ptr<he::Event<T>> event_to_ptr(const T& d)
{
    std::shared_ptr<he::Event<T>> eventPtr = std::make_shared<he::Event<T>>(d);
    return std::move(eventPtr);
}

class IAction {
public:
    virtual ~IAction() {}
    virtual void execute() const = 0;
};

template <typename T>
class Action : public IAction {
public:
    T param;

    explicit Action(const T& p) : param(p) {}

    void execute() const override {
        std::cout << "Action executed with parameter: " << param << std::endl;
    }
};

class IGuard {
public:
    virtual ~IGuard() {}
    virtual bool check() const = 0;
};

template <typename T>
class Guard : public IGuard {
public:
    T value;

    explicit Guard(const T& v) : value(v) {}

    bool check() const override {
        return value > 0;
    }
};

template <typename T>
struct Transition 
{
    std::shared_ptr<T> fromState{nullptr};
    std::shared_ptr<IEvent> event{nullptr};
    std::shared_ptr<T> toState{nullptr};
    std::shared_ptr<IAction> action{nullptr};
    std::function<bool()> guard{nullptr};
};

template <typename T>
class IStateMachine
{
public:
    virtual ~IStateMachine() = default;

    virtual void setCurrentState(T* state) = 0;
    virtual void addTransition( std::shared_ptr<T> fromState, std::shared_ptr<IEvent> event, std::shared_ptr<T> toState,
                               std::shared_ptr<IAction> action,
                               std::shared_ptr<IGuard> guard) = 0;
    virtual void addTransition(const Transition<T>& ) = 0;
    virtual void process_event(const std::shared_ptr<IEvent>& event) = 0;
};
} // namespace he