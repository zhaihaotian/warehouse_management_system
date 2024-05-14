#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <ctime>
#include <typeinfo>
#include <sstream>

// 商品类模板
template <typename T>
class Item {
private:
    std::string name;
    T stock;
    double price;

public:
    Item(std::string name, T stock, double price) : name(std::move(name)), stock(stock), price(price) {}

    void updateStock(T amount) {
        stock += amount;
    }

    void updatePrice(double newPrice) {
        price = newPrice;
    }

    std::string getName() const { return name; }
    T getStock() const { return stock; }
    double getPrice() const { return price; }

    void display() const {
        std::cout << "Item: " << name << ", Stock: " << stock << ", Price: $" << price << std::endl;
    }
};

// 用户类
class User {
private:
    std::string id;
    std::string username;
    std::string contactInfo;
    std::vector<std::string> notifications;

public:
    User(std::string id, std::string username, std::string contactInfo)
        : id(std::move(id)), username(std::move(username)), contactInfo(std::move(contactInfo)) {}

    std::string getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getContactInfo() const { return contactInfo; }

    void addNotification(const std::string& notification) {
        notifications.push_back(notification);
    }

    void viewNotifications() {
        std::cout << "Notifications for " << username << ":" << std::endl;
        for (const auto& notification : notifications) {
            std::cout << notification << std::endl;
        }
        notifications.clear();
    }
};

// 事件数据结构
struct EventData {
    std::string itemName;
    int stockChange;
    double priceChange;

    EventData(std::string name, int stock = 0, double price = 0.0)
        : itemName(std::move(name)), stockChange(stock), priceChange(price) {}

    friend std::ostream& operator<<(std::ostream& os, const EventData& data) {
        os << "Item Name: " << data.itemName << ", Stock Change: " << data.stockChange << ", Price Change: " << data.priceChange;
        return os;
    }
};

// 事件类模板
template <typename T>
class Event {
public:
    enum Type { ITEM_CREATED, STOCK_UPDATED, PRICE_UPDATED };

private:
    Type type;
    T relatedData;
    std::string activityName;
    time_t triggerTime;

public:
    Event(Type type, T relatedData, std::string activityName = "")
        : type(type), relatedData(std::move(relatedData)), activityName(std::move(activityName)), triggerTime(time(0)) {}

    Type getType() const { return type; }
    const T& getData() const { return relatedData; }
    const std::string& getActivityName() const { return activityName; }
    time_t getTime() const { return triggerTime; }

    void display() const {
        std::ostringstream oss;
        oss << "Event Type: " << type << ", Data: " << relatedData << ", Activity: " << activityName << ", Time: " << triggerTime;
        std::cout << oss.str() << std::endl;
    }
};

// 事件管理器类模板
template <typename T>
class EventManager {
private:
    std::vector<Event<T>> events;
    std::unordered_map<typename Event<T>::Type, std::unordered_map<std::string, std::vector<User*>>> subscribers;

public:
    void subscribe(User* user, typename Event<T>::Type eventType, const std::string& itemName) {
        subscribers[eventType][itemName].push_back(user);
    }

    void addNotification(const Event<T>& event) {
        std::ostringstream oss;
        if (typeid(T) == typeid(EventData)) {
            const EventData& data = event.getData();
            for (auto* user : subscribers[event.getType()][data.itemName]) {
                if (user) {
                    oss.str("");
                    oss << "Event Type: " << event.getType() << ", Item: " << data.itemName
                        << ", Stock Change: " << data.stockChange << ", Price Change: " << data.priceChange;
                    user->addNotification(oss.str());
                }
            }
        } else if (typeid(T) == typeid(std::string)) {
            for (auto* user : subscribers[event.getType()]["NEW_ITEMS"]) {
                if (user) {
                    oss.str("");
                    oss << "New item event: " << event.getData();
                    user->addNotification(oss.str());
                }
            }
        }
    }

    void triggerEvent(const Event<T>& event) {
        events.push_back(event);
        addNotification(event);
    }
};

// 检查商品是否存在
bool itemExists(const std::vector<Item<int>>& items, const std::string& itemName) {
    for (const auto& item : items) {
        if (item.getName() == itemName) {
            return true;
        }
    }
    return false;
}

