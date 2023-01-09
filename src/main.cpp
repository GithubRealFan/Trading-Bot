#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <windows.h>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include "TDA.h"
#include "account.h"


//#define THREAD_CYCLING_TIME_ACCOUNT 60
#define THREAD_CYCLING_TIME_TRADING 15

int64_t tickCount;

void handle_failed_authentication(Account& account, std::string oauth_uid);
void getUser();

void accountThread(TDA *tda) {

}

void tradingThread(TDA *tda) {
    while(true) {
        tda->disp_Trading();
        sleep(THREAD_CYCLING_TIME_TRADING);
    }
}

int main() {


    /*std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    std::cout << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<  now->tm_mday << std::endl;
    */
    //get user
    FILE* fp = fopen("user.json", "rb"); // non-Windows use "r"

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document user;
    user.ParseStream(is);
    fclose(fp);


    Account account(user["account_number"].GetString(), user["oauth_uid"].GetString());

    if (account.get_authorization_status() != OAuthStatus::Valid) {
        handle_failed_authentication(account, user["oauth_uid"].GetString());
    }

    TDA testTda(account);
    //testTda.run_loop();
    //std::thread thread_Of_Account(accountThread, &testTda);
    std::thread thread_Of_Trading(tradingThread, &testTda);

    //thread_Of_Account.join();
    thread_Of_Trading.join();

	return 0;
}

void handle_failed_authentication(Account& account, std::string oauth_uid) {
    // Loading refresh from disk failed, we need to manually get a new token.
    OAuth oauth(oauth_uid, "https://127.0.0.1:8080", account.get_rest_interface());
    std::string code = "";

    std::cout << "Failed to load valid refresh token from disk. Please manually reauthenticate." << std::endl;
    std::cout << "Steps:" << std::endl;
    std::cout << "1. Go to this url: " << oauth.generate_authentication_url() << std::endl;
    std::cout << "2. Authenticate with your TD Ameritrade account to approve the app." << std::endl;
    std::cout << "3. The result will be a code in your address bar and a failed page load @ https://127.0.0.1:8080. Paste entire address bar contents below: " << std::endl;
    std::cout << ": ";

    std::cin >> code;

    bool code_accepted = oauth.accept_authentication_code(code);
    OAuthStatus token_generation_status = oauth.generate_tokens();

    if (code_accepted && token_generation_status == OAuthStatus::Valid) {
        std::cout << std::endl << "Successfully validated account! Please restart application." << std::endl;
    }
    else {
        std::cout << std::endl << "Error loading account. Please check credentials and developer application authentication and try again." << std::endl;
    }

    std::cout << " Press enter to quit.";
    std::cin.ignore();
}
