#pragma once

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

class str {
public:
	template <typename T> static std::string from_number(T val);
	static std::string lcase(const std::string& str);
	static std::string replace(const std::string& str, const std::string& find, const std::string& rep);
	static std::string read(const std::string& filename);
	static void write(const std::string& str, const std::string& filename, bool append);
	static std::string strip_ext(const std::string& str);
	static std::string strip_dir(const std::string& str);
	static std::string extract_ext(const std::string& str);
	static std::string extract_dir(const std::string& str);
	static std::vector<std::string> split(const std::string& str, char delim);
private:
	str() {}
	~str() {}
};

template <typename T>
std::string str::from_number(T val) {
	std::ostringstream stream;
	stream << val;
	return stream.str();
}

inline std::string str::lcase(const std::string& str) {
	std::string copy = str;
	std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
	return copy;
}

inline std::string str::replace(const std::string& str, const std::string& find, const std::string& rep) {
	std::string strcopy = str;
	while ( strcopy.find(find) != std::string::npos ) {
		strcopy.replace(strcopy.find(find), find.length(), rep);
	}
	return strcopy;
}

inline std::string str::read(const std::string& filename) {
	std::ifstream istream(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	std::stringstream sstream;
	sstream << istream.rdbuf();
	istream.close();
	return sstream.str();
}

inline void str::write(const std::string& str, const std::string& filename, bool append = true) {
	std::ofstream ostream(filename.c_str(), std::ios_base::out | std::ios_base::binary | (append ? std::ios_base::app : std::ios_base::trunc));
	ostream << str;
	ostream.close();
}

inline std::string str::strip_ext(const std::string& filename) {
	return filename.substr(0, filename.find('.'));
}

inline std::string str::strip_dir(const std::string& filename) {
	std::string filenameCopy = replace(filename, "\\", "/");
    size_t pos = filenameCopy.rfind('/');
    if ( pos != std::string::npos ) {
        return filenameCopy.substr(pos+1, filenameCopy.length() - pos - 1);
    } else {
        return filename;
    }
}

inline std::string str::extract_ext(const std::string& filename) {
    size_t pos = filename.rfind('.');
    if ( pos != std::string::npos ) {
        return filename.substr(pos + 1, filename.length() - pos - 1);
    } else {
        return "";
    }
}

inline std::string str::extract_dir(const std::string& filename) {
	std::string filenameCopy = replace(filename, "\\", "/");
    size_t pos = filenameCopy.rfind('/');
    if ( pos != std::string::npos ) {
        return filenameCopy.substr(0, pos);
    } else {
        return "";
    }
}

inline std::vector<std::string> str::split(const std::string& str, char delim) {
	std::vector<std::string> elems;
	std::stringstream sstream(str);
	std::string item;
	while (std::getline(sstream, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
