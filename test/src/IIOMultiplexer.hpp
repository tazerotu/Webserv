/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IIOMultiplexer.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 12:30:35 by yroard            #+#    #+#             */
/*   Updated: 2026/02/19 17:07:59 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_IIOMULTIPLEXER_HPP
#define WEBSERV_IIOMULTIPLEXER_HPP

namespace webserv {
    class IIOMultiplexer {
    public:
        virtual ~IIOMultiplexer() {}
        virtual void addFd(int fd) = 0;
        virtual void removeFd(int fd) = 0;
        virtual void listenWriting(int fd) = 0;
        virtual void stopListeningWriting(int fd) = 0;
        virtual int wait(int max_fd) = 0;
        virtual std::vector<int> getReadyReadFds() const = 0;
        virtual std::vector<int> getReadyWriteFds() const = 0;
        // virtual bool isSendFailedFatally(int fd, Client* client) = 0;
    };
}

#endif //WEBSERV_IIOMULTIPLEXER_HPP