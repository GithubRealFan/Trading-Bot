#ifndef TDA_H
#define TDA_H

#include "account.h"
struct OrdersList{
    std::string orderStrategyType;
    std::string status;
    std::string orderID;
    double price;

    bool operator < (const OrdersList& order) {
        return (price < order.price);
    }
};

class TDA
{
public:
    TDA(Account& account_in);
    ~TDA();

    void disp_Trading();
    void run_loop(); //develop for testing

private:
    Account& account;
    std::shared_ptr<Rest> rest_interface;
    std::shared_ptr<OAuth> authorization;
    std::vector<OrdersList> ordersList;

    void refresh_balances();
    void refresh_positions();
    void refresh_orders();
    void refresh_symbol();
    void refresh_history();
    void place_Orders();
    void cancel_Orders();

};

#endif
