𝘛𝘩𝘪𝘴 𝘱𝘳𝘰𝘫𝘦𝘤𝘵 𝘩𝘢𝘴 𝘣𝘦𝘦𝘯 𝘤𝘳𝘦𝘢𝘵𝘦𝘥 𝘢𝘴 𝘱𝘢𝘳𝘵 𝘰𝘧 𝘵𝘩𝘦 42 𝘤𝘶𝘳𝘳𝘪𝘤𝘶𝘭𝘶𝘮 𝘣𝘺 𝘺𝘳𝘰𝘢𝘳𝘥 𝘢𝘯𝘥 𝘵𝘵𝘢𝘴

Description :
	Webserv is a custom web server project developed as part of the 42 curriculum. The goal of this project is to build a fully functional HTTP/1.1 web server from scratch, capable of handling client requests, and managing multiple connections efficiently.

	This project emphasizes understanding low-level networking, process and socket management, and HTTP protocol mechanics. Webserv supports features such as request parsing, response generation, error handling, and configurable server behavior through a simple configuration file. By completing this project, students gain hands-on experience with server-side programming and learn the foundations of web server architecture.

Instructions :

	To start the server, you will need a configuration file, said file needs to have the *.conf extention.
	The format of the file is as follows :
		server {
			// contains the content of the server and can have multiple servers
			server_name name	// the name of the server
			host host_ip		// the ip needed to browse the server
			port port_nbr		// the port to which the server will listen
			client_max_body 100000		// the max size of the client request body
			error_page error_code page/directory/to/the_page		//can have multiple pages, which have to be declared in separate lines
			
			route / {		// there can be multiple routes, which can be declared after this one
				route_name /
				allowed_methods GET	// methods allowed in this route

				root root_directory
				index index_page
				upload_status 0		// 0/1 (0 is off, 1 is on)
				autoindex 0			// same as upload status

				cgi_path /usr/bin/php-cgi	// left in config file for better ease of access/setup
				cgi_ext .php				// CGI extentions allowed
			}

			route /something {
				// to show how to declare multiple routes
			}
		}
		server {
			// same format as the previous one
		}

	once the conf file is setup, go to the root directory and do :
		- make re
		- ./webserv ./path/to/conf/default.conf
		
	After starting the server, you will need to open a browser and enter server_ip:server_port to access the server



Ressources :
	AI was used mostly for information gathering and debugging (check print functions), as well as repetitive tasks (Error pages) and writing this Readme.md

	- Overall :
	https://www.figma.com/board/uhGOOwWJVKSQrdwLc31jYh/Webserv?node-id=0-1&p=f&t=OYxIPOu6uhDw8zSH-0

	- Requete
	https://www.pierre-giraud.com/http-reseau-securite-cours/requete-reponse-session/

	- Web
	https://www.w3schools.com/php/php_file_upload.asp
	https://www.php.net/manual/en/features.file-upload.php
