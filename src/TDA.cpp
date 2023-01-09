#include "TDA.h"
#include <conio.h>
#include <iostream>
//#include "curses.h"
#include "rapidjson/document.h"
#include <thread>
#include <cstdlib>
#include <algorithm>

#define TICKER              "ACB"

#define ORDER_NUM 5
#define PROFIT 0.05
#define STOPPRICEOFFSET 0.02
#define PRICEGAP 0.01
#define QUANTITY 3

#define EPS 1e-5

TDA::TDA (Account& account_in)
    : account(account_in)
{
    // Clean ordersList
    ordersList.clear();
}

TDA::~TDA() {
}

void TDA::disp_Trading() {

    std::string rlt;

    this->account.refresh_account();
    // Display account info
    std::cout << "------------Account Info-----------" << std::endl;
    std::cout << "Account ID: " << this->account.account_id() << std::endl;
    std::cout << "Account Type: " << this->account.account_type() << std::endl;
    std::cout << "Round Trips: " << this->account.round_trips() << std::endl;
    std::cout << "Restriction: " << (this->account.is_closing_only_restricted() ? "Restricted" : "Active") << std::endl;
    std::cout << "DayTrader: " << (this->account.is_day_trader()? "DayTrader" : "Not DayTrader") << std::endl;

    this->refresh_balances();
    this->refresh_positions();

    // Get stock price
    rlt = this->account.getQuote(TICKER);

    if( rlt == "Request Error!" || rlt == "Refresh Token!") {
        std::cout << rlt << std::endl;
        return;
    }

    rapidjson::Document quote_result;
    quote_result.Parse(rlt.c_str(), rlt.length());

    std::cout << "-----------Refresh Symbol----------" << std::endl;
    std::cout << "Symbol: " << quote_result[TICKER]["symbol"].GetString() << std::endl;
    std::cout << "Quote-Time: " << quote_result[TICKER]["quoteTimeInLong"].GetInt64() << std::endl;
    std::cout << "Last Price: " << quote_result[TICKER]["lastPrice"].GetDouble() << std::endl;
    std::cout << "Last Size: " << quote_result[TICKER]["lastSize"].GetInt() << std::endl;
    std::cout << "Bid Price: " << quote_result[TICKER]["bidPrice"].GetDouble() << std::endl;
    std::cout << "Bid Size: " << quote_result[TICKER]["bidSize"].GetInt() << std::endl;
    std::cout << "Ask Price: " << quote_result[TICKER]["askPrice"].GetDouble() << std::endl;
    std::cout << "Ask Size: " << quote_result[TICKER]["askSize"].GetInt() << std::endl;

    double price = (int)(((quote_result[TICKER]["bidPrice"].GetDouble() + quote_result[TICKER]["askPrice"].GetDouble() + 0.01) / 2.0 ) * 100) / 100.0;

    // Get Orders By Path
    std::cout << "--------Get Orders By Path--------" << std::endl;

    rlt = this->account.getOdersByPath();

    if( rlt == "Request Error!" || rlt == "Refresh Token!") {
        std::cout << rlt << std::endl;
        return;
    }

    rapidjson::Document orders_result;
    orders_result.Parse(rlt.c_str(), rlt.length());

    // Add orders to ordersList
    this->ordersList.clear();

    for (auto& order : orders_result.GetArray()) {
        if (order["orderStrategyType"].GetString() == std::string("TRIGGER")) {
            if(((order["status"].GetString() == std::string("PENDING_ACTIVATION")) ||
                (order["status"].GetString() == std::string("WORKING")) ||
                ((order["status"].GetString() == std::string("FILLED")) &&
                ((order["childOrderStrategies"][0]["status"].GetString() == std::string("PENDING_ACTIVATION")) || (order["childOrderStrategies"][0]["status"].GetString() == std::string("WORKING")))))) {
                OrdersList tempOrder;
                tempOrder.orderID = std::to_string(order["orderId"].GetInt64());
                tempOrder.status = order["status"].GetString();
                tempOrder.orderStrategyType = order["orderStrategyType"].GetString();
                tempOrder.price = (double)order["price"].GetDouble();
                //tempOrder.childOrderPrice = (double)order["childOrderStrategies"][0]["price"].GetDouble();

                //std::cout << tempOrder.orderID << " " << tempOrder.status << " " << tempOrder.orderStrategyType << " " << tempOrder.price << " "<< order["childOrderStrategies"][0]["status"].GetString()<< " " << tempOrder.childOrderPrice << std::endl;
                //std::cout << tempOrder.orderID << " " << tempOrder.status << " " << tempOrder.orderStrategyType << " " << tempOrder.price << " "<< order["childOrderStrategies"][0]["status"].GetString() << std::endl;

                if((((double)order["price"].GetDouble() + EPS >= (price + ORDER_NUM * PRICEGAP)) || ((double)order["price"].GetDouble() + EPS < (price - 2 * ORDER_NUM * PRICEGAP))) && (order["status"].GetString() != std::string("FILLED"))) {
                    rlt = this->account.cancelOrder(std::to_string(order["orderId"].GetInt64()));
                    if( rlt == "Request Error!" || rlt == "Refresh Token!") {
                        std::cout << rlt << std::endl;
                        return;
                    }
                } else {
                    this->ordersList.push_back(tempOrder);
                }
            }
        }
    }
    sort(ordersList.begin(), ordersList.end());
    std::reverse(ordersList.begin(), ordersList.end());
    for (auto orderMember = this->ordersList.begin(); orderMember != this->ordersList.end(); orderMember++) {
        std::cout << orderMember->orderID << " " << orderMember->status << " " << orderMember->orderStrategyType << " " << orderMember->price << std::endl;
    }
    //Place Orders

    std::cout << "-----------Place Orders------------" << std::endl;

    for (int i = 0; i < ORDER_NUM; i++) {
        //this->account.placeOTOOrder(TICKER, price - PRICEGAP * (i + 1), PROFIT, QUANTITY);
        bool existFlag = false;
        for (auto orderMember = this->ordersList.begin(); orderMember != this->ordersList.end(); orderMember++) {
            if (abs(orderMember->price - price + PRICEGAP * (i + 1)) < EPS ){
                std::cout << orderMember->orderID << " is exist with the price " << orderMember->price << "." << std::endl;
                existFlag = true;
                break;
            }
        }

        if (!existFlag) {
            if (this->account.account_type() == "CASH") {
                if (this->account.projected_cash_available_for_trading() + EPS < (price - PRICEGAP * (i + 1)) * QUANTITY) {
                    std::cout << "Available funds for trading is not enough."<< std::endl;
                    break;
                } else {
                    std::cout << price - PRICEGAP * (i + 1) << " level is placed!" << std::endl;
                    rlt = this->account.placeOTOTAOrder(TICKER, price - PRICEGAP * (i + 1), PROFIT, QUANTITY, STOPPRICEOFFSET);
                    if( rlt == "Request Error!" || rlt == "Refresh Token!") {
                        std::cout << rlt << std::endl;
                        return;
                    }
                }
            } else {
                if (this->account.current_buying_power() + EPS < (price - PRICEGAP * (i + 1)) * QUANTITY) {
                    std::cout << "Available funds for trading is not enough."<< std::endl;
                    break;
                } else {
                    std::cout << price - PRICEGAP * (i + 1) << " level is placed!" << std::endl;
                    rlt = this->account.placeOTOTAOrder(TICKER, price - PRICEGAP * (i + 1), PROFIT, QUANTITY, STOPPRICEOFFSET);
                    if( rlt == "Request Error!" || rlt == "Refresh Token!") {
                        std::cout << rlt << std::endl;
                        return;
                    }
                }
            }
            rlt = this->account.refresh_account();
        }
    }
}

