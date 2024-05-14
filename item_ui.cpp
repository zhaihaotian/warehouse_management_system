#include <wx/wx.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

// 商品类模板
template <typename T>
class Item {
private:
    std::string name;
    T stock;
    double price;

public:
    Item(std::string name, T stock, double price) : name(std::move(name)), stock(stock), price(price) {}

    std::string getInfo() const {
        std::ostringstream ss;
        ss << "Item: " << name << ", Stock: " << stock << ", Price: $" << std::fixed << std::setprecision(2) << price;
        return ss.str();
    }

    void updateStock(T amount) {
        stock += amount;
    }

    void updatePrice(double newPrice) {
        price = newPrice;
    }
};

// 应用主窗口
class MainFrame : public wxFrame {
private:
    wxTextCtrl* itemNameCtrl;
    wxTextCtrl* itemStockCtrl;
    wxTextCtrl* itemPriceCtrl;
    wxComboBox* typeChoice;
    wxListBox* itemList;
    std::vector<Item<int>> itemsInt;
    std::vector<Item<double>> itemsDouble;

public:
    MainFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400)) {
        auto* panel = new wxPanel(this, wxID_ANY);
        auto* vbox = new wxBoxSizer(wxVERTICAL);

        itemNameCtrl = new wxTextCtrl(panel, wxID_ANY);
        itemStockCtrl = new wxTextCtrl(panel, wxID_ANY);
        itemPriceCtrl = new wxTextCtrl(panel, wxID_ANY);
        typeChoice = new wxComboBox(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
        itemList = new wxListBox(panel, wxID_ANY);

        typeChoice->Append("int");
        typeChoice->Append("double");
        typeChoice->SetSelection(0); // Default to int

        auto* createButton = new wxButton(panel, wxID_ANY, wxT("Create Item"));
        createButton->Bind(wxEVT_BUTTON, &MainFrame::OnCreateItem, this);

        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Item Name:")), 0, wxALL, 5);
        vbox->Add(itemNameCtrl, 0, wxEXPAND | wxALL, 5);
        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Stock:")), 0, wxALL, 5);
        vbox->Add(itemStockCtrl, 0, wxEXPAND | wxALL, 5);
        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Price:")), 0, wxALL, 5);
        vbox->Add(itemPriceCtrl, 0, wxEXPAND | wxALL, 5);
        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Stock Type:")), 0, wxALL, 5);
        vbox->Add(typeChoice, 0, wxEXPAND | wxALL, 5);
        vbox->Add(createButton, 0, wxALL, 5);
        vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Items List:")), 0, wxALL, 5);
        vbox->Add(itemList, 1, wxEXPAND | wxALL, 5);

        panel->SetSizer(vbox);
    }

    void OnCreateItem(wxCommandEvent& event) {
        std::string name = itemNameCtrl->GetValue().ToStdString();
        double price = std::stod(itemPriceCtrl->GetValue().ToStdString());
        std::string type = typeChoice->GetStringSelection().ToStdString();
        if (type == "int") {
            int stock = std::stoi(itemStockCtrl->GetValue().ToStdString());
            itemsInt.emplace_back(name, stock, price);
            itemList->AppendString(itemsInt.back().getInfo());
        } else if (type == "double") {
            double stock = std::stod(itemStockCtrl->GetValue().ToStdString());
            itemsDouble.emplace_back(name, stock, price);
            itemList->AppendString(itemsDouble.back().getInfo());
        }
    }
};

class ItemApp : public wxApp {
public:
    virtual bool OnInit() {
        MainFrame* mainFrame = new MainFrame(wxT("Item Management System"));
        mainFrame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(ItemApp);
