#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include "Parsing/ParsingUtils.hpp"
#include <fstream>
#include <vector>
#include <algorithm>

class Server {
	private:
		std::vector<Location> locations;
		ServerData servData;

	public:
		Server();
		Server(const Server &serv);
		Server &operator=(const Server &serv);
		~Server();

		void pushBackLocation(Location loc);
		void setData(ServerData data);

		/* #region getter */
		const Location& getLocation(std::string path) const;
		std::vector<Location>& getLocations();
		std::string getPort() const;
		int getBodySize() const;
		std::string getName() const;
		std::string getAddr() const;
		std::string getJsPath() const;
		std::string get404Path() const;
		std::string getIdxPath() const;
		std::string getImgPath() const;
		std::string getCssPath() const;
		std::string getRootPath() const;
		/* #endregion */
};

Server parseServer(String::Reader &sr);
#endif