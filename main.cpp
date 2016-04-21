/**
 * Main entry point.
 */

#include "stdinclude.h"
#include "data_reader.h"

class Foo : public DataConsumer {

    virtual void addInt(int val, string::size_type len) override {
        cout << val << " ";
    }

    virtual void endRow() override {
        cout << endl;
    }
};

int main() {
    Foo foo;
    DataReader::read_file("test_data/t1.txt", foo);

    return 0;
}
