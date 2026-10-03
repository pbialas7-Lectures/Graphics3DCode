#include "Application/shader_source.h"
#include <cstring>

#include "spdlog/spdlog.h"

namespace {
    char *copy_string_to_char(std::string str, const std::string &suffix = "") {
        auto line = new char[str.size() + suffix.size() + 1];
        std::memcpy(line, str.c_str(), str.size());
        std::memcpy(line + str.size(), suffix.c_str(), suffix.size());
        line[str.size() + suffix.size()] = '\0';
        return line;
    }
}

namespace xe {
    namespace utils {

        void source_t::push_back_string(const std::string &str) {
            auto line = copy_string_to_char(str, "\n");
            push_back(line);
        }

        void source_t::load(const std::string &path, bool single_string) {
            if (!single_string) {

                std::ifstream file(path, std::ios::in);
                if (file) {
                    std::string str;
                    while (std::getline(file, str)) {
                        push_back_string(str);
                    }
                    file.close();
                } else {
                    spdlog::error("Cannot load shader source from `{}'", path);
                }

            } else {
                std::ifstream file(path, std::ios::in | std::ios::binary);
                std::ostringstream contents;
                contents << file.rdbuf();
                push_back_string(contents.str());
                file.close();
            }
        }

        void source_t::print(std::ostream &stream) const {
            for (auto line: src) {
                if (line != nullptr) {
                    stream << line;
                }
            }
        }

    }
}