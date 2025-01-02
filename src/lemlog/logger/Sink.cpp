#include "lemlog/logger/Sink.hpp"
#include <algorithm>

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

void log(Level level, const std::string& topic, const std::string& message) {
    // send the message to all sinks
    for (Sink* const sink : sinks) {
        sink->send(level, topic, message);
    }
}

Sink::Sink(std::string name)
    : m_name(name) {
    sinks.push_back(this);
}

void Sink::addToAllowList(const std::string& topic) {
    m_allowList.emplace_back(topic);
}

void Sink::removeFromAllowList(const std::string& topic) {
    m_allowList.remove(topic);
}

void Sink::addToBlockedList(const std::string& topic) {
    m_blockedList.emplace_front(topic);
}

void Sink::removeFromBlockedList(const std::string& topic) {
    m_blockedList.remove(topic);
}

void Sink::setLoggingLevel(Level level) { m_minLevel = level; }

const std::string& Sink::getName() const& { return m_name; }

SinkStatus Sink::send(Level level, const std::string& topic,
                      const std::string& message) {
    // Check if the topic is on the allowlist
    if (std::find(m_allowList.begin(), m_allowList.end(), topic) !=
        m_allowList.end()) {
        return this->write(level, topic, message);
    }

    // Check if the topic is on the blocked list
    if (std::find(m_blockedList.begin(), m_blockedList.end(), topic) !=
        m_blockedList.end()) {
        return SinkStatus::OK;
    }

    // Check if the level is >= the minimum logging level
    if (static_cast<int>(level) >= static_cast<int>(m_minLevel)) {
        return SinkStatus::OK;
    }

    // Default case: write the message
    return this->write(level, topic, message);
}

Sink::~Sink() { sinks.erase(this); }
} // namespace logger
