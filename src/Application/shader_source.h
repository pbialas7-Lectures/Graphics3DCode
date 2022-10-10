#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

namespace xe
{
    namespace utils
    {
        class source_t
        {
        public:
            source_t() = default;

            source_t(source_t &other) : src(other.src) { other.clear(); };

            source_t &operator=(source_t &rhs)
            {
                std::swap(this->src, rhs.src);
                rhs.clear();
                return *this;
            };

            void clear()
            {
                for (auto p : src)
                {
                    if (p != nullptr)
                    {
                        delete[] p;
                    }
                }
                src.clear();
            }
            ~source_t() { clear(); }

            void print(std::ostream &stream) const;

            friend std::ostream &operator<<(std::ostream &stream, const source_t &src)
            {
                src.print(stream);
                return stream;
            };

            bool empty() const { return src.empty(); }
            size_t size() const { return src.size(); }
            char *const *data() const { return src.data(); }

            void push_back(char *elem) { src.push_back(elem); }

            void push_back_string(const std::string &str);
            void load(const std::string &path, bool single_string = false);

            std::vector<char *>::iterator find_version_line();

            char *replace_version(const std::string &version);

        private:
            std::vector<char *> src;
        };
    }
}