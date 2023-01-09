#include "account.h"

#include <iomanip>
#include <sstream>
#include <iostream>

Account::Account(std::string account_number_in, std::string oauth_uid_in)
    : account_number(account_number_in)
{
    this->rest_interface = std::shared_ptr<Rest>(new Rest());
    this->authorization = std::shared_ptr<OAuth>(new OAuth(oauth_uid_in, "https://127.0.0.1:8080", this->rest_interface));

    this->account_resource_url.append(account_number);
    this->account_resource_url.append("?fields=positions,orders");
}

bool Account::refresh_account() {
    bool success = false;

    if (this->get_authorization_status() == OAuthStatus::Valid) {
        std::string header = std::string("Authorization: Bearer ") + authorization->get_access_token();
        std::string response = this->rest_interface->get(this->account_resource_url, header, success);
        this->account_details.Parse(response.c_str(), response.length());
    } else {
        if(this->authorization->generate_tokens() == OAuthStatus::Valid) success = true;
    }

    return success;
}

std::string Account::account_type() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("type", account);

        return data.GetString();
    }
    catch (...) {
        return "";
    }
}

std::string Account::account_id() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("accountId", account);

        return data.GetString();
    }
    catch (...) {
        return "";
    }
}

unsigned int Account::round_trips() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("roundTrips", account);

        return data.GetUint();
    }
    catch (...) {
        return 0;
    }
}

bool Account::is_day_trader() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("isDayTrader", account);

        return data.GetBool();
    }
    catch (...) {
        return false;
    }
}

bool Account::is_closing_only_restricted() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("isDayTrader", account);

        return data.GetBool();
    }
    catch (...) {
        return false;
    }
}

