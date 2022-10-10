#include "Application/shader_source.h"
#include <regex>

namespace {
    char *copy_string_to_char(std::string str, const std::string &suffix = "") {
        auto line = new char[str.size() + suffix.size() + 1];
        std::memcpy(line, str.c_str(), str.size());
        std::memcpy(line + str.size(), suffix.c_str(), suffix.size());
        line[str.size() + suffix.size()] = '\0';
        return line;
    }

    char *replace_version(const char *version_line, const std::string &version) {
        std::regex version_regex("#version\\s+(\\d+)");
        auto replaced = std::regex_replace(version_line, version_regex, std::string("#version ") + version);
        auto replaced_p = copy_string_to_char(replaced);
        return replaced_p;
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
                    std::cerr << "Cannot load shader source from`" << path << "'\n";
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

        std::vector<char *>::iterator source_t::find_version_line() {
            std::regex version_regex("#version\\s+(\\d+)");
            auto line_number = 0;
            for (auto line = src.begin(); line != src.end(); line++) {
                std::cmatch version_match;
                if (std::regex_search(*line, version_match, version_regex)) {
                    return line;
                }
                line_number++;
            }
            return src.end();
        }

        char *source_t::replace_version(const std::string &version) {
            auto version_line = find_version_line();
            auto new_version = ::replace_version(*version_line, version);
            delete[] *version_line;
            *version_line = new_version;

            return new_version;
        }

    }
}