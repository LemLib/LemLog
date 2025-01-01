#include "lemlog/logger/logger.hpp"

namespace logger {

template <typename T>
class Node;

/**
 * @brief Doubly linked list
 *
 * The linked list in the standard library cannot be initialized at compile
 * time. If we were to use the stdlib implementation, we wouldn't be able to
 * create sinks during static initialization, as the sinks may initialize before
 * the list of sinks do
 *
 * @tparam T the element to store
 */
template <typename T>
class List {
    public:
        void push_back(T value) {
            if (exists(value)) return;
            Node<T>* const last = getLast();
            Node<T>* const newLast = new Node<T>(value);
            if (last == nullptr) {
                m_first = newLast;
            } else {
                last->next = newLast;
            }
            newLast->prev = last;
        }

        Node<T>* getLast() {
            if (m_first == nullptr) {
                return nullptr;
            } else {
                Node<T>* res = m_first;
                while (res->next != nullptr) {
                    res = res->next;
                }
                return res;
            }
        }

        bool exists(T element) {
            Node<T>* ptr = m_first;
            while (ptr != nullptr) {
                if (ptr->value == element) return true;
            }
            return false;
        }

        void erase(T element) {
            Node<T>* ptr = m_first;
            while (ptr != nullptr) {
                if (ptr->value == element) delete ptr;
            }
        }

        // Access operator
        T& operator[](size_t index) const {
            Node<T>* last = m_first;
            for (int i = 0; i < index; ++i) {
                last = last->next;
            }
            return last->value;
        }

        // Size getter
        size_t size() const {
            Node<T>* last = m_first;
            size_t s = 0;
            while (last != nullptr) {
                last = last->next;
                ++s;
            }
            return s;
        }

        const T* begin() const { return &m_first->value; }

        const T* end() const { return &(*this)[size()]; }
    private:
        Node<T>* m_first = nullptr;
};

template <typename T>
class Node {
        friend List<T>;
    public:
        Node(T value)
            : value(value) {}
    private:
        ~Node() {
            if (prev != nullptr) prev->next = this->next;
        }

        T value;
        Node<T>* prev = nullptr;
        Node<T>* next = nullptr;
};

// doubly linked list of all sinks
static constinit List<Sink*> sinks;
static constinit List<std::string> whitelist;
static constinit List<std::string> blacklist;

void addWhitelist(std::string s) { whitelist.push_back(s); }

void removeWhitelist(std::string s) { whitelist.erase(s); }

void addBlacklist(std::string s) { blacklist.push_back(s); }

void removeBlacklist(std::string s) { blacklist.erase(s); }

void log(Level level, std::string topic, std::string message) {
    // is the message a debug message?
    if (level == Level::DEBUG) {
        // is it whitelisted?
        bool found = false;
        for (const std::string& s : whitelist) {
            if (topic == s) {
                found = true;
                break;
            }
        }
        if (!found) return;
    } else {
        // is the message blacklisted?
        for (const std::string& s : blacklist) {
            if (topic == s) return;
        }
    }
    // send the message to all sinks
    for (Sink* const sink : sinks) {
        sink->send(level, topic, message);
    }
}

Helper::Helper(std::string topic)
    : m_topic(topic) {}

Sink::Sink() { sinks.push_back(this); }

Sink::~Sink() { sinks.erase(this); }
} // namespace logger
