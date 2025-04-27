#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cctype>
#include <iomanip>

using namespace std;
// Constants
const int MAX_PRODUCTS = 100;
const int MAX_CART_ITEMS = 100;
const int MAX_ORDERS = 100;

// Product class
class Product {
private:
    string id;
    string name;
    double price;

public:
    Product() : id(""), name(""), price(0.0) {}
    Product(string id, string name, double price) : id(id), name(name), price(price) {}

    // Getters
    string getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }

    // Setters with validation
    void setId(string newId) {
        if (!newId.empty()) {
            id = newId;
        }
    }

    void setName(string newName) {
        if (!newName.empty()) {
            name = newName;
        }
    }

    void setPrice(double newPrice) {
        if (newPrice >= 0) {
            price = newPrice;
        }
    }

    void display() const {
        cout << left << setw(10) << id 
                  << setw(15) << name 
                  << "$" << setw(8) <<  price 
                  << left << endl;
    }
};

// Shopping Cart Item class
class CartItem {
private:
    Product product;
    int quantity;

public:
    CartItem() : quantity(0) {}
    CartItem(Product p, int qty) : product(p), quantity(qty) {}

    Product getProduct() const { return product; }
    int getQuantity() const { return quantity; }

    void setQuantity(int newQty) {
        if (newQty > 0) {
            quantity = newQty;
        }
    }

    double getTotalPrice() const {
        return product.getPrice() * quantity;
    }

    void display() const {
        cout << left << setw(10) << product.getId() 
                  << setw(15) << product.getName() 
                  << "$" << setw(8) <<  product.getPrice()
                  << setw(10) << right << quantity << endl;
    }
};

// Order class
class Order {
private:
    string orderId;
    CartItem items[MAX_CART_ITEMS];
    int itemCount;
    double totalAmount;
    string paymentMethod;

public:
    Order() : orderId(""), itemCount(0), totalAmount(0.0), paymentMethod("") {}

    Order(string id, CartItem cart[], int count, double total, string payment) 
        : orderId(id), itemCount(count), totalAmount(total), paymentMethod(payment) {
        for (int i = 0; i < count; i++) {
            items[i] = cart[i];
        }
    }

    string getOrderId() const { return orderId; }
    double getTotalAmount() const { return totalAmount; }
    string getPaymentMethod() const { return paymentMethod; }
    int getItemCount() const { return itemCount; }
    CartItem getItem(int index) const { return items[index]; }

    void display() const {
        cout << "\n----------------------------------------\n";
        cout << "Order ID: " << orderId << "\n";
        cout << "Total Amount: " << fixed << setprecision(2) << totalAmount << "\n";
        cout << "Payment Method: " << paymentMethod << "\n";
        cout << "Order Details:\n";
        cout << left 
                  << setw(12) << "Product ID"
                  << setw(18) << "Name"
                  << setw(12) << "Price"
                  << "Quantity" << "\n";
        
        for (int i = 0; i < itemCount; i++) {
            ostringstream priceStream;
            priceStream << "$" << fixed << setprecision(2) << items[i].getProduct().getPrice();
            
            cout << left 
                      << setw(12) << items[i].getProduct().getId()
                      << setw(18) << items[i].getProduct().getName()
                      << setw(12) << priceStream.str()  // Use the formatted price string
                      << setw(10) << items[i].getQuantity() << "\n";
        }
        cout << "----------------------------------------\n";
    }
};

// Payment Strategy Interface
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() {}
    virtual void pay(double amount) = 0;
    virtual string getMethodName() const = 0;
};

// Concrete Payment Strategies
class CashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid $" << amount << " in cash.\n";
    }
    
    string getMethodName() const override {
        return "Cash";
    }
};

class CardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid $" << amount << " using Credit/Debit Card.\n";
    }
    
    string getMethodName() const override {
        return "Credit / Debit Card";
    }
};

class GCashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid $" << amount << " using GCash.\n";
    }
    
    string getMethodName() const override {
        return "GCash";
    }
};

