#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cctype>
#include <iomanip>

// Constants
const int MAX_PRODUCTS = 100;
const int MAX_CART_ITEMS = 100;
const int MAX_ORDERS = 100;

// Product class
class Product {
private:
    std::string id;
    std::string name;
    double price;

public:
    Product() : id(""), name(""), price(0.0) {}
    Product(std::string id, std::string name, double price) : id(id), name(name), price(price) {}

    // Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }

    // Setters with validation
    void setId(std::string newId) {
        if (!newId.empty()) {
            id = newId;
        }
    }

    void setName(std::string newName) {
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
        std::cout << std::left << std::setw(10) << id 
                  << std::setw(15) << name 
                  << "$" << std::setw(8) <<  price 
                  << std::left << std::endl;
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
        std::cout << std::left << std::setw(10) << product.getId() 
                  << std::setw(15) << product.getName() 
                  << "$" << std::setw(8) <<  product.getPrice()
                  << std::setw(10) << std::right << quantity << std::endl;
    }
};

// Order class
class Order {
private:
    std::string orderId;
    CartItem items[MAX_CART_ITEMS];
    int itemCount;
    double totalAmount;
    std::string paymentMethod;

public:
    Order() : orderId(""), itemCount(0), totalAmount(0.0), paymentMethod("") {}

    Order(std::string id, CartItem cart[], int count, double total, std::string payment) 
        : orderId(id), itemCount(count), totalAmount(total), paymentMethod(payment) {
        for (int i = 0; i < count; i++) {
            items[i] = cart[i];
        }
    }

    std::string getOrderId() const { return orderId; }
    double getTotalAmount() const { return totalAmount; }
    std::string getPaymentMethod() const { return paymentMethod; }
    int getItemCount() const { return itemCount; }
    CartItem getItem(int index) const { return items[index]; }

    void display() const {
        std::cout << "\n----------------------------------------\n";
        std::cout << "Order ID: " << orderId << "\n";
        std::cout << "Total Amount: " << std::fixed << std::setprecision(2) << totalAmount << "\n";
        std::cout << "Payment Method: " << paymentMethod << "\n";
        std::cout << "Order Details:\n";
        std::cout << std::left 
                  << std::setw(12) << "Product ID"
                  << std::setw(18) << "Name"
                  << std::setw(12) << "Price"
                  << "Quantity" << "\n";
        
        for (int i = 0; i < itemCount; i++) {
            std::ostringstream priceStream;
            priceStream << "$" << std::fixed << std::setprecision(2) << items[i].getProduct().getPrice();
            
            std::cout << std::left 
                      << std::setw(12) << items[i].getProduct().getId()
                      << std::setw(18) << items[i].getProduct().getName()
                      << std::setw(12) << priceStream.str()  // Use the formatted price string
                      << std::setw(10) << items[i].getQuantity() << "\n";
        }
        std::cout << "----------------------------------------\n";
    }
};

// Payment Strategy Interface
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() {}
    virtual void pay(double amount) = 0;
    virtual std::string getMethodName() const = 0;
};

// Concrete Payment Strategies
class CashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        std::cout << "Paid $" << amount << " in cash.\n";
    }
    
    std::string getMethodName() const override {
        return "Cash";
    }
};

class CardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        std::cout << "Paid $" << amount << " using Credit/Debit Card.\n";
    }
    
    std::string getMethodName() const override {
        return "Credit / Debit Card";
    }
};

class GCashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        std::cout << "Paid $" << amount << " using GCash.\n";
    }
    
    std::string getMethodName() const override {
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

    std::string getPaymentMethod() const {
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
            std::cout << "\nNo orders yet.\n";
            return;
        }

        for (int i = 0; i < orderCount; i++) {
            orders[i].display();
        }
    }
};

OrderManager* OrderManager::instance = nullptr;

// Utility functions
std::string generateOrderId() {
    static int counter = 1;
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y%m%d", ltm);
    
    return std::string(buffer) + "-" + std::to_string(counter++);
}

bool validateYesNo(const std::string& input) {
    return (input == "Y" || input == "y" || input == "N" || input == "n");
}

bool validateNumeric(const std::string& input) {
    for (char c : input) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

int getValidatedInt(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (validateNumeric(input)) {
            return std::stoi(input);
        }
        std::cout << "Invalid input. Please enter a number.\n";
    }
}

