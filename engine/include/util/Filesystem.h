#pragma once

#include <string>

class Filesystem {
public:

	Filesystem(Filesystem const&) = delete;
	~Filesystem() = default;

	static Filesystem& instance();

	std::string basePath;

	std::string getBasePath();

	std::string getFileContents(const std::string& relativePath);

	std::string resolveRelativePath(const std::string& relativePath) {
		return basePath + relativePath;
	}

private:

	Filesystem();

};