// Payment Context (Strategy Pattern)
class PaymentContext {
private:
    PaymentStrategy* strategy;

public:
    PaymentContext() : strategy(nullptr) {}
    ~PaymentContext() {
        delete strategy;
    }

    void setStrategy(PaymentStrategy* newStrategy) {
        delete strategy;
        strategy = newStrategy;
    }

    void executePayment(double amount) {
        if (strategy) {
            strategy->pay(amount);
        }
    }

    string getPaymentMethod() const {
        if (strategy) {
            return strategy->getMethodName();
        }
        return "Unknown";
    }
};

// Order Manager (Singleton Pattern)
class OrderManager {
private:
    static OrderManager* instance;
    Order orders[MAX_ORDERS];
    int orderCount;
    
    OrderManager() : orderCount(0) {
        // Private constructor for singleton
    }

public:
    static OrderManager* getInstance() {
        if (!instance) {
            instance = new OrderManager();
        }
        return instance;
    }

    void addOrder(const Order& order) {
        if (orderCount < MAX_ORDERS) {
            orders[orderCount++] = order;
        }
    }

    void displayAllOrders() const {
        if (orderCount == 0) {
            cout << "\nNo orders yet.\n";
            return;
        }

        for (int i = 0; i < orderCount; i++) {
            orders[i].display();
        }
    }
};

OrderManager* OrderManager::instance = nullptr;

// Utility functions
string generateOrderId() {
    static int counter = 1;
    return to_string(counter++);
}

bool validateYesNo(const string& input) {
    return (input == "Y" || input == "y" || input == "N" || input == "n");
}

bool validateNumeric(const string& input) {
    for (char c : input) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

int getValidatedInt(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (validateNumeric(input)) {
            return stoi(input);
        }
        cout << "Invalid input. Please enter a number.\n";
    }
}

string getValidatedYesNo(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (validateYesNo(input)) {
            return toupper(input[0]) == 'Y' ? "Y" : "N";
        }
        cout << "Invalid input. Please enter Y or N.\n";
    }
}

// E-Commerce System
class ECommerceSystem {
private:
    Product products[MAX_PRODUCTS];
    CartItem shoppingCart[MAX_CART_ITEMS];
    int productCount;
    int cartItemCount;

    void initializeProducts() {
        // Initialize with some sample products
        products[0] = Product("P100", "Laptop", 999.99);
        products[1] = Product("P101", "Smartphone", 699.99);
        products[2] = Product("P102", "Headphones", 149.99);
        products[3] = Product("P103", "Mouse", 24.99);
        products[4] = Product("P104", "Keyboard", 49.99);
        productCount = 5;
    }

    void displayProducts() const {
        cout << "\nAvailable Products:\n";
        cout << left << setw(10) << "ID" 
                  << setw(15) << "Name" 
                  << "Price" << endl;
        cout << "--------------------------------\n";
        for (int i = 0; i < productCount; i++) {
            products[i].display();
        }
        cout << "--------------------------------\n";
    }

    void addToCart() {
        string input;
        bool addMore = true;
        
        while (addMore && cartItemCount < MAX_CART_ITEMS) {
            displayProducts();
            
            string productId;
            bool found = false;
            
            while (!found) {
                cout << "Enter the ID of the product you want to add to the shopping cart: ";
                getline(cin, productId);
                
                for (int i = 0; i < productCount; i++) {
                    if (products[i].getId() == productId) {
                        found = true;
                        
                        // Check if product already in cart
                        bool alreadyInCart = false;
                        for (int j = 0; j < cartItemCount; j++) {
                            if (shoppingCart[j].getProduct().getId() == productId) {
                                alreadyInCart = true;
                                shoppingCart[j].setQuantity(shoppingCart[j].getQuantity() + 1);
                                break;
                            }
                        }
                        
                        if (!alreadyInCart) {
                            shoppingCart[cartItemCount++] = CartItem(products[i], 1);
                        }
                        
                        cout << "Product added successfully!\n";
                        break;
                    }
                }
                
                if (!found) {
                    cout << "Product ID not found. Please try again.\n";
                }
            }
            
            string choice = getValidatedYesNo("Do you want to add another product? (Y/N): ");
            addMore = (choice == "Y");
        }
    }

