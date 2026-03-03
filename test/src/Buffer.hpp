/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Buffer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:33:50 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 10:59:50 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_HPP
# define BUFFER_HPP

#include <vector>

namespace webserv {
	class Buffer {
	private:
		std::vector<char> m_data;
		
	public:
		Buffer(){}     
		explicit Buffer(const std::string& str) {
			m_data.assign(str.begin(), str.end());
		}
		template <typename InputIterator>
		Buffer(InputIterator first, InputIterator last) 
			: m_data(first, last){}
		
		Buffer& operator=(const Buffer& other){
			if (this != &other){
				m_data = other.m_data;
			}
			return *this;	
		}
		~Buffer() {}
		char& operator[](const size_t pos) {
			return m_data[pos]; 
		}
		
		const char& operator[](const size_t pos) const {
			return m_data[pos]; 
		}

		size_t size() const { 
			return m_data.size(); 
		}
		
		bool empty() const { 
			return m_data.empty(); 
		}
		
		void clear() { 
			m_data.clear(); 
		}
		
		void push_back(char c) { 
			m_data.push_back(c); 
		}
		
		void reserve(size_t n) { 
			m_data.reserve(n); 
		}

		typedef std::vector<char>::iterator iterator;
		typedef std::vector<char>::const_iterator const_iterator;
		
		iterator begin() { 
			return m_data.begin(); 
		}
		
		iterator end() { 
			return m_data.end(); 
		}
		
		const_iterator begin() const { 
			return m_data.begin();
		}
		
		const_iterator end() const { 
			return m_data.end();
		}
		
		std::string getBufferStr() const {
			return std::string(m_data.begin(), m_data.end());
		}
		
		void append(const std::string& str) {
			m_data.insert(m_data.end(), str.begin(), str.end());
		}

		void insert(std::string& buffer, size_t start, size_t end) {
			m_data.insert(m_data.end(), buffer.begin() + start,
				buffer.begin() + end);
		}
	};
}

#endif
