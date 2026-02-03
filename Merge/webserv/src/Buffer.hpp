/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Buffer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:33:50 by yroard            #+#    #+#             */
/*   Updated: 2026/01/27 11:03:07 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_HPP
# define BUFFER_HPP

#include <vector>

namespace webserv {
// Inherit from vector to get all vector methods ([], size, begin, end) 
//for free
    class Buffer : public std::vector<char> {
    public:
        // Default constructor
        Buffer() : std::vector<char>() {}
        // Constructor from string
        explicit Buffer(const std::string& str) {
            // Use the base class's method to fill data
            this->reserve(str.size());
            this->assign(str.begin(), str.end());
        }
        // Iterator Constructor (Crucial for Client.hpp)
        // Allows: Buffer buf(str.begin(), str.end());
        template <typename InputIterator>
        Buffer(InputIterator first, InputIterator last) 
            : std::vector<char>(first, last) {}
        ~Buffer() {}
        // You don't need getBuffer(), 'this' IS the buffer.
        std::string getBufferStr() const {
            return std::string(this->begin(), this->end());
        }
    };
}

#endif
