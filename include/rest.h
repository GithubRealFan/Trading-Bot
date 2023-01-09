#ifndef REST_H
#define REST_H

#include <mutex>
#include <string>

class Rest
{
public:
    Rest();
    ~Rest();

    std::string post(std::string url, std::string header, std::string post_data, bool& valid_data_out, bool verbose = false);
    std::string get(std::string url, std::string header, bool& valid_data_out, bool verbose = false);
    std::string del(std::string url, std::string header, bool& valid_data_out, bool verbose = false);

    std::string url_encoding(std::string input);

private:
    std::mutex curl_lock;

    static size_t curl_callback(void* contents, size_t size, size_t nmemb, void* return_data);
};

#endif // REST_H