// 示例用例
int main() {
    std::vector<Item<int>> items = {
        Item<int>("Item1", 50, 10.0),
        Item<int>("Item2", 100, 20.0),
        Item<int>("Item3", 150, 30.0),
        Item<int>("Item4", 200, 40.0),
        Item<int>("Item5", 250, 50.0)
    };
    
    std::list<User> users = {
        User("1", "haotian", "zihan9198@gmail.com"),
        User("2", "zhaihaotian", "2022030148@buct.edu.cn")
    };

    EventManager<EventData> eventManagerData;
    EventManager<std::string> eventManagerString;

    for (auto& user : users) {
        eventManagerString.subscribe(&user, Event<std::string>::ITEM_CREATED, "NEW_ITEMS");
    }

    std::string input;

    while (true) {
        std::cout << "Enter 'admin' for manager or 'user' for customer (or 'exit' to quit): ";
        std::cin >> input;

        if (input == "admin") {
            while (true) {
                std::cout << "Enter 'new' to add item, 'list' to show items, 'price_update' to update price, 'stock_update' to update stock, or 'back' to go back: ";
                std::cin >> input;
                if (input == "new") {
                    std::string name;
                    int stock;
                    double price;
                    std::cout << "Enter item name: ";
                    std::cin >> name;

                    if (itemExists(items, name)) {
                        std::cout << "Item already exists." << std::endl;
                        continue;
                    }

                    std::cout << "Enter stock: ";
                    std::cin >> stock;
                    std::cout << "Enter price: ";
                    std::cin >> price;
                    items.emplace_back(name, stock, price);
                    Event<std::string> newItem(Event<std::string>::ITEM_CREATED, name, "New Item Added");
                    eventManagerString.triggerEvent(newItem);
                } else if (input == "list") {
                    for (const auto& item : items) {
                        item.display();
                    }
                } else if (input == "price_update") {
                    std::string name;
                    double priceChange;
                    std::cout << "Enter item name: ";
                    std::cin >> name;
                    if (!itemExists(items, name)) {
                        std::cout << "Item does not exist." << std::endl;
                        continue;
                    }
                    std::cout << "Enter new price: ";
                    std::cin >> priceChange;
                    for (auto& item : items) {
                        if (item.getName() == name) {
                            item.updatePrice(priceChange);
                            EventData data(name, 0, priceChange);
                            Event<EventData> priceUpdate(Event<EventData>::PRICE_UPDATED, data, "Price Change");
                            eventManagerData.triggerEvent(priceUpdate);
                            break;
                        }
                    }
                } else if (input == "stock_update") {
                    std::string name;
                    int stockChange;
                    std::cout << "Enter item name: ";
                    std::cin >> name;
                    if (!itemExists(items, name)) {
                        std::cout << "Item does not exist." << std::endl;
                        continue;
                    }
                    std::cout << "Enter stock change: ";
                    std::cin >> stockChange;
                    for (auto& item : items) {
                        if (item.getName() == name) {
                            item.updateStock(stockChange);
                            EventData data(name, stockChange, 0.0);
                            Event<EventData> stockUpdate(Event<EventData>::STOCK_UPDATED, data, "Stock Change");
                            eventManagerData.triggerEvent(stockUpdate);
                            break;
                        }
                    }
                } else if (input == "back") {
                    break;
                }
            }
        } else if (input == "user") {
            while (true) {
                std::cout << "Enter 'id' to enter user ID, 'new' to create a new user, 'list' to view all users, or 'back' to go back: ";
                std::cin >> input;

                if (input == "id") {
                    std::string userId;
                    std::cout << "Enter user ID: ";
                    std::cin >> userId;

                    User* currentUser = nullptr;
                    for (auto& user : users) {
                        if (user.getId() == userId) {
                            currentUser = &user;
                            break;
                        }
                    }
                    if (!currentUser) {
                        std::cout << "User not found." << std::endl;
                        continue;
                    }

                    while (true) {
                        std::cout << "Enter 'subscribe' to subscribe to an item, 'view' to view notifications, 'list_items' to view all items,  or 'back' to go back: ";
                        std::cin >> input;

                        if (input == "subscribe") {
                            std::string itemName;
                            std::cout << "Enter item name to subscribe: ";
                            std::cin >> itemName;
                            if (!itemExists(items, itemName)) {
                                std::cout << "Item does not exist." << std::endl;
                                continue;
                            }
                            std::cout << "Enter event type (1 for STOCK_UPDATED, 2 for PRICE_UPDATED): ";
                            int eventType;
                            std::cin >> eventType;

                            if (eventType == Event<EventData>::PRICE_UPDATED || eventType == Event<EventData>::STOCK_UPDATED) {
                                eventManagerData.subscribe(currentUser, static_cast<Event<EventData>::Type>(eventType), itemName);
                            } 
                        } else if (input == "view") {
                            currentUser->viewNotifications();
                        } else if (input == "list_items") {
                            for (const auto& item : items) {
                                item.display();
                            }
                        } 
                         else if (input == "back") {
                            break;
                        }
                    }
                } else if (input == "new") {
                    std::string username, contactInfo;
                    std::cout << "Enter username: ";
                    std::cin >> username;
                    std::cout << "Enter contact info: ";
                    std::cin >> contactInfo;
                    users.emplace_back(std::to_string(users.size() + 1), username, contactInfo);
                    User* currentUser = &users.back();
                    eventManagerString.subscribe(currentUser, Event<std::string>::ITEM_CREATED, "NEW_ITEMS");  // 默认订阅新商品上架事件

                    while (true) {
                        std::cout << "Enter 'subscribe' to subscribe to an item, 'view' to view notifications, 'list_items' to view all items, or 'back' to go back: ";
                        std::cin >> input;

                        if (input == "subscribe") {
                            std::string itemName;
                            std::cout << "Enter item name to subscribe: ";
                            std::cin >> itemName;
                            if (!itemExists(items, itemName)) {
                                std::cout << "Item does not exist." << std::endl;
                                continue;
                            }
                            std::cout << "Enter event type (1 for STOCK_UPDATED, 2 for PRICE_UPDATED): ";
                            int eventType;
                            std::cin >> eventType;

                            if (eventType == Event<EventData>::PRICE_UPDATED || eventType == Event<EventData>::STOCK_UPDATED) {
                                eventManagerData.subscribe(currentUser, static_cast<Event<EventData>::Type>(eventType), itemName);
                            } 
                        } else if (input == "view") {
                            currentUser->viewNotifications();
                        } else if (input == "list_items") {
                            for (const auto& item : items) {
                                item.display();
                            }
                        } 
                         else if (input == "back") {
                            break;
                        }
                    }
                } else if (input == "list") {
                    std::cout << "User List:" << std::endl;
                    for (const auto& user : users) {
                        std::cout << "ID: " << user.getId() << ", Name: " << user.getUsername() << ", Email: " << user.getContactInfo() << std::endl;
                    }
                } else if (input == "back") {
                    break;
                }
            }
        } else if (input == "exit") {
            break;
        }
    }
    return 0;
}