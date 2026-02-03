/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainServer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:20:50 by yroard            #+#    #+#             */
/*   Updated: 2025/11/12 16:32:55 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//5/core05-webserv/webserv$ ./webserv 8080
//Server listening on port 8080
//
//Navigate to http://localhost:8080/ (replace 8080 with your chosen port).
//You should see "Hello World!" in your browser.
//In your server's terminal, you'll see the incoming request and the
//"Accepted connection from..." message:
/*
Accepted connection from localhost:52326
Received request:
GET / HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:144.0) Gecko/20100101 Firefox/144.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*'/'*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br, zstd
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Priority: u=0, i


Accepted connection from localhost:52338
Received request:
GET /favicon.ico HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:144.0) Gecko/20100101 Firefox/144.0
Accept: image/avif,image/webp,image/png,image/svg+xml,image'/'*;q=0.8,*'/'*;q=0.5
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br, zstd
Connection: keep-alive
Referer: http://localhost:8080/
Sec-Fetch-Dest: image
Sec-Fetch-Mode: no-cors
Sec-Fetch-Site: same-origin
Priority: u=6
*/

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // For close()
#include <arpa/inet.h> // For inet_ntop

#define BUF_SIZE 2048 // HTTP requests can be larger
#define PORT "8080"   // Standard HTTP port, or another high port for testing

// Function to handle a single client connection
void handle_client(int client_sfd) {
	char req_buf[BUF_SIZE]; 
	ssize_t bytes_read;  // Read client's request 
	bytes_read = recv(client_sfd, req_buf, BUF_SIZE - 1, 0); 
	if (bytes_read == -1) { 
		perror("recv error"); 
		close(client_sfd); 
		return; 
	} 
	req_buf[bytes_read] = '\0'; // Null-terminate the received data  
	printf("Received request:\n%s\n", req_buf);  // --- Very basic HTTP request parsing --- 
	// Look for GET / HTTP/1.1 
	if (strncmp(req_buf, "GET / HTTP", 10) == 0) {
		const char *http_response_ok = "HTTP/1.1 200 OK\r\n" 
				"Content-Type: text/plain\r\n"
				"Content-Length: 12\r\n" 
				"\r\n" // Important: End of headers with an empty line 
				"Hello World!";  
		if (send(client_sfd, http_response_ok, strlen(http_response_ok), 0) == -1) {
			perror("send error");
		}
	}
	else if 	(strncmp(req_buf, "GET / ", 6) == 0) {
		const char *http_response_ok = "HTTP/1.1 200 OK\r\n"
				"Content-Type: text/plain\r\n"
				"Content-Length: 14\r\n"
				"\r\n" // Important: End of headers with an empty line
				"Hello World 2!";
		if (send(client_sfd, http_response_ok, strlen(http_response_ok), 0) == -1) {
			perror("send error");
		}
	}
	else { // Send a 404 Not Found for anything else
		const char *http_response_404 = "HTTP/1.1 404 Not Found\r\n" 
				"Content-Type: text/plain\r\n" 
				"Content-Length: 13\r\n" "\r\n" 
				"404 Not Found";  
		if (send(client_sfd, http_response_404, strlen(http_response_404), 0) == -1) {
			perror("send error");
		}
	}
	close(client_sfd); // Close the client-specific connection
}

//argv[] contains only ports in this program whereas we will need to handle
//a configuration file
int main(int argc, char *argv[]) { 
	int				sfd, s; 
	struct addrinfo	hints; 
	struct addrinfo *result, *rp; 
	int             yes = 1; // For setsockopt SO_REUSEADDR  
	if (argc != 2) { 
		fprintf(stderr, "Usage: %s port\n", argv[0]); 
		exit(EXIT_FAILURE); 
	}  
	memset(&hints, 0, sizeof(hints)); 
	hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */ 
	hints.ai_socktype = SOCK_STREAM; /* <--- TCP stream socket */ 
	hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */ 
	hints.ai_protocol = 0;           /* Any protocol */  
	
	s = getaddrinfo(NULL, argv[1], &hints, &result); 
	
	if (s != 0) { 
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s)); 
		exit(EXIT_FAILURE); 
	}  
	for (rp = result; rp != NULL; rp = rp->ai_next) { 
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1) 
			continue;  // Optional: Helps in quickly restarting the server after a crash 
		if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
			perror("setsockopt"); 
			close(sfd); continue; 
		}  
		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) 
			break;                  /* Success */
		close(sfd); 
	}  
	freeaddrinfo(result);           /* No longer needed */  
	if (rp == NULL) {               /* No address succeeded */ 
		fprintf(stderr, "Could not bind\n"); 
		exit(EXIT_FAILURE);
	}  // --- NEW: Listen for incoming connections ---
	if (listen(sfd, 10) == -1) { // 10 is the backlog queue size
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %s\n", argv[1]);
	// --- NEW: Accept client connections in a loop ---
	for (;;) {
		struct sockaddr_storage peer_addr;
		socklen_t peer_addrlen = sizeof(peer_addr);
		int client_sfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addrlen);
		if (client_sfd == -1) {
			perror("accept");
			continue; // Keep trying to accept
		}
		// For logging who connected
		char host[NI_MAXHOST], service[NI_MAXSERV];
		s = getnameinfo((struct sockaddr *)&peer_addr, peer_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
		if (s == 0)
			printf("Accepted connection from %s:%s\n", host, service);
		else
			fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
		handle_client(client_sfd); // Handle the client's request
	}  // Note: sfd is never closed in this infinite loop, which is typical for servers.
	// Cleanup would happen on program exit.
	// close(sfd);
	// Should technically be here if breaking the loop, but server runs indefinitely
	return 0; 
}