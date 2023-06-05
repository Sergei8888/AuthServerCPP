//
// Created by Sergei Kuzmenkov on 04.06.2023.
//

#ifndef AUTHSERVER_TEST_CONTROLLER_H
#define AUTHSERVER_TEST_CONTROLLER_H

#include <httplib.h>
#include <boost/multiprecision/cpp_int.hpp>
using big_int = boost::multiprecision::cpp_int;

class FibonacciController {
public:
    FibonacciController() = default;

    ~FibonacciController() = default;

    void fibonacci(const httplib::Request &req, httplib::Response &res) {
        big_int n = std::stoi(req.matches[1]);

        if (n < 0) {
            res.status = 400;
            res.set_content("Bad request", "text/plain");
            return;
        }

        big_int a = 0;
        big_int b = 1;
        big_int c = 0;

        for (int i = 0; i < n; i++) {
            c = a + b;
            a = b;
            b = c;
        }

        res.set_content(to_string(c), "text/plain");
    }
};

#endif //AUTHSERVER_TEST_CONTROLLER_H
