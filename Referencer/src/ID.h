#pragma once
#include <iostream>
#include <atomic>

namespace Referencer {
    class ID {
    public:
        ID() : id(generateID()) {}

        // Copy constructor this has to be remade so that it can point to object from which it was copyed + hat its own ID
        ID(const ID& other) : id(generateID()) {}

        ID& operator=(const ID& other) {
            if (this != &other) {
                id = other.id;
            }
            return *this;
        }

        bool operator==(const ID& other) const {
            return id == other.id;
        }

        bool operator!=(const ID& other) const {
            return id != other.id;
        }

        int get() const {
            return id;
        }

    private:
        // Function to generate a unique ID
        static int generateID() {
            static std::atomic<int> counter(0);
            return counter++;
        }

        int id;
    };


}
