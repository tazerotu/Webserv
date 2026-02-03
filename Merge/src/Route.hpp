/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:34:44 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:34:47 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class IRoute {
    virtual void handler(http::Request &req, http::Response &res) = 0;
};

//class Route: où est la gestion du path? de la méthode?
class RouteHelloWorld : public IRoute {
public:
    void handler(http::Request &req, http::Response &res) {
        res.status(200);
        res.headers().set("Content-type", "text/plain"); //
        res.body() = "Hello World 2!";
        res.send();
    }
};

// créer une classe route pour chaque type d'erreur? Géré par Thimothy?
// créer une classe route par défaut?


// class RouteHelloWorld : public IRoute {
// public:
//     void handler(http::Request &req, http::Response &res) {
//         res.status(200);
//         res.headers().set("Content-type", "text/plain");
//         res.body() = "Hello World 2!";
//         res.send();
//     }
// };
//
// class RouteHelloWorld : public IRoute {
// public:
//     void handler(http::Request &req, http::Response &res) {
//         res.status(200);
//         res.headers().set("Content-type", "text/plain");
//         res.body() = "Hello World 2!";
//         res.send();
//     }
// };
