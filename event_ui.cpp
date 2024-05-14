#include <wx/wx.h>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>

// 定义事件数据结构
struct EventData {
    std::string itemName;
    int stockChange;
    double priceChange;

    EventData(std::string name, int stock = 0, double price = 0.0)
        : itemName(std::move(name)), stockChange(stock), priceChange(price) {}
};

// 事件类模板
template <typename T>
class Event {
public:
    enum Type { ITEM_CREATED, STOCK_UPDATED, PRICE_UPDATED };

private:
    Type type;
    T data;
    std::string description;
    std::time_t time;

public:
    Event(Type type, T data, std::string description = "")
        : type(type), data(std::move(data)), description(std::move(description)), time(std::time(nullptr)) {}

    std::string getInfo() const {
        std::ostringstream ss;
        ss << "Type: " << type << ", Data: " << description << ", Time: " << std::ctime(&time);
        return ss.str();
    }
};

// 主窗口
class MainFrame : public wxFrame {
private:
    wxTextCtrl* itemNameCtrl;
    wxTextCtrl* itemStockCtrl;
    wxTextCtrl* itemPriceCtrl;
    wxListBox* eventList;
    wxComboBox* eventTypeChoice;

public:
    MainFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400)) {
        auto* panel = new wxPanel(this, wxID_ANY);
        auto* vbox = new wxBoxSizer(wxVERTICAL);

        eventTypeChoice = new wxComboBox(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
        eventTypeChoice->Append("Item Created");
        eventTypeChoice->Append("Stock Updated");
        eventTypeChoice->Append("Price Updated");
        eventTypeChoice->SetSelection(0);

        itemNameCtrl = new wxTextCtrl(panel, wxID_ANY);
        itemStockCtrl = new wxTextCtrl(panel, wxID_ANY);
        itemPriceCtrl = new wxTextCtrl(panel, wxID_ANY);
        eventList = new wxListBox(panel, wxID_ANY);

        auto* createEventBtn = new wxButton(panel, wxID_ANY, wxT("Trigger Event"));
        createEventBtn->Bind(wxEVT_BUTTON, &MainFrame::OnCreateEvent, this);

        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Event Type:")), 0, wxALL, 5);
        vbox->Add(eventTypeChoice, 0, wxEXPAND | wxALL, 5);
        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Item Name:")), 0, wxALL, 5);
        vbox->Add(itemNameCtrl, 0, wxEXPAND | wxALL, 5);
        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Stock Change:")), 0, wxALL, 5);
        vbox->Add(itemStockCtrl, 0, wxEXPAND | wxALL, 5);
        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Price Change:")), 0, wxALL, 5);
        vbox->Add(itemPriceCtrl, 0, wxEXPAND | wxALL, 5);
        vbox->Add(createEventBtn, 0, wxALL, 5);
        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Events:")), 0, wxALL, 5);
        vbox->Add(eventList, 1, wxEXPAND | wxALL, 5);

        panel->SetSizer(vbox);
    }

    void OnCreateEvent(wxCommandEvent& event) {
        int typeIndex = eventTypeChoice->GetSelection();
        std::string itemName = itemNameCtrl->GetValue().ToStdString();
        int stockChange = std::stoi(itemStockCtrl->GetValue().ToStdString());
        double priceChange = std::stod(itemPriceCtrl->GetValue().ToStdString());
        EventData data(itemName, stockChange, priceChange);
        
        switch (typeIndex) {
            case 0: // Item Created
                eventList->AppendString("Item Created: " + itemName);
                break;
            case 1: // Stock Updated
                eventList->AppendString("Stock Updated: " + itemName + ", Stock Change: " + std::to_string(stockChange));
                break;
            case 2: // Price Updated
                eventList->AppendString("Price Updated: " + itemName + ", Price Change: " + std::to_string(priceChange));
                break;
        }
    }
};

// 应用程序类
class EventApp : public wxApp {
public:
    virtual bool OnInit() {
        MainFrame* mainFrame = new MainFrame(wxT("Event Management System"));
        mainFrame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(EventApp);