void TDA::run_loop() {

    int keypress;

    // Be nice.
    std:: cout << "Welcome to TDAmeritrade!" << std::endl;
    if (this->account.get_authorization_status() == OAuthStatus::Valid) {
        std::cout << "[Connected to Brokerage Account]" << std::endl;
    }
    else {
        std::cout << "[Not Connected]" << std::endl;
        return;
    }

    this->account.refresh_account();
    // Display account info
    std::cout << "------------Account Info-----------" << std::endl;
    std::cout << "Account ID: " << this->account.account_id() << std::endl;
    std::cout << "Account Type: " << this->account.account_type() << std::endl;
    std::cout << "Round Trips: " << this->account.round_trips() << std::endl;
    std::cout << "Restriction: " << (this->account.is_closing_only_restricted() ? "Restricted" : "Active") << std::endl;
    std::cout << "DayTrader: " << (this->account.is_day_trader()? "DayTrader" : "Not DayTrader") << std::endl;

    // Handle user input.
    do {
        switch(keypress) {
        case 'r':
            //Display balances, positions and orders
            this->account.refresh_account();
            this->refresh_balances();
            this->refresh_positions();
            this->refresh_orders();
            break;
        case 's':
            this->refresh_symbol();
            break;
        case 'h':
            this->refresh_history();
            break;
        case 'c':
            this->cancel_Orders();
            break;
        case 'p':
            this->place_Orders();
            break;

        /*case KEY_UP:
        case KEY_DOWN:
            std::cout << "up/down! " << std::endl;
            break;
        */
        default:
            break;
        }
    }
    while((keypress = getch()) != 'q');

}

