


#include "image_url_generator.h"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace utils{
    std::string generate_image_url(const std::string& image_name) {
        fs::path path = fs::current_path() / "assets" / "pics" / image_name;

        if(fs::exists(path)) {
            return "/images/" + image_name;
        } else {
            return "";
        }
    }
}