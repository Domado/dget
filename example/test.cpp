#include "test.h" 
#include <iostream>
#include <string>
#include <cstring> 

extern "C" {
    #include "daiso_http.h"
}

bool run_http_test(const std::string& url) {
    std::cout << "--- Dget Test: TESTING URL: " << url << " ---" << std::endl;

    DaisoHttpHandle *http = nullptr;
    DaisoHttpResponse *resp = nullptr;
    bool success = false;

    http = daiso_http_init();
    if (!http) {
        std::cerr << "!!! TEST FAILED: daiso_http_init() RETURN NULL" << std::endl;
        return false;
    }
    std::cout << "  [PASS] daiso_http_init()" << std::endl;

    if (daiso_http_set_url(http, url.c_str()) != 0) {
        std::cerr << "!!! TEST FAILED: daiso_http_set_url() FAILED" << std::endl;
        goto cleanup;
    }
    std::cout << "  [PASS] daiso_http_set_url()" << std::endl;

    resp = daiso_http_perform(http);
    if (!resp) {
        std::cerr << "!!! TEST FAILED: daiso_http_perform() RETURN NULL" << std::endl;
        goto cleanup;
    }
    std::cout << "  [PASS] daiso_http_perform()" << std::endl;

    std::cout << "  [INFO] VERTIFYING..." << std::endl;
    if (resp->status_code == 200) {
        std::cout << "    [PASS] 状态码: " << resp->status_code 
                  << " " << (resp->status_text ? resp->status_text : "") << std::endl;
    } else {
        std::cerr << "!!! TEST FAILED: CODE NOT 200, RATHER THAN " << resp->status_code << std::endl;
        goto cleanup;
    }

    if (resp->body_len > 0 && resp->body) {
        std::cout << "    [PASS] RESPONSE BODY: " << resp->body_len << " BYTES" << std::endl;
    } else {
        std::cerr << "!!! TEST FAILED: RESPONSE BODY NULL" << std::endl;
        goto cleanup;
    }

    if (strncmp(resp->body, "<!doctype html>", 15) == 0 || 
        strncmp(resp->body, "<html>", 6) == 0) {
        std::cout << "    [PASS] RESONSE BODY IS HTML" << std::endl;
    } else {
        std::cout << "    [WARN] RESPONSE BODY IS NOT LIKE A HTML WEBPAGE(?)" << std::endl;
    }

    success = true;

cleanup:
    std::cout << "  [INFO] CLEANING MEMROY..." << std::endl;
    daiso_http_response_free(resp);
    daiso_http_cleanup(http);

    return success;
}

int main(int argc, char *argv[]) {
    std::string url_to_test = "http://www.daiso666.com/";

    if (argc > 1) {
        url_to_test = argv[1];
    }

    if (run_http_test(url_to_test)) {
        std::cout << "\n--- TEST SUITE: PASSED ---" << std::endl;
        return 0; 
    } else {
        std::cout << "\n--- TEST SUITE: FAILED ---" << std::endl;
        return 1; 
    }
}
