#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <typeinfo>

// 商品类模板
template <typename T>
class Item {
private:
    std::string name;
    T stock;
    double price;

public:
    Item(std::string name, T stock, double price) : name(name), stock(stock), price(price) {}

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

public:
    User(std::string id, std::string username, std::string contactInfo)
        : id(id), username(username), contactInfo(contactInfo) {}

    std::string getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getContactInfo() const { return contactInfo; }

    void subscribeToEvent(/* 参数 */) {
        // 实现订阅事件逻辑
    }
};

// 事件数据结构
struct EventData {
    std::string itemName;
    int stockChange;
    double priceChange;

    EventData(std::string name, int stock = 0, double price = 0.0)
        : itemName(name), stockChange(stock), priceChange(price) {}

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
        : type(type), relatedData(relatedData), activityName(activityName), triggerTime(time(0)) {}

    Type getType() const { return type; }
    T getData() const { return relatedData; }
    std::string getActivityName() const { return activityName; }
    time_t getTime() const { return triggerTime; }

    void display() const {
        std::cout << "Event Type: " << type << ", Data: " << relatedData << ", Activity: " << activityName << ", Time: " << triggerTime << std::endl;
    }
};

// 事件管理器类模板
template <typename T>
class EventManager {
private:
    std::vector<Event<T> > events;
    std::unordered_map<typename Event<T>::Type, std::unordered_map<std::string, std::vector<User> > > subscribers;

public:
    void subscribe(User user, typename Event<T>::Type eventType, const std::string& itemName) {
        subscribers[eventType][itemName].push_back(user);
    }

    void notify(const Event<T>& event) {
        if (typeid(T) == typeid(EventData)) {
            const EventData& data = event.getData();
            for (const auto& user : subscribers[event.getType()][data.itemName]) {
                std::cout << "Notifying user " << user.getUsername() << " about event: " << std::endl;
                event.display();
            }
        } else if (typeid(T) == typeid(std::string)) {
            for (const auto& user : subscribers[event.getType()]["NEW_ITEMS"]) {
                std::cout << "Notifying user " << user.getUsername() << " about new item event: " << std::endl;
                event.display();
            }
        }
    }

    void triggerEvent(const Event<T>& event) {
        events.push_back(event);
        notify(event);
    }
};

// 示例用例
int main() {
    // 创建商品
    Item<int> apple("Apple", 100, 0.5);
    Item<int> pear("Pear", 200, 0.4);

    // 创建用户
    User user1("1", "John Doe", "john@example.com");
    User user2("2", "Jane Smith", "jane@example.com");

    // 创建事件管理器
    EventManager<EventData> eventManagerData;
    EventManager<std::string> eventManagerString;

    // 用户订阅特定商品的特定事件
    eventManagerData.subscribe(user1, Event<EventData>::PRICE_UPDATED, "Apple");
    eventManagerData.subscribe(user2, Event<EventData>::STOCK_UPDATED, "Pear");

    // 默认所有用户订阅新商品上架事件
    eventManagerString.subscribe(user1, Event<std::string>::ITEM_CREATED, "NEW_ITEMS");
    eventManagerString.subscribe(user2, Event<std::string>::ITEM_CREATED, "NEW_ITEMS");

    // 苹果价格更新事件
    EventData appleData("Apple", 0, 0.1);  // 价格变化
    Event<EventData> applePriceUpdate(Event<EventData>::PRICE_UPDATED, appleData, "Seasonal Discount");
    eventManagerData.triggerEvent(applePriceUpdate);  // 通知 user1

    // 梨库存更新事件
    EventData pearData("Pear", -50, 0.0);  // 库存变化
    Event<EventData> pearStockUpdate(Event<EventData>::STOCK_UPDATED, pearData, "Inventory Adjustment");
    eventManagerData.triggerEvent(pearStockUpdate);  // 通知 user2

    // 新商品上架事件
    Event<std::string> newItem(Event<std::string>::ITEM_CREATED, "New Product", "New Product Launch");
    eventManagerString.triggerEvent(newItem);  // 通知所有用户

    return 0;
}