std::string getValidatedYesNo(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (validateYesNo(input)) {
            return toupper(input[0]) == 'Y' ? "Y" : "N";
        }
        std::cout << "Invalid input. Please enter Y or N.\n";
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
        std::cout << "\nAvailable Products:\n";
        std::cout << std::left << std::setw(10) << "ID" 
                  << std::setw(15) << "Name" 
                  << "Price" << std::endl;
        std::cout << "--------------------------------\n";
        for (int i = 0; i < productCount; i++) {
            products[i].display();
        }
        std::cout << "--------------------------------\n";
    }

    void addToCart() {
        std::string input;
        bool addMore = true;
        
        while (addMore && cartItemCount < MAX_CART_ITEMS) {
            displayProducts();
            
            std::string productId;
            bool found = false;
            
            while (!found) {
                std::cout << "Enter the ID of the product you want to add to the shopping cart: ";
                std::getline(std::cin, productId);
                
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
                        
                        std::cout << "Product added successfully!\n";
                        break;
                    }
                }
                
                if (!found) {
                    std::cout << "Product ID not found. Please try again.\n";
                }
            }
            
            std::string choice = getValidatedYesNo("Do you want to add another product? (Y/N): ");
            addMore = (choice == "Y");
        }
    }

    void viewCart() {
        if (cartItemCount == 0) {
            std::cout << "\nYour shopping cart is empty.\n";
            return;
        }
        
        std::cout << "\nYour Shopping Cart:\n";
        std::cout << std::left << std::setw(10) << "ID" 
                  << std::setw(15) << "Name" 
                  << std::setw(10) << "Price" 
                  << "Quantity" << std::endl;
        std::cout << "---------------------------------------------\n";
        
        double total = 0.0;
        for (int i = 0; i < cartItemCount; i++) {
            shoppingCart[i].display();
            total += shoppingCart[i].getTotalPrice();
        }
        
        std::cout << "---------------------------------------------\n";
        std::cout << "Total: $" << std::fixed << std::setprecision(2) << total << "\n";
        
        std::string choice = getValidatedYesNo("\nDo you want to check out all the products? (Y/N): ");
        if (choice == "Y") {
            checkout(total);
        }
    }

    void checkout(double total) {
        if (cartItemCount == 0) {
            std::cout << "No items to checkout.\n";
            return;
        }
        
        std::cout << "\nProducts for Checkout:\n";
        std::cout << std::left << std::setw(10) << "ID" 
                  << std::setw(15) << "Name" 
                  << std::setw(10) << "Price" 
                  << "Quantity" << std::endl;
        std::cout << "--------------------------------\n";
        
        for (int i = 0; i < cartItemCount; i++) {
            shoppingCart[i].display();
        }
        
        std::cout << "--------------------------------\n";
        std::cout << "Total Amount: $" << total << "\n\n";
        
        // Select payment method
        PaymentContext paymentContext;
        
        std::cout << "Select Payment Method:\n";
        std::cout << "1. Cash\n";
        std::cout << "2. Credit/Debit Card\n";
        std::cout << "3. GCash\n";
        
        int choice = getValidatedInt("Enter your choice (1-3): ");
        while (choice < 1 || choice > 3) {
            std::cout << "Invalid choice. Please enter a number between 1 and 3.\n";
            choice = getValidatedInt("Enter your choice (1-3): ");
        }
        
        switch (choice) {
            case 1: paymentContext.setStrategy(new CashPayment()); break;
            case 2: paymentContext.setStrategy(new CardPayment()); break;
            case 3: paymentContext.setStrategy(new GCashPayment()); break;
        }
        
        paymentContext.executePayment(total);
        
        // Create order
        std::string orderId = generateOrderId();
        Order newOrder(orderId, shoppingCart, cartItemCount, total, paymentContext.getPaymentMethod());
        
        // Add to order manager
        OrderManager::getInstance()->addOrder(newOrder);
        
        // Log the order
        logOrder(orderId, paymentContext.getPaymentMethod());
        
        // Clear the cart
        cartItemCount = 0;
        
        std::cout << "\nYou have successfully checked out the products!\n";
    }

    void logOrder(const std::string& orderId, const std::string& paymentMethod) {
        std::ofstream logFile("orders.log", std::ios::app);
        if (logFile.is_open()) {
            logFile << "[LOG] -> Order ID: " << orderId 
                   << " has been successfully checked out and paid using " 
                   << paymentMethod << ".\n";
            logFile.close();
        } else {
            std::cerr << "Unable to open log file.\n";
        }
    }

    void viewOrders() const {
        std::cout << "\nOrder History:\n";
        OrderManager::getInstance()->displayAllOrders();
    }

public:
    ECommerceSystem() : productCount(0), cartItemCount(0) {
        initializeProducts();
    }

    void run() {
        while (true) {
            std::cout << "\n=== Online Store Menu ===\n";
            std::cout << "1. View Products\n";
            std::cout << "2. View Shopping Cart\n";
            std::cout << "3. View Orders\n";
            std::cout << "4. Exit\n";
            
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
                    std::cout << "Thank you for shopping with us!\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main() {
    ECommerceSystem system;
    system.run();
    return 0;
}