double Account::current_accrued_interest() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("accruedInterest", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_available_funds_non_marginable_trade() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("availableFundsNonMarginableTrade", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_bond_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("bondValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_buying_power() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("buyingPower", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_cash_balance() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("cashBalance", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_cash_available_for_trading() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("cashAvailableForTrading", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_cash_reciepts() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("cashReceipts", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_day_trading_buying_power() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("dayTradingBuyingPower", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_day_trading_buying_power_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("dayTradingBuyingPowerCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_day_trading_equity_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("dayTradingEquityCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_equity() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("equity", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_equity_percentage() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("equityPercentage", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_liquidation_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("liquidationValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_long_margin_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("longMarginValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_long_option_market_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("longOptionMarketValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_long_stock_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("longStockValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_maintenance_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("maintenanceCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_maintenance_requirement() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("maintenanceRequirement", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_margin() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("margin", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_margin_equity() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("marginEquity", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_money_market_fund() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("moneyMarketFund", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_mutual_fund_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("mutualFundValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_reg_t_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("regTCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_short_margin_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("shortMarginValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_short_option_market_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("shortOptionMarketValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_short_stock_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("shortStockValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_total_cash() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("totalCash", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_is_in_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("isInCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_unsettled_cash() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("unsettledCash", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_pending_deposits() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("pendingDeposits", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_margin_balance() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("marginBalance", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_short_balance() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("shortBalance", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::current_account_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("currentBalances", account);
        rapidjson::Value& balance = this->parse_json_field("accountValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}



/**
 * Initial Balance section.
 */

double Account::initial_accrued_interest() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("accruedInterest", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_available_funds_non_marginable_trade() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("availableFundsNonMarginableTrade", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_bond_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("bondValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_buying_power() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("buyingPower", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_cash_balance() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("cashBalance", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_cash_available_for_trading() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("cashAvailableForTrading", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_cash_reciepts() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("cashReceipts", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_day_trading_buying_power() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("dayTradingBuyingPower", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_day_trading_buying_power_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("dayTradingBuyingPowerCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_day_trading_equity_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("dayTradingEquityCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_equity() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("equity", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_equity_percentage() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("equityPercentage", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_liquidation_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("liquidationValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_long_margin_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("longMarginValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_long_option_market_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("longOptionMarketValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_long_stock_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("longStockValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_maintenance_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("maintenanceCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_maintenance_requirement() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("maintenanceRequirement", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_margin() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("margin", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_margin_equity() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("marginEquity", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_money_market_fund() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("moneyMarketFund", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_mutual_fund_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("mutualFundValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_reg_t_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("regTCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_short_margin_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("shortMarginValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_short_option_market_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("shortOptionMarketValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_short_stock_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("shortStockValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_total_cash() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("totalCash", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_is_in_call() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("isInCall", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_unsettled_cash() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("unsettledCash", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_pending_deposits() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("pendingDeposits", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_margin_balance() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("marginBalance", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_short_balance() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("shortBalance", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::initial_account_value() {
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("initialBalances", account);
        rapidjson::Value& balance = this->parse_json_field("accountValue", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

//Projected Balances

double Account::projected_cash_available_for_trading(){
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("projectedBalances", account);
        rapidjson::Value& balance = this->parse_json_field("cashAvailableForTrading", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

double Account::projected_cash_available_for_withdrawl(){
    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("projectedBalances", account);
        rapidjson::Value& balance = this->parse_json_field("cashAvailableForWithdrawal", data);

        return balance.GetDouble();
    }
    catch (...) {
        return 0.0;
    }
}

OAuthStatus Account::get_authorization_status() {
    return this->authorization->get_status();
}

std::shared_ptr<Rest> Account::get_rest_interface() {
    return this->rest_interface;
}

std::vector<std::string> Account::positions() {
    std::vector<std::string> display_data;

    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("positions", account);

        for (auto& position : data.GetArray()) {
            rapidjson::Value& instrument = this->parse_json_field("instrument", position);
            std::string header;

            std::stringstream longqty;
            std::stringstream shortqty;
            std::stringstream day_pl_percent;
            std::stringstream day_pl;
            std::stringstream market_value;
            std::stringstream purchase_price;

            bool is_option = instrument["assetType"].GetString() == std::string("OPTION");
            bool is_equity = instrument["assetType"].GetString() == std::string("EQUITY");

            longqty << std::fixed << std::setprecision(2) << position["longQuantity"].GetDouble();
            shortqty << std::fixed << std::setprecision(2) << position["shortQuantity"].GetDouble();
            day_pl_percent << std::fixed << std::setprecision(2) << position["currentDayProfitLossPercentage"].GetDouble();
            day_pl << std::fixed << std::setprecision(2) << position["currentDayProfitLoss"].GetDouble();
            market_value << std::fixed << std::setprecision(2) << position["marketValue"].GetDouble();
            purchase_price << std::fixed << std::setprecision(4) << position["averagePrice"].GetDouble();

            if (is_equity) {
                // We have a stock position.
                header = instrument["symbol"].GetString() + std::string(" [Equity]");
            }
            else if (is_option) {
                // We have an option position
                header = instrument["underlyingSymbol"].GetString() + std::string(" [Option]");
            }

            // Line 1 - Symbol name, quantity short/long, market value.
            display_data.push_back(
                    header +
                    std::string(" [+") +
                    longqty.str() +
                    std::string(" -") +
                    shortqty.str() +
                    std::string("] [$") +
                    market_value.str() +
                    std::string("]")
            );

            // Line 2 (Options only) - Call/Put Description
            if (is_option) {
                display_data.push_back(
                            std::string("\t") +
                            instrument["description"].GetString()
                        );
            }

            // Line 3, balance and P/L
            display_data.push_back(
                    std::string("\tP/L: ") +
                    day_pl_percent.str() +
                    std::string("% ($") +
                    day_pl.str() +
                    std::string(")  Purchased @ $") +
                    purchase_price.str()
            );

            display_data.push_back("");
        }

        return display_data;
    }
    catch (...) {
        return std::vector<std::string>();
    }
}

std::vector<std::string> Account::orders() {
    std::vector<std::string> display_data;

    try {
        rapidjson::Value& account = this->parse_json_field("securitiesAccount", this->account_details);
        rapidjson::Value& data = this->parse_json_field("orderStrategies", account);

        for (auto& order : data.GetArray()) {
            // Enforce only single limit orders for now.
            std::string d1 = order["orderStrategyType"].GetString();
            std::string d2 = order["orderType"].GetString();
            if (order["orderStrategyType"].GetString() != std::string("TRIGGER")) {
                continue;
            }

            std::stringstream placed_qty;
            std::stringstream filled_qty;
            std::stringstream price;
            std::stringstream order_id;

            // Line 1 - Order ID, price, status.
            placed_qty << std::fixed << std::setprecision(1) << order["quantity"].GetDouble();
            filled_qty << std::fixed << std::setprecision(1) << order["filledQuantity"].GetDouble();
            price << std::fixed << std::setprecision(2) << order["price"].GetDouble();
            order_id << order["orderId"].GetInt64();


            display_data.push_back(
                        order_id.str() +
                        std::string("\n\t[$") +
                        price.str() +
                        std::string("] [") +
                        order["status"].GetString() +
                        std::string("]")
                        );

            // Line 2, leg info
            for (auto& leg : order["orderLegCollection"].GetArray()) {
                rapidjson::Value& instrument = leg["instrument"];
                display_data.push_back(std::string("\t") +
                            leg["instruction"].GetString() +
                            std::string(" ") +
                            instrument["symbol"].GetString() +
                            std::string(" [") +
                            instrument["assetType"].GetString() +
                            std::string("] ")
                            );
            }
            // Line 3, Quantity info
            display_data.push_back(std::string("\t") +
                        std::string("Qty Placed: ") +
                        placed_qty.str() +
                        std::string("  Qty Filled: ") +
                        filled_qty.str()
                        );
            // Line 4, Child Order Strategies info
            rapidjson::Value& childData = this->parse_json_field("childOrderStrategies", order);
            for (auto& childOrder : childData.GetArray()) {
                std::stringstream child_placed_qty;
                std::stringstream child_filled_qty;
                std::stringstream child_price;
                std::stringstream child_order_id;

                // Line 5 - Child Order ID, price, status.
                child_placed_qty << std::fixed << std::setprecision(1) << childOrder["quantity"].GetDouble();
                child_filled_qty << std::fixed << std::setprecision(1) << childOrder["filledQuantity"].GetDouble();
                child_price << std::fixed << std::setprecision(2) << childOrder["price"].GetDouble();
                child_order_id << childOrder["orderId"].GetInt64();
                display_data.push_back(
                        child_order_id.str() +
                        std::string("\n\t[$") +
                        child_price.str() +
                        std::string("] [") +
                        childOrder["status"].GetString() +
                        std::string("]")
                        );
                // Line 6, leg info
                for (auto& leg : childOrder["orderLegCollection"].GetArray()) {
                    rapidjson::Value& instrument = leg["instrument"];
                    display_data.push_back(std::string("\t") +
                                leg["instruction"].GetString() +
                                std::string(" ") +
                                instrument["symbol"].GetString() +
                                std::string(" [") +
                                instrument["assetType"].GetString() +
                                std::string("] ")
                                );
                }
                // Line 7, Quantity info
                display_data.push_back(std::string("\t") +
                            std::string("Qty Placed: ") +
                            child_placed_qty.str() +
                            std::string("  Qty Filled: ") +
                            child_filled_qty.str()
                            );


            }
            display_data.push_back("");
        }

        return display_data;
    }
    catch (...) {
        return std::vector<std::string>();
    }
}

std::string Account::getQuote(std::string symbol) {
    std::string rlt="";
    if (this->get_authorization_status() == OAuthStatus::Valid) {
        std::string header = std::string("Authorization: Bearer ") + this->authorization->get_access_token();
        // Perform the REST query.
        bool valid_data_from_post;
        std::string quote_url = this->market_get_resource_url + symbol + "/quotes?";// + this->authorization->get_OAuth_UID();
        std::string get_return_data = this->rest_interface->get(quote_url, header, valid_data_from_post);
        //std::cout << get_return_data << std::endl;
        // Parse out and handle tokens if post was successfull.
        if(valid_data_from_post) {
            rlt = get_return_data;
        }
        else { // POST came back incorrect.
            rlt = "Request Error!";
        }
    } else {
        if(this->refresh_account()) {
            rlt = "Refresh Token!";
        }
    }
    return rlt;
}

std::string Account::getHistPrice(std::string symbol, std::string periodType, std::string period, std::string freqType, std::string freq, unsigned int endDate, unsigned int startDate, bool extHourData) {
    std::string rlt="";
    if (this->get_authorization_status() == OAuthStatus::Valid) {
        std::string header = std::string("Authorization: Bearer ") + this->authorization->get_access_token();

        // Perform the REST query.
        bool valid_data_from_post;
        std::string history_url = this->market_get_resource_url + symbol + "/pricehistory?";// + this->authorization->get_OAuth_UID();
        // append the values for each parameter if they are there
		history_url.append("periodType=" + periodType);

		if(period != "") {
			//std::cout << "Period Present" << std::endl;
			history_url.append("&period=" + period);
		}

		if(freqType != "") {
			//std::cout << "Frequence Type Present" << std::endl;
			history_url.append("&frequencyType=" + freqType);
		}

		if(freq != "") {
			//std::cout << "Frequence Present" << std::endl;
			history_url.append("&frequency=" + freq);
		}

		if(endDate != 0) {
			//std::cout << "End Date Present" << std::endl;
			history_url.append("&endDate=" + std::to_string(endDate));
		}

		if(startDate != 0) {
			//std::cout << "Start date present" << std::endl;
			history_url.append("&startDate=" + std::to_string(startDate));
		}

		if(extHourData) {
			history_url.append("&needExtendedHoursData=true");
		}else {
			history_url.append("&needExtendedHoursData=false");
		}
        std::string get_return_data = this->rest_interface->get(history_url, header, valid_data_from_post);

        if(valid_data_from_post) {
            rlt = get_return_data;
        }
        else { // POST came back incorrect.
            rlt = "Request Error!";
        }
    } else {
        if(this->refresh_account()) {
            rlt = "Refresh Token!";
        }
    }
    return rlt;
}

std::string Account::placeOTTOrder(std::string symbol, double price, double profit, int quanty, std::string cancelTime, double stopPriceOffset) {
    std::string rlt="";
    if (this->get_authorization_status() == OAuthStatus::Valid) {
        std::string header = "Content-Type: application/json,Authorization: Bearer " + this->authorization->get_access_token();
        std::string postData =
            R"({
              "orderType": "LIMIT",
              "session": "SEAMLESS",
              "price": )" + std::to_string(price) + R"(,
              "duration": "DAY",
              "orderStrategyType": "TRIGGER",
              "orderLegCollection": [
                {
                  "instruction": "BUY",
                  "quantity": )" + std::to_string(quanty) + R"(,
                  "instrument": {
                    "symbol": ")" + symbol +R"(",
                    "assetType": "EQUITY"
                  }
                }
              ],
              "childOrderStrategies": [
                {
                  "orderType": "LIMIT",
                  "session": "SEAMLESS",
                  "price": )" + std::to_string(price + profit) + R"(,
                  "duration": "GOOD_TILL_CANCEL",
                  "cancelTime" : ")" + cancelTime + R"(",
                  "orderStrategyType": "SINGLE",
                  "orderLegCollection": [
                    {
                      "instruction": "SELL",
                      "quantity": 1,
                      "instrument": {
                        "symbol": ")" + symbol + R"(",
                        "assetType": "EQUITY"
                      }
                    }
                  ]
                }, {
                  "orderType": "TRAILING_STOP",
                  "session": "NORMAL",
                  "stopPriceLinkBasis": "BID",
                  "stopPriceLinkType": "VALUE",
                  "stopPriceOffset": )" + std::to_string(stopPriceOffset) + R"(,
                  "duration": "GOOD_TILL_CANCEL",
                  "orderStrategyType": "SINGLE",
                  "orderLegCollection": [
                    {
                      "instruction": "SELL",
                      "quantity": )" + std::to_string(quanty - 1) + R"(,
                      "instrument": {
                        "symbol": ")" + symbol + R"(",
                        "assetType": "EQUITY"
                      }
                    }
                  ]
                }
              ]
            })";
        // Perform the REST query.
        bool valid_data_from_post;
        std::string order_url = this->order_resource_url + this->account_id() + "/orders";
        std::string return_data = this->rest_interface->post(order_url, header, postData, valid_data_from_post);

        // Parse out and handle tokens if post was successfull.
        if(valid_data_from_post) {
            rlt = return_data;
        }
        else { // POST came back incorrect.
            rlt = "Request Error!";
        }
    } else {
        if(this->refresh_account()) {
            rlt = "Refresh Token!";
        }
    }
    return rlt;
}

std::string Account::replaceOTOOrder(std::string orderID, std::string symbol, double price, double profit, int quanty, std::string cancelTime) {
    std::string rlt="";
    if (this->get_authorization_status() == OAuthStatus::Valid) {
        std::string header = "Content-Type: application/json,Authorization: Bearer " + this->authorization->get_access_token();
        std::string postData =
            R"({
              "orderType": "LIMIT",
              "session": "SEAMLESS",
              "price": )" + std::to_string(price) + R"(,
              "duration": "DAY",
              "orderStrategyType": "TRIGGER",
              "orderLegCollection": [
                {
                  "instruction": "BUY",
                  "quantity": )" + std::to_string(quanty) + R"(,
                  "instrument": {
                    "symbol": ")" + symbol +R"(",
                    "assetType": "EQUITY"
                  }
                }
              ],
              "childOrderStrategies": [
                {
                  "orderType": "LIMIT",
                  "session": "SEAMLESS",
                  "price": )" + std::to_string(price + profit) + R"(,
                  "duration": "GOOD_TILL_CANCEL",
                  "cancelTime" : ")" + cancelTime + R"(",
                  "orderStrategyType": "SINGLE",
                  "orderLegCollection": [
                    {
                      "instruction": "SELL",
                      "quantity": )" + std::to_string(quanty) + R"(,
                      "instrument": {
                        "symbol": ")" + symbol + R"(",
                        "assetType": "EQUITY"
                      }
                    }
                  ]
                }
              ]
            })";
        // Perform the REST query.
        bool valid_data_from_post;
        std::string order_url = this->order_resource_url + this->account_id() + "/orders/" + orderID;
        std::string return_data = this->rest_interface->post(order_url, header, postData, valid_data_from_post);

        // Parse out and handle tokens if post was successfull.
        if(valid_data_from_post) {
            rlt = return_data;
        }
        else { // POST came back incorrect.
            rlt = "Request Error!";
        }
    } else {
        if(this->refresh_account()) {
            rlt = "Refresh Token!";
        }
    }
    return rlt;
}

std::string Account::placeOTOTAOrder(std::string symbol, double price, double profit, int quanty, double stopPriceOffset) {
    std::string rlt="";
    if (this->get_authorization_status() == OAuthStatus::Valid) {
        std::string header = "Content-Type: application/json,Authorization: Bearer " + this->authorization->get_access_token();
        std::string postData =
            R"({
              "orderType": "LIMIT",
              "session": "NORMAL",
              "price": )" + std::to_string(price) + R"(,
              "duration": "DAY",
              "orderStrategyType": "TRIGGER",
              "orderLegCollection": [
                {
                  "instruction": "BUY",
                  "quantity": )" + std::to_string(quanty) + R"(,
                  "instrument": {
                    "symbol": ")" + symbol +R"(",
                    "assetType": "EQUITY"
                  }
                }
              ],
              "childOrderStrategies": [
                {
                  "orderType": "LIMIT",
                  "session": "NORMAL",
                  "price": )" + std::to_string(price + profit) + R"(,
                  "duration": "GOOD_TILL_CANCEL",
                  "orderStrategyType": "TRIGGER",
                  "orderLegCollection": [
                    {
                      "instruction": "SELL",
                      "quantity": 1,
                      "instrument": {
                        "symbol": ")" + symbol + R"(",
                        "assetType": "EQUITY"
                      }
                    }
                  ],
                  "childOrderStrategies": [
                    {
                      "orderType": "TRAILING_STOP",
                      "session": "NORMAL",
                      "stopPriceLinkBasis": "BID",
                      "stopPriceLinkType": "VALUE",
                      "stopPriceOffset": )" + std::to_string(stopPriceOffset) + R"(,
                      "duration": "GOOD_TILL_CANCEL",
                      "orderStrategyType": "SINGLE",
                      "orderLegCollection": [
                        {
                          "instruction": "SELL",
                          "quantity": )" + std::to_string(quanty - 1) + R"(,
                          "instrument": {
                            "symbol": ")" + symbol + R"(",
                            "assetType": "EQUITY"
                          }
                        }
                      ]
                    }
                  ]
                }
              ]
            })";
        // Perform the REST query.
        bool valid_data_from_post;
        std::string order_url = this->order_resource_url + this->account_id() + "/orders";
        std::string return_data = this->rest_interface->post(order_url, header, postData, valid_data_from_post);

        // Parse out and handle tokens if post was successfull.
        if(valid_data_from_post) {
            rlt = return_data;
        }
        else { // POST came back incorrect.
            rlt = "Request Error!";
        }
    } else {
        if(this->refresh_account()) {
            rlt = "Refresh Token!";
        }
    }
    return rlt;
}

std::string Account::getOdersByPath(int maxResults, std::string fromEnteredTime, std::string toEnteredTime, std::string status) {
    std::string rlt="";
    if (this->get_authorization_status() == OAuthStatus::Valid) {
        std::string header = std::string("Authorization: Bearer ") + this->authorization->get_access_token();

        // Perform the REST query.
        bool valid_data_from_post;
        std::string order_url = this->order_resource_url + this->account_id() + "/orders?";

        order_url.append("maxResults=" + std::to_string(maxResults));

		if(fromEnteredTime != "") {
			order_url.append("&fromEnteredTime=" + fromEnteredTime);
		}

        if(toEnteredTime != "") {
			//std::cout << "Period Present" << std::endl;
			order_url.append("&toEnteredTime=" + toEnteredTime);
		}

		if(status != "") {
			//std::cout << "Period Present" << std::endl;
			order_url.append("&status=" + status);
		}

        //std::cout << order_url << std::endl;
        std::string return_data = this->rest_interface->get(order_url, header, valid_data_from_post);

        // Parse out and handle tokens if post was successfull.
        if(valid_data_from_post) {
            rlt = return_data;
        }
        else { // POST came back incorrect.
            rlt = "Request Error!";
        }
    } else {
        if(this->refresh_account()) {
            rlt = "Refresh Token!";
        }
    }
    return rlt;
}

std::string Account::cancelOrder(std::string orderID) {
    std::string rlt="";
    if (this->get_authorization_status() == OAuthStatus::Valid) {
        std::string header = std::string("Authorization: Bearer ") + this->authorization->get_access_token();
        // Perform the REST query.
        bool valid_data_from_post;
        std::string order_url = this->order_resource_url + this->account_id() + "/orders/" + orderID;

        std::string return_data = this->rest_interface->del(order_url, header, valid_data_from_post, true);

        // Parse out and handle tokens if post was successfull.
        if(valid_data_from_post) {
            rlt = return_data;
        }
        else { // POST came back incorrect.
            rlt = "Request Error!";
        }
    } else {
        if(this->refresh_account()) {
            rlt = "Refresh Token!";
        }
    }
    return rlt;
}

rapidjson::Value& Account::parse_json_field(std::string name, rapidjson::Document& to_parse) {
    if (to_parse.HasMember(name.c_str())) {
        return to_parse[name.c_str()];
    }
    else {
        throw "Field does not exist!";
    }
}

rapidjson::Value& Account::parse_json_field(std::string name, rapidjson::Value& to_parse) {
    if (to_parse.HasMember(name.c_str())) {
        return to_parse[name.c_str()];
    }
    else {
        throw "Field does not exist!";
    }
}