void TDA::refresh_balances() {
    std::cout << "-------------Balances--------------" << std::endl;
    std::string cash_balance = std::to_string(this->account.current_cash_balance());
    std::string available_funds = std::to_string(this->account.projected_cash_available_for_trading());
    std::string stock_buying_power = (std::to_string(this->account.current_buying_power()));

    std::cout << "Cash Balance: " << cash_balance << std::endl;
    std::cout << "Available Funds for Trading:" << available_funds << std::endl;
    std::cout << "Stock Buying Power: " << stock_buying_power << std::endl;

}

void TDA::refresh_positions() {

    std::cout << "------------Positions--------------" << std::endl;

    std::vector<std::string> positions = this->account.positions();

    if (positions.empty()) {
        std::cout << "No positions held." << std::endl;
    }
    else {
        for (std::string position : positions) {
            std::cout << position << std::endl;
        }
    }
}

void TDA::refresh_orders() {

    std::cout << "-----------Recent Orders-----------" << std::endl;

    std::vector<std::string> orders = this->account.orders();

    if (orders.empty()) {
        std::cout << "No recent orders." << std::endl;
    }
    else {
        for (std::string order : orders) {
            std::cout << order << std::endl;
        }
    }
}

void TDA::refresh_symbol() {
    std::string quote = this->account.getQuote(TICKER);

    rapidjson::Document quote_result;
    quote_result.Parse(quote.c_str(), quote.length());

    std::cout << "-----------Refresh Symbol----------" << std::endl;
    std::cout << "Symbol: " << quote_result[TICKER]["symbol"].GetString() << std::endl;
    std::cout << "Quote-Time: " << quote_result[TICKER]["quoteTimeInLong"].GetInt64() << std::endl;
    std::cout << "Last Price: " << quote_result[TICKER]["lastPrice"].GetDouble() << std::endl;
    std::cout << "Last Size: " << quote_result[TICKER]["lastSize"].GetInt() << std::endl;
    std::cout << "Bid Price: " << quote_result[TICKER]["bidPrice"].GetDouble() << std::endl;
    std::cout << "Bid Size: " << quote_result[TICKER]["bidSize"].GetInt() << std::endl;
    std::cout << "Ask Price: " << quote_result[TICKER]["askPrice"].GetDouble() << std::endl;
    std::cout << "Ask Size: " << quote_result[TICKER]["askSize"].GetInt() << std::endl;
}

void TDA::refresh_history() {
    std::string history = this->account.getHistPrice(TICKER);

	std::cout << "----------Refresh history----------" << std::endl;
    std::cout << "History price: " << history << std::endl;
}
void TDA::place_Orders() {
    std::cout << "-----------Place Orders------------" << std::endl;

    std::string quote = this->account.getQuote(TICKER);
    double price;

    rapidjson::Document quote_result;
    quote_result.Parse(quote.c_str(), quote.length());

    price = quote_result[TICKER]["lastPrice"].GetDouble();

    std::cout << price << std::endl;

    for (int i = 0; i < ORDER_NUM; i++) {
        this->account.placeOTTOrder(TICKER, price - PRICEGAP * (i + 1), PROFIT, QUANTITY);
    }
}

void TDA::cancel_Orders() {
    std::cout << "-----------Cancel Orders-----------" << std::endl;
    std::string cancel_res = this->account.cancelOrder("9148777894");
    std::cout << cancel_res << std::endl;
}



