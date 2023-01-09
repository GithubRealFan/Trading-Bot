#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <memory>
#include <vector>

#include "oauth.h"
#include "rest.h"

#include "rapidjson/document.h"

class Account
{
public:
    Account(std::string account_number_in, std::string oauth_uid_in);

    bool refresh_account();

    // FIXME: populate with rest of available fields.
    std::string account_id();
    std::string account_type();

    unsigned int round_trips();

    bool is_day_trader();
    bool is_closing_only_restricted();

    // Current Balances
    double current_accrued_interest();
    double current_available_funds_non_marginable_trade();
    double current_bond_value();
    double current_buying_power();
    double current_cash_balance();
    double current_cash_available_for_trading();
    double current_cash_reciepts();
    double current_day_trading_buying_power();
    double current_day_trading_buying_power_call();
    double current_day_trading_equity_call();
    double current_equity();
    double current_equity_percentage();
    double current_liquidation_value();
    double current_long_margin_value();
    double current_long_option_market_value();
    double current_long_stock_value();
    double current_maintenance_call();
    double current_maintenance_requirement();
    double current_margin();
    double current_margin_equity();
    double current_money_market_fund();
    double current_mutual_fund_value();
    double current_reg_t_call();
    double current_short_margin_value();
    double current_short_option_market_value();
    double current_short_stock_value();
    double current_total_cash();
    double current_is_in_call();
    double current_unsettled_cash();
    double current_pending_deposits();
    double current_margin_balance();
    double current_short_balance();
    double current_account_value();

    // Initial Balances
    double initial_accrued_interest();
    double initial_available_funds_non_marginable_trade();
    double initial_bond_value();
    double initial_buying_power();
    double initial_cash_balance();
    double initial_cash_available_for_trading();
    double initial_cash_reciepts();
    double initial_day_trading_buying_power();
    double initial_day_trading_buying_power_call();
    double initial_day_trading_equity_call();
    double initial_equity();
    double initial_equity_percentage();
    double initial_liquidation_value();
    double initial_long_margin_value();
    double initial_long_option_market_value();
    double initial_long_stock_value();
    double initial_maintenance_call();
    double initial_maintenance_requirement();
    double initial_margin();
    double initial_margin_equity();
    double initial_money_market_fund();
    double initial_mutual_fund_value();
    double initial_reg_t_call();
    double initial_short_margin_value();
    double initial_short_option_market_value();
    double initial_short_stock_value();
    double initial_total_cash();
    double initial_is_in_call();
    double initial_unsettled_cash();
    double initial_pending_deposits();
    double initial_margin_balance();
    double initial_short_balance();
    double initial_account_value();

    //Projected Balances
    double projected_cash_available_for_trading();
    double projected_cash_available_for_withdrawl();

    OAuthStatus get_authorization_status();
    std::shared_ptr<Rest> get_rest_interface();

    // FIXME: This should really be a collection of position classes, with all

    std::vector<std::string> positions();

    // only supports single limit orders at the moment
    // FIXME: This should really be a collection of order classes, with all

    std::vector<std::string> orders();

    // FIXME: Get the current market price of the stock.
    std::string getQuote(std::string symbol);

    // Place OTO orders

    // Cancel nonworking orders

    /*  getHistPrice()

    Gets the historic price for a given ticker

    params:

    ticker - ticker symbol of company

    periodType - The type of period to show. Valid values are day, month, year, or ytd (year to date).
     Default is day.

    period - The number of periods to show.

     Example: For a 2 day / 1 min chart, the values would be:

     period: 2
     periodType: day
     frequency: 1
     frequencyType: min

     Valid periods by periodType (defaults marked with an asterisk):

     day: 1, 2, 3, 4, 5, 10*
     month: 1*, 2, 3, 6
     year: 1*, 2, 3, 5, 10, 15, 20
     ytd: 1*

    freqType - The type of frequency with which a new candle is formed.

       Valid frequencyTypes by periodType (defaults marked with an asterisk):

       day: minute*
       month: daily, weekly*
       year: daily, weekly, monthly*
       ytd: daily, weekly*

    freq - The number of the frequencyType to be included in each candle.

       Valid frequencies by frequencyType (defaults marked with an asterisk):

       minute: 1*, 5, 10, 15, 30
       daily: 1*
       weekly: 1*
       monthly: 1*

    endDate - End date as milliseconds since epoch. If startDate and endDate are provided, period should not be provided.
      Default is previous trading day.

    startDate - Start date as milliseconds since epoch. If startDate and endDate are provided, period should not be provided.

    extHourData - true to return extended hours data, false for regular market hours only. Default is true */

	std::string getHistPrice(std::string symbol, std::string periodType = "day", std::string period = "", std::string freqType = "", std::string freq = "", unsigned int endDate = 0, unsigned int startDate = 0, bool extHourData = true);

    std::string placeOTTOrder(std::string symbol, double price, double profit = 0.05, int quanty = 1, std::string cancelTime = "2022-12-12", double stopPriceOffset = 0.02);
    std::string replaceOTOOrder(std::string orderID, std::string symbol, double price, double profit = 0.05, int quanty = 1, std::string cancelTime = "");

    std::string placeOTOTAOrder(std::string symbol, double price, double profit = 0.05, int quanty = 1, double stopPriceOffset = 0.02);

    std::string getOdersByPath(int maxResults = 1000, std::string fromEnteredTime = "2022-08-21", std::string toEnteredTime = "", std::string status = "");
    std::string cancelOrder(std::string orderID);

private:
    Account() = delete;

    std::shared_ptr<Rest> rest_interface;
    std::shared_ptr<OAuth> authorization;

    std::string account_number;
    std::string account_resource_url = "https://api.tdameritrade.com/v1/accounts/";
    std::string market_get_resource_url = "https://api.tdameritrade.com/v1/marketdata/";
    std::string order_resource_url = "https://api.tdameritrade.com/v1/accounts/";

    rapidjson::Document account_details;


    rapidjson::Value& parse_json_field(std::string name, rapidjson::Document& to_parse);
    rapidjson::Value& parse_json_field(std::string name, rapidjson::Value& to_parse);
};
#endif // ACCOUNT_H


