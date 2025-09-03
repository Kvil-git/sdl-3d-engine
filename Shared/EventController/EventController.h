#ifndef EVENT_CONTROLLER_H
#define EVENT_CONTROLLER_H



#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <string>


class EventController {
    private:
        using CallbackID = size_t;
        using EventCallbackList = std::vector<std::pair<CallbackID, std::function<void(const void*)>>>;

        std::unordered_map<std::type_index, EventCallbackList> subscriptions;
        CallbackID nextCallbackID = 0;

    public:
        template <typename EventType>
        CallbackID Subscribe(std::function<void(const EventType&)> callback) {
            auto wrapper = [callback](const void* eventData) {
                callback(*static_cast<const EventType*>(eventData));
            };

            CallbackID currentId = nextCallbackID++;
            subscriptions[typeid(EventType)].push_back(std::make_pair(currentId, wrapper));
            return currentId;
        }

        template <typename EventType>
        void Unsubscribe(CallbackID callbackId) {
            auto eventTypeIterator = subscriptions.find(typeid(EventType));
            if (eventTypeIterator == subscriptions.end()) return;
            
            auto& callbackList = eventTypeIterator->second;
            auto callbackIterator = std::find_if(callbackList.begin(), callbackList.end(),
                [callbackId](const auto& subscription) { 
                    return subscription.first == callbackId; 
                });
            
            if (callbackIterator != callbackList.end()) {
                callbackList.erase(callbackIterator);
            }
        }

        template <typename EventType>
        void Emit(const EventType& event) {
            auto eventTypeIterator = subscriptions.find(typeid(EventType));
            if (eventTypeIterator == subscriptions.end()) return;

            for (const auto& callbackPair : eventTypeIterator->second) {
                auto callback = callbackPair.second;
                callback(&event);
            }
        }
};




#endif