    void viewCart() {
        if (cartItemCount == 0) {
            cout << "\nYour shopping cart is empty.\n";
            return;
        }
        
        cout << "\nYour Shopping Cart:\n";
        cout << left << setw(10) << "ID" 
                  << setw(15) << "Name" 
                  << setw(10) << "Price" 
                  << "Quantity" << endl;
        cout << "---------------------------------------------\n";
        
        double total = 0.0;
        for (int i = 0; i < cartItemCount; i++) {
            shoppingCart[i].display();
            total += shoppingCart[i].getTotalPrice();
        }
        
        cout << "---------------------------------------------\n";
        cout << "Total: $" << fixed << setprecision(2) << total << "\n";
        
        string choice = getValidatedYesNo("\nDo you want to check out all the products? (Y/N): ");
        if (choice == "Y") {
            checkout(total);
        }
    }

    void checkout(double total) {
        if (cartItemCount == 0) {
            cout << "No items to checkout.\n";
            return;
        }
        
        cout << "\nProducts for Checkout:\n";
        cout << left << setw(10) << "ID" 
                  << setw(15) << "Name" 
                  << setw(10) << "Price" 
                  << "Quantity" << endl;
        cout << "--------------------------------\n";
        
        for (int i = 0; i < cartItemCount; i++) {
            shoppingCart[i].display();
        }
        
        cout << "--------------------------------\n";
        cout << "Total Amount: $" << total << "\n\n";
        
        // Select payment method
        PaymentContext paymentContext;
        
        cout << "Select Payment Method:\n";
        cout << "1. Cash\n";
        cout << "2. Credit/Debit Card\n";
        cout << "3. GCash\n";
        
        int choice = getValidatedInt("Enter your choice (1-3): ");
        while (choice < 1 || choice > 3) {
            cout << "Invalid choice. Please enter a number between 1 and 3.\n";
            choice = getValidatedInt("Enter your choice (1-3): ");
        }
        
        switch (choice) {
            case 1: paymentContext.setStrategy(new CashPayment()); break;
            case 2: paymentContext.setStrategy(new CardPayment()); break;
            case 3: paymentContext.setStrategy(new GCashPayment()); break;
        }
        
        paymentContext.executePayment(total);
        
        // Create order
        string orderId = generateOrderId();
        Order newOrder(orderId, shoppingCart, cartItemCount, total, paymentContext.getPaymentMethod());
        
        // Add to order manager
        OrderManager::getInstance()->addOrder(newOrder);
        
        // Log the order
        logOrder(orderId, paymentContext.getPaymentMethod());
        
        // Clear the cart
        cartItemCount = 0;
        
        cout << "\nYou have successfully checked out the products!\n";
    }

    void logOrder(const string& orderId, const string& paymentMethod) {
        ofstream logFile("orders.log", ios::app);
        if (logFile.is_open()) {
            time_t now = time(0);
            tm* ltm = localtime(&now);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
            
            logFile << "[" << buffer << "] Order ID: " << orderId 
                   << " has been successfully checked out and paid using " 
                   << paymentMethod << ".\n";
            logFile.close();
        } else {
            cerr << "Unable to open log file.\n";
        }
    }

    void viewOrders() const {
        cout << "\nOrder History:\n";
        OrderManager::getInstance()->displayAllOrders();
    }

public:
    ECommerceSystem() : productCount(0), cartItemCount(0) {
        initializeProducts();
    }

    void run() {
        while (true) {
            cout << "\n=== Online Store Menu ===\n";
            cout << "1. View Products\n";
            cout << "2. View Shopping Cart\n";
            cout << "3. View Orders\n";
            cout << "4. Exit\n";
            
            int choice = getValidatedInt("Enter your choice (1-4): ");
            
            switch (choice) {
                case 1:
                    addToCart();
                    break;
                case 2:
                    viewCart();
                    break;
                case 3:
                    viewOrders();
                    break;
                case 4:
                    cout << "Thank you for shopping with us!\n";
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main() {
    ECommerceSystem system;
    system.run();
    return 0;